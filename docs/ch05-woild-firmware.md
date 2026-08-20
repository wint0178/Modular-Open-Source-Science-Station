# 🌲 Chapter 5: WOILD Firmware Deployment & Payload Architecture

The WOILD (Wake On Interrupt Landslide Detector) firmware family is engineered for ultra-low-power edge nodes deployed in high-risk landslide zones. Operating primarily in a deep-sleep state, the microcontroller utilizes hardware interrupt mapping to wake instantly on motion threshold breaches or transmit periodic baseline telemetry over LoRaWAN.

---

## ⚡ Power Management & Operational Modes

To maximize solar/battery longevity in field deployments, the WOILD firmware switches between three operational modes governed by onboard hardware interrupts:

| Mode ID | Mode Name | Behavior & Power Draw | Transmission Trigger |
| :---: | :--- | :--- | :--- |
| **`0`** | **Timed Baseline** | MCU remains in Deep Sleep (~15µA draw). Wakes briefly via internal RTC timer. | Scheduled heartbeat uplink (e.g., every 60 mins). |
| **`1`** | **Alert (Threshold)** | Accelerometer interrupt fires on motion ($>32\text{ mg}$ threshold). Node enters high-frequency sampling. | Immediate event-driven packet publish. |
| **`2`** | **Tripwire Interlock** | External physical continuity circuit (e.g., breakaway ground cable) snaps or breaks connection. | Instant emergency priority uplink with hardware flag bit set. |

---

## 📦 Firmware Version Comparison & Selection Guide

Several firmware builds are maintained under `../software/Landslide-Detectors/`. Choose the version that best matches your deployment needs:

| Version | Key Focus & Innovation | OTA Downlink Control | Dynamic Sleep Delays | Ideal Deployment Scenario |
| :---: | :--- | :---: | :---: | :--- |
| **[v1.1.3](../software/Landslide-Detectors/Wake_On_Interrupt_Landslide_Detector_v113/)** | **Power Conservation Baseline**<br>Dual motion thresholds and low-voltage auto-recovery. | ❌ Static | ❌ Hardcoded | Simple, static field nodes with standard solar/battery hardware. |
| **[v1.1.5](../software/Landslide-Detectors/Wake_On_Interrupt_Landslide_Detector_v115/)** | **Dynamic NVS & Bitpacked Overhaul**<br>Dynamic NVS thresholds and bitpacked payload overhaul. | **Yes** (2-byte frame) | ❌ Hardcoded | Remote nodes needing dynamic threshold and motion tuning over-the-air. |
| **[v1.1.6](../software/Landslide-Detectors/Wake_On_Interrupt_Landslide_Detector_v116/)** *(Latest)* | **Full Remote Command & Control**<br>Dynamic sleep lookups, OTA tripwire toggling, and NVS persistence. | **Yes** (2-byte frame) | **Yes** (Lookup-based) | **Recommended for production.** Remote nodes where over-the-air parameter tuning is required. |

### Which Version Should You Implement?

* **Choose WOILD v1.1.6 (Recommended for Production):** Represents the cumulative production release. It adds **bi-directional LoRaWAN downlinks**, allowing operators to remotely adjust sleep/wake delays (`MINIMUM_DELAY_LOOKUP_SEC` and `TRIP_ALERT_LOOKUP_SEC`) and toggle the physical tripwire interlock on/off via TTN without climbing up the mountain to re-flash the MCU. All configuration states are persisted across deep sleep cycles in non-volatile storage (NVS) via the ESP32 Preferences API.
* **Choose WOILD v1.1.5 (Dynamic Threshold Tuning):** Adds NVS flash persistence (`Preferences` API) to store dynamic motion settings across sleep cycles and processes 2-byte LoRaWAN downlinks to dynamically update Motion Threshold ($0–224\text{ mg}$), Software Gate ($0–224\text{ mg}$), and Duration Index ($10–2000\text{ ms}$).
* **Choose WOILD v1.1.3 (Minimalist Baseline):** Offers a lightweight, non-configurable baseline. It introduces two separate gravitational thresholds (wake-on-interrupt vs. transmit-on-motion) and enters an automatic **20-hour low-voltage hibernation mode** if the battery drops below $3.0\text{V}$, giving small solar panels time to recharge the cell without brownout loops.

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
Navigate to `../software/Landslide-Detectors/` and open the primary `.ino` sketch file for your chosen version.

### Step 2: Compile & Flash Microcontroller
1. Connect the node to your workstation using a data-rated USB cable.
2. Select your target board under **Tools $\rightarrow$ Board** and select the active port under **Tools $\rightarrow$ Port**.
3. In the **Tools** menu, enable **"Erase all flash before sketch upload"** and select your desired LoRa frequency (e.g., `US915`).
4. Click **Verify** (Checkmark icon) to confirm dependencies resolve cleanly.
5. Click **Upload** (Right arrow icon) to write the compiled binary payload to node flash memory.

### Step 3: Node Provisioning in TTN
1. In the TTN Console, select **Add** and then **Enter end device specifics manually**. Set the parameters as follows:
   * **Frequency Plan:** United States 902-928 MHz, FSB 2
   * **LoRaWAN Version:** 1.1.0
   * **Regional Parameters Version:** 1.1 revision B
   * **JoinEUI:** Enter all zeros (`00 00 00 00 00 00 00 00`).
2. Select **Confirm** to proceed.
3. Click **Generate** for `DevEUI`, `AppKey`, and `NwkKey`. Name your device using lowercase letters and hyphens with no spaces (e.g., `my-new-device`), then click **Register end device**.

![TTN Node Provisioning](/Science-Station/images/TTN-new-node.png)

### Step 4: Deploy TTN / ChirpStack Payload Decoder
1. Open the [software directory](../software/Landslide-Detectors/) where your chosen firmware is located, open the `.js` payload formatter file, and copy the code.
2. Log into your **The Things Network (TTN) Console** or ChirpStack server.
3. Navigate to **Applications $\rightarrow$ [Your Application] $\rightarrow$ Payload Formatters $\rightarrow$ Uplink**.
4. Select **Custom JavaScript**, paste the script, scroll to the bottom to locate where the node ID is established, change it to a unique value (e.g., `LD01`, `LD02`, `LD03`, etc.), and click **Save Changes**.
5. Repeat this process with the downlink formatter if one is provided for your firmware version.

### Step 5: Post-Flash Verification & Initial Join
1. Open the Arduino **Serial Monitor** set to `115200` baud.
2. Verify initialization messages. The device will wait for the TTN provisioning IDs. Enter them when prompted:
   * **Frequency:** `US915`
   * **Subband:** `2`
   * **DevEUI:** `0000000000000000`
   * **AppKey:** `YOUR_APPKEY` formatted as `12345678910112131415161718192021`
   * **NwkKey:** `YOUR_NWKKEY` formatted as `12345678910112131415161718192021`
3. The node will attempt to join TTN and send its first transmission. Monitor and verify the join sequence in the Arduino Serial Monitor and in the TTN Live Data feed.

### Step 6: Establish Calibration & Static Baselines
1. Secure your hardware node inside its static field tracking enclosure.
2. Run the initial calibration test routine to calculate resting 2D surface angles.
3. If onboard EEPROM auto-calibration is disabled, hardcode these baseline offsets directly into the global node configuration parameters.
4. Once verified, the node is ready for field mounting.

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

To render WOILD telemetry into active entities within Home Assistant, append some or all of the provided YAML configurations to your respective Home Assistant files using the File Editor add-on (e.g., adding these template definitions to `configuration.yaml` or `templates.yaml`). All provided YAML files are located in the [software directory](../software/Home-Assistant/Runtime-Configuration/).

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
        state: "{{ state_attr('sensor.woild_node_01_tilt_x') }}"

      - name: "Node 01 Tilt Y-Axis"
        unique_id: "woild_node_01_tilt_y"
        unit_of_measurement: "°"
        state: "{{ state_attr('sensor.woild_node_01_tilt_y') }}"

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