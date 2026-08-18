# 📡 MOSSS Software & Configuration Deployment Guide

This directory contains the core software components, custom firmware, and automation logic that power the network topology of the **Modular and Open-Source Science Station (MOSSS)**. 

The software ecosystem is split into three modular pillars: **Long-Range LoRaWAN Nodes**, **Flexible Wi-Fi & Remote I/O Links**, and **Central Gateway Orchestration**.

---

## 📂 Directory Structure

*   **`./Home-Assistant/`**: Contains the central operating system guidelines, multi-tiered user privileges, custom template sensors, and data-logging optimizations required to process field inputs locally.
*   **`./Landslide-Detectors/`**: Contains the firmware sketches, hardware interrupt configurations, and JavaScript payload decoders for the primary, field-deployed **WOILD v1.1.6** (Wake-On-Interrupt Landslide Detector) LoRaWAN arrays.
*   **`./ESPHome/`**: Contains production-ready YAML firmware templates for ESP32 and ESP8266 microcontrollers. Supports local Wi-Fi telemetry ingestion, remote connectivity via DDNS and port forwarding, and specialized ESP32-S3 builds compiling Tailscale client binaries (esphome-tailscale.yaml) to tunnel encrypted telemetry across networks without firewall modifications.

---

## 📝 Step-by-Step Implementation

To deploy this integrated environmental intelligence package on your network infrastructure, execute these configuration steps sequentially:

### Step 1: [Configure the Home Assistant Gateway](/software/Home-Assistant/)
1. Navigate to `./Home-Assistant/` and review the installation guide to provision **Home Assistant OS (HAOS)** on a Raspberry Pi 4 or 5.
2. Create a dedicated Machine-to-Machine (M2M) profile called `databroker` (login: `databroker`) under **Settings > People** to handle incoming data streams securely.
3. Configure your local **Recorder** filter component using the provided template to restrict data-logging to a strict 30-day target, protecting your SD card from wear.

### Step 2: [Map Inputs to Your Meteorological Sensors](/software/EcoWitt/)
1. Ensure your local **EcoWitt** weather gateway is integrated with Home Assistant, calibrated to transmit wind, rain, and pressure data updates at a steady **5-minute ingestion cadence**.
    > 💡 *Note: For deployments were your Home Assistant server and GW3000 gateway are not on the same network, an access token can be used to provide remote data streaming and intermitant HA pulldown requests.*
2. Optional: Embed interactive vector models on your dashboard using a standard Webpage Card pointing to a customized Windy.com viewport string.

### Step 3: [Flash the WOILD Hardware Nodes](/software/Landslide-Detectors/)
1. Navigate to `./Landslide-Detectors/`.
2. Open the primary `.ino` sketch file in the **Arduino IDE** or **VS Code / PlatformIO** (ensure microcontroller board definitions are installed).
3. Configure your unique local network credentials (`DevEUI`, `AppEUI`, `AppKey`) within the top configuration block.
4. Compile and flash the **WOILD v1.1.6** firmware. 
    > 💡 *Note: The firmware utilizes hardware interrupt mapping to keep the MCU in deep sleep under normal operating conditions—transmitting baseline telemetry periodically or waking up instantly on motion threshold events.*
5. Copy the custom template sensors into your Home Assistant `configuration.yaml` file to process raw WOILD v1.1.6 payloads into the live **2D Vector Deviation Matrix**.

### Step 4: [Deploy the TTN / ChirpStack Payload Decoder](/software/Landslide-Detectors/Wake_On_Interrupt_Landslide_Detector_v116/)
1. Navigate to `./Landslide-Detectors/`.
2. Open `WOILD_v1.1.6_payload_formatter.js`.
3. Copy the script content and paste it into **The Things Network (TTN) Console** (or ChirpStack) under **Payload Formatters > Uplink**.

### Step 5: [Provision Your Remote ESP32/ESPHome Devices](/software/ESPHome/) (Optional)
1. Navigate to `./ESPHome/`.
2. Open `esphome-tailscale.yaml` and update your `device_id`, generating a unique Base64 API key and fresh Tailscale auth token.
3. Compile and flash your ESP32-S3 boards. On boot, these chips build an internal, encrypted WireGuard tunnel over local Wi-Fi, joining your private mesh to feed data streams across cellular or remote relays.

---

## ⚡ How the Alert Logic Works

Once deployed, the software relies on a dual-trigger mechanism to eliminate false alerts from environmental noise (such as high wind vectors or passing animals):

1. **The Wake-on-Interrupt Event:** The physical landslide tracking node remains silent in deep sleep until an active geometric slope or acceleration threshold is crossed, waking up instantly to blast an alert payload (`stateMode = 1` or `2`) via LoRaWAN or ESPHome.
2. **The Vector Deviation Matrix Evaluation:** Home Assistant intercepts the payload using the `databroker` channel, runs it through internal matrix template equations, evaluates concurrent localized rain accumulation rate from your EcoWitt array, and instantly fires a high-priority mobile notification if both thresholds match critical slope-failure parameters.