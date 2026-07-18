# 💻 MOSSS Software & Configuration Deployment Guide

This directory contains the core software components, firmware, and automation logic that power the **Modular and Open-Source Science Station (MOSSS)** network topology. 

The software ecosystem is split into two primary layers: **Edge Node Firmware** (running on the physical detectors) and **Gateway Orchestration** (running on your central local hub).

---

## 📂 Directory Structure

*   **`./Landslide-Detectors/`**: Contains the Arduino-based firmware sketches for the physical WOILD (Wake on Interrupt Landslide Detector) field nodes.
*   **`./Home-Assistant/`**: Contains the Home Assistant YAML configurations, custom template sensors, and automations required to process data and trigger alerts.

---

## 🛠️ Step-by-Step Implementation

To deploy this software package on your own network, follow these configuration steps in order.

### Step 1: Flash the WOILD Hardware Nodes
1. Navigate to the `./Landslide-Detectors/` folder.
2. Open the primary `.ino` sketch file in the **Arduino IDE** (ensure you have installed your microcontroller's board definition package, e.g., ESP32 or SAMD21).
3. Configure your local network parameters (LoRaWAN keys/credentials or Wi-Fi configurations) within the `config.h` or configuration section at the top of the script.
4. Compile and flash the firmware onto your physical detector nodes. 
    > 💡 *Note: The firmware utilizes hardware interrupt mapping to keep the MCU in a deep-sleep state until a specific acceleration or tilt threshold is crossed, optimizing battery life.*

### Step 2: Configure the Home Assistant Gateway
1. Ensure you have a local instance of **Home Assistant Core** running on your gateway hardware.
2. Open the `./Home-Assistant/` folder in this repository.
3. Copy the custom template sensors from our template file and paste them into your local `configuration.yaml` file. These templates process raw inputs into the live **2D Vector Deviation Matrix**.
4. Import the alert automations into your local `automations.yaml` file. 

### Step 3: Map Inputs to Your Meteorological Sensors
1. Ensure your **EcoWitt** weather gateway is integrated with Home Assistant.
2. Map your local rain rate sensor entity names to match the variables used in the imported automations to ensure proper microclimate correlation logic is functional.

---

## 🔔 How the Alert Logic Works

Once deployed, the software relies on a dual-trigger mechanism to completely eliminate false alerts from environmental noise (like wind or passing animals):

1. **The Wake-on-Interrupt Event:** The physical node remains completely silent until a physical threshold is breached, waking up instantly to blast a high-priority telemetry payload to the gateway.
2. **The Vector Deviation Matrix Evaluation:** Home Assistant intercepts the payload, runs it through the template matrix equations, evaluates the concurrent localized rain rate from your EcoWitt data, and instantly fires a mobile emergency notification if both thresholds match risk parameters.