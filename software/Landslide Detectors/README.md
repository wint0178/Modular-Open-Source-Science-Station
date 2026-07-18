WOILD HARDWARE FIRMWARE DEPLOYMENT GUIDE (V1.1.3)
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
