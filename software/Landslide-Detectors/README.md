WOILD HARDWARE FIRMWARE DEPLOYMENT GUIDE (V1.1.6)
==============================================================================

This directory contains the core Arduino sketches required to configure, compile, 
and flash the hardware nodes for the Multi-Node Landslide Detection Grid.

------------------------------------------------------------------------------
PREREQUISITES & DEVELOPMENT ENVIRONMENT
------------------------------------------------------------------------------
Before compiling the sketch, ensure your development environment is properly 
configured:

  1. IDE Installation:
     * Use Arduino IDE v2.0 or higher.
     
  2. Core Board Support Packages:
     * Open Tools > Board > Boards Manager...
     * Search for and install the core package matching your micro-controller 
       architecture (e.g., ESP32, SAMD21, STM32, or AVR depend on your hardware build).

  3. Required Hardware Libraries:
     Ensure the following external libraries are installed via the Library Manager 
     (Tools > Manage Libraries...):
     * Accelerometer / Inclinometer Library (e.g., ADXL345, LIS3DH, or MPU6050)
     * Temperature/Humidity Sensor Library (e.g., DHT, SHT3x, or BME280)
     * LoRa / Communication Network Library (e.g., RadioHead or LoRaWAN LMIC)

------------------------------------------------------------------------------
STEP-BY-STEP FIRMWARE CONFIGURATION
------------------------------------------------------------------------------

  1. Open the primary sketch file (.ino) in this directory.
  
  2. Node Identification (CRITICAL):
     Locate the global configuration constants at the top of the main file. 
     Before flashing a board, you MUST update its individual network ID:
     
     ex:  #define NODE_ID 1    // Change to 2 for LD02, 3 for LD03, etc.
     
  3. Calibration & Baselines:
     * Secure your hardware node in its static tracking enclosure.
     * Use the initial setup test routine to calculate the resting 2D surface angles.
     * Hardcode these specific baseline values into the node parameters if 
       on-board EEPROM auto-calibration is disabled.

------------------------------------------------------------------------------
COMPILING AND FLASHING THE HARDWARE
------------------------------------------------------------------------------

  1. Interface Connection:
     * Connect your hardware node to your deployment computer via a high-quality 
       data-rated USB micro/Type-C cable.
       
  2. Port and Board Mapping:
     * Navigate to Tools > Board and select your specific hardware layout.
     * Navigate to Tools > Port and select the active COM/Serial interface 
       associated with your connected hardware.

  3. Verification:
     * Click the "Verify" button (Checkmark icon) to compile the source code.
     * Ensure there are no compilation errors or missing library dependencies.

  4. Flashing:
     * Click the "Upload" button (Arrow icon) to write the compiled binary 
       payload onto the node microcontroller memory storage.
     * Maintain the data connection until the IDE console reports "Done uploading".

------------------------------------------------------------------------------
DEPLOYING THE PAYLOAD FORMATTER (TTN / CHIRPSTACK)
------------------------------------------------------------------------------

  1. Open the file 'WOILD_v1.1.6_payload_formatter.js' located in this directory.
  2. Copy the entire JavaScript snippet.
  3. Log into your TTN Console or ChirpStack network server.
  4. Navigate to Applications > [Your Application] > Payload Formatters > Uplink.
  5. Select 'Custom JavaScript' (or equivalent), paste the script into the editor, 
     and save changes.
     
------------------------------------------------------------------------------
POST-DEPLOYMENT TELEMETRY VERIFICATION
------------------------------------------------------------------------------

  1. Open the Arduino Serial Monitor (Tools > Serial Monitor).
  2. Match the baud rate to the value defined inside your sketch's Serial.begin() 
     statement (typically 9600 or 115200).
  3. Look for the initialization sequence printouts:
     * Sensor sensor checks (Inclinometer, Telemetry matrix, Battery tracking)
     * Network packet initialization status
     * Transmit status verification showing active Nonce updates
  4. Once confirmed functional, deploy the hardware node to its physical field station 
     and verify the Home Assistant 'LD01' dashboard entity receives payload states.
==============================================================================

## 🛠️ Payload Format (WOILD v1.1.6)

The WOILD v1.1.6 firmware uses an ultra-dense **8-byte binary payload** designed for long battery endurance and efficient LoRaWAN airtime utilization:

| Byte | Field | Bit Allocation & Range | Physical Resolution / Function |
| :--- | :--- | :--- | :--- |
| **0** | Nonce | Bits 0–7 | 8-bit Random Execution Nonce |
| **1** | Config Indices | Bits 0–2: `cfg_motion`<br>Bits 3–5: `cfg_sw`<br>Bits 6–7: `dur_low2` | Motion Threshold ($32\text{ mg}$ steps)<br>Software Threshold ($32\text{ mg}$ steps)<br>Duration lower 2 bits |
| **2** | Battery Voltage | Bits 0–7 | Range: $2.0\text{V} - 4.55\text{V}$ ($0.01\text{V}$ step) |
| **3** | Angle X (Upper) | Bits 0–7 | Upper 8 bits of 9-bit signed X angle |
| **4** | Angle Y (Upper) | Bits 0–7 | Upper 8 bits of 9-bit signed Y angle |
| **5** | Shared Control Byte | Bit 0: `x_lsb`<br>Bit 1: `y_lsb`<br>Bits 2–3: `stateMode`<br>Bit 4: `dur_top1`<br>Bits 5–7: `vector_3bit` | LSB for X Angle<br>LSB for Y Angle<br>System Mode ($0=\text{Timed}, 1=\text{Alert}, 2=\text{Tripwire}, 3=\text{Reserved}$)<br>Duration Bit 2 ($1–15\text{ ms}$ lookup)<br>Peak Dynamic Vector ($8\text{ mg}$ steps) |
| **6** | Humidity | Bits 0–7 | Relative Humidity ($1\%$ resolution) |
| **7** | Temp & Flags | Bits 0–5: Temperature<br>Bit 6: `use_tripwire`<br>Bit 7: Reserved | MPU Temperature ($0–63^\circ\text{C}$, $1^\circ\text{C}$ resolution)<br>Tripwire hardware flag |
