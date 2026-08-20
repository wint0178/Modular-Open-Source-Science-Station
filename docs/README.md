# Modular and Open-Source Science Station (MOSSS)
> **Ensuring safe stones gather moss.**

---

The Modular and Open-Source Science Station (MOSSS) features a decentralized environmental monitoring network designed for rugged, remote terrain. Utilizing a localized LoRaWAN mesh alongside EcoWitt meteorological sensors, the system aggregates real-time microclimate data, barometric pressure, and physical movement tracking.

At the core of the MOSSS physical tracking infrastructure is the **Wake on Interrupt Landslide Detector (WOILD)** subsystem—an open-source hardware and telemetry framework specifically engineered to monitor slope stability and ground acceleration.

All data streams converge on a centrally located, local Home Assistant Core gateway, which orchestrates automated mobile alerts via custom vector deviation matrices and bridges the network into broader commercial smart integrations.

![MOSSS Topology](./images/MOSSS_Topology.png)

---

## 🛠️ Hardware Stack

| Layer | Device | Sensors / Specs |
| :--- | :--- | :--- |
| **Field Nodes** | Heltec WiFi LoRa 32 V3 | MPU6050 Accelerometer, DHT22, 1W Solar |
| **Weather** | EcoWitt WN90 Array | Piezo Rain, Anemometer, Temp/Humid/Pres |
| **Gateways** | SenseCAP M2 & EcoWitt GW3001 | LoRaWAN (TTN), WiFi |
| **Core Gateway**| Raspberry Pi 4 / 5 | 32GB mSD, Home Assistant Core, Nabu Casa |

---

## 🔄 How It Works (Data Pipeline)

1. **Telemetry Collection:** Solar-powered WOILD LoRa nodes monitor environmental metrics and structural movement on hillsides, remaining in deep sleep until periodic transmission or hardware motion interrupts trigger an uplink.
2. **Backhaul:** Data is pushed via LoRaWAN to the SenseCAP gateway (integrated with TTN), while weather data routes through the EcoWitt gateway. Remote ESP32 Wi-Fi nodes utilize encrypted Tailscale WireGuard tunnels.
3. **Processing & Action:** Home Assistant Core ingests all streams, evaluates slope movement against real-time rainfall data via custom matrix template equations, handles local valley logic, and pushes automated alerts to mobile clients.

## ⚡ How the Alert Logic Works

Once deployed, the software relies on a dual-trigger mechanism to eliminate false alerts from environmental noise (such as high wind vectors or passing animals):

1. **The Wake-on-Interrupt Event:** The physical landslide tracking node remains silent in deep sleep until an active geometric slope or acceleration threshold is crossed, waking up instantly to blast an alert payload (`stateMode = 1` or `2`) via LoRaWAN or ESPHome.
2. **The Vector Deviation Matrix Evaluation:** Home Assistant intercepts the payload using the `databroker` channel, runs it through internal matrix template equations, evaluates concurrent localized rain accumulation rate from your EcoWitt array, and instantly fires a high-priority mobile notification if both thresholds match critical slope-failure parameters.

---

## 🚀 Step-by-Step Deployment Roadmap

After purchasing all necessary hardware, assemble a MOSSS station from raw parts and field deploy it following these these sequential steps:

1. **[Hardware Assembly & Sensor Construction](./ch01-hardware-bom.md):** Order parts, build physical enclosures, wire your Heltec V3 board, and connect accelerometers and environmental sensors.
2. **[Set-up Home Assistant Server (Provision Central Gateway)](./ch02-home-assistant.md):** Install HAOS, create the `databroker` M2M user account, and configure 30-day recorder filters before bringing field nodes online.
3. **[Establish Remote Connections](./ch03-remote-access.md)** Using NabuCasa (recommended), Tailscale VPN, or DDNS with port forwarding, setup remote access to your HA instance.
3. **[Integrate Commercial Smart Equipment](ch03-home-assistant-integrations.md):** Unbox, power up, and integrate off-the-shelf meteorological, agricultural, or scientific hardware—such as the **EcoWitt GW3001** weather station gateway—directly into Home Assistant to establish real-time microlocal metrics (rainfall, wind vectors, barometric pressure, ...).
4. **[Add WiFi sensors with ESPHome (Optional)](./ch03-esphome.md):** Set up option-based Wi-Fi or Tailscale VPN nodes using ESPHome. 
5. **[Setup TTN and LoRa transmissions](./ch04-TTN-LoRa.md):**  Setup TTN LoRa communication. Navigate to https://nam1.cloud.thethings.network and create an account. If no public gateway is available, setup your TTN LoRa gateway. Create an application and link it to the HA TTN integration using the application id, API Key, and TTN server address.
6. **[Flash WOILD v1.1.6 Firmware & Calibrate](./ch05-woild-firmware.md):** Configure your LoRaWAN credentials (`DevEUI`, `AppEUI`, `AppKey`), assign unique node IDs (`NODE_ID`), set static baseline orientation angles, and flash the MCU via Arduino IDE or PlatformIO. Then, open `WOILD_v1.1.6_payload_formatter.js` in [`../software/Landslide-Detectors/`](../software/Landslide-Detectors/) and paste the decoder script into your TTN or ChirpStack uplink payload formatter console.
7. **[Field Installation & Alert Verification](./ch06-field-deployment.md):** Mount hardware nodes at your field station, run physical tilt/motion interrupt tests, and verify real-time data ingestion and vector deviation matrix alerting on your Home Assistant dashboard.

---

👉 **Let's get started! Proceed to [Chapter 1: Hardware and BoM](./ch01-hardware-bom.md)**