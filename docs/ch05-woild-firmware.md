# 🌲 Chapter 5: WOILD Firmware Deployment & Payload Architecture

The WOILD (Wireless Off-grid Inclinometer & Landslide Detector) v1.1.6 firmware is engineered for ultra-low-power edge nodes deployed in high-risk landslide zones. Operating primarily in a deep-sleep state, the microcontroller utilizes hardware interrupt mapping to wake instantly on motion threshold breaches or transmit periodic baseline telemetry over LoRaWAN.

---

## ⚡ Power Management & Operational Modes

To maximize solar/battery longevity in field deployments, the WOILD v1.1.6 firmware switches between three operational modes governed by onboard hardware interrupts:

| Mode ID | Mode Name | Behavior & Power Draw | Transmission Trigger |
| :---: | :--- | :--- | :--- |
| **`0`** | **Timed Baseline** | MCU remains in Deep Sleep (~15µA draw). Wakes briefly via internal RTC timer. | Scheduled heartbeat uplink (e.g., every 60 mins). |
| **`1`** | **Alert (Threshold)** | Accelerometer interrupt fires on motion ($>32\text{ mg}$ threshold). Node enters high-frequency sampling. | Immediate event-driven packet publish. |
| **`2`** | **Tripwire Interlock** | External physical continuity circuit (e.g., breakaway ground cable) snaps or breaks connection. | Instant emergency priority uplink with hardware flag bit set. |

---

## ⚙️ Prerequisites & Environment Setup

Before compiling and flashing the WOILD firmware, configure your local development environment:

* **IDE Requirement:** Arduino IDE v2.0+ or VS Code with the **PlatformIO** extension.
* **Core Board Support:** Install your microcontroller target architecture via **Tools $\rightarrow$ Board $\rightarrow$ Boards Manager** (e.g., ESP32, SAMD21, STM32, or AVR depending on your hardware build).
* **Required Libraries:** Install the following dependencies via **Tools $\rightarrow$ Manage Libraries**:
  * **Inclinometer / Accelerometer:** `ADXL345`, `LIS3DH`, or `MPU6050`
  * **Environmental Sensors:** `DHT`, `SHT3x`, or `BME280`
  * **LoRaWAN Communications:** `MCCI LoRaWAN LMIC library` or `RadioHead`

---

## 🚀 Node Firmware Deployment

Follow this step-by-step procedure to configure, compile, and flash WOILD field nodes.

### Step 1: Configure Node Network Identifiers
Navigate to `../software/Landslide-Detectors/` and open the primary `.ino` sketch file. Locate the global network configuration block at the top of the file:

* **LoRaWAN Keys (OTAA):** Input your TTN application credentials (`DevEUI`, `AppEUI`, `AppKey`).
* **Node Identifier:** Set the unique node integer:
  ```cpp
  #define NODE_ID 1    // Change to 2 for LD02, 3 for LD03, etc.
  ```

### Step 2: Establish Calibration & Static Baselines
Secure your hardware node inside its static field tracking enclosure. Run the initial calibration test routine to calculate resting 2D surface angles. If onboard EEPROM auto-calibration is disabled, hardcode these baseline offsets directly into the global node configuration parameters.

### Step 3: Compile & Flash Microcontroller
1. Connect the node to your workstation using a data-rated USB cable.
2. Select your target board under **Tools $\rightarrow$ Board** and select the active port under **Tools $\rightarrow$ Port**.
3. Click **Verify** (Checkmark icon) to confirm dependencies resolve cleanly.
4. Click **Upload** (Right arrow icon) to write the compiled binary payload to node flash memory.

### Step 4: Deploy TTN / ChirpStack Payload Decoder
1. Open `WOILD_v1.1.6_payload_formatter.js` located in `../software/Landslide-Detectors/`.
2. Copy the entire JavaScript script.
3. Log into your **The Things Network (TTN) Console** or ChirpStack server.
4. Navigate to **Applications $\rightarrow$ [Your Application] $\rightarrow$ Payload Formatters $\rightarrow$ Uplink**.
5. Select **Custom JavaScript**, paste the script, and click **Save Changes**.

### Step 5: Post-Deployment Verification
1. Open the Arduino **Serial Monitor** set to `115200` baud.
2. Verify initialization messages for sensor checks (Inclinometer, Environmental array, Battery tracking) and LoRaWAN join sequence updates.
3. Once verified, deploy the node to its physical field station.

---

## 🛠️ Payload Specification (WOILD v1.1.6)

To maximize battery life and comply with LoRaWAN Fair Use airtime limits, WOILD v1.1.6 packs sensor telemetry, device diagnostics, and interrupt configuration states into an ultra-dense **8-byte binary frame**:

| Byte | Field | Bit Allocation & Range | Physical Resolution / Function |
| :---: | :--- | :--- | :--- |
| **0** | **Nonce** | Bits 0–7 | 8-bit Execution Nonce (Frame tracking & replay prevention) |
| **1** | **Config Indices** | Bits 0–2: `cfg_motion`<br>Bits 3–5: `cfg_sw`<br>Bits 6–7: `dur_low2` | Motion Threshold ($32\text{ mg}$ steps)<br>Software Threshold ($32\text{ mg}$ steps)<br>Duration lower 2 bits |
| **2** | **Battery Voltage** | Bits 0–7 | Voltage Range: $2.0\text{V} - 4.55\text{V}$ ($0.01\text{V}$ step resolution) |
| **3** | **Angle X (Upper)** | Bits 0–7 | Upper 8 bits of 9-bit signed X-axis tilt angle |
| **4** | **Angle Y (Upper)** | Bits 0–7 | Upper 8 bits of 9-bit signed Y-axis tilt angle |
| **5** | **Shared Control Byte** | Bit 0: `x_lsb`<br>Bit 1: `y_lsb`<br>Bits 2–3: `stateMode`<br>Bit 4: `dur_top1`<br>Bits 5–7: `vector_3bit` | LSB for X Angle<br>LSB for Y Angle<br>System Mode ($0=\text{Timed}, 1=\text{Alert}, 2=\text{Tripwire}, 3=\text{Reserved}$)<br>Duration Bit 2 ($1–15\text{ ms}$ lookup)<br>Peak Dynamic Vector ($8\text{ mg}$ steps) |
| **6** | **Humidity** | Bits 0–7 | Relative Humidity ($1\%$ resolution, $0–100\%$) |
| **7** | **Temp & Flags** | Bits 0–5: Temperature<br>Bit 6: `use_tripwire`<br>Bit 7: Reserved | On-chip Temperature ($0–63^\circ\text{C}$, $1^\circ\text{C}$ resolution)<br>Tripwire hardware interlock state flag |

---

## 📊 Home Assistant Sensor Templates (YAML)

To render WOILD telemetry into active entities within Home Assistant, append these template definitions to your `configuration.yaml` or `templates.yaml` file:

~~~yaml
template:
  - sensor:
      - name: "Node 01 Battery Voltage"
        unique_id: "woild_node_01_battery"
        unit_of_measurement: "V"
        device_class: "voltage"
        state: "{{ state_attr('sensor.woild_node_01_telemetry', 'battery_v') }}"

      - name: "Node 01 Tilt X-Axis"
        unique_id: "woild_node_01_tilt_x"
        unit_of_measurement: "°"
        state: "{{ state_attr('sensor.woild_node_01_telemetry', 'tilt_x') }}"

      - name: "Node 01 Tilt Y-Axis"
        unique_id: "woild_node_01_tilt_y"
        unit_of_measurement: "°"
        state: "{{ state_attr('sensor.woild_node_01_telemetry', 'tilt_y') }}"

      - name: "Node 01 System Mode"
        unique_id: "woild_node_01_mode"
        state: >-
          {% set mode = state_attr('sensor.woild_node_01_telemetry', 'stateMode') %}
          {% if mode == 0 %} Timed Baseline
          {% elif mode == 1 %} Alert Event
          {% elif mode == 2 %} Tripwire Interlock
          {% else %} Unknown
          {% endif %}
        icon: "mdi:shield-alert"
~~~

---

👉 **Proceed to [Chapter 6: Field Deployment](./ch06-field-deployment.md)**