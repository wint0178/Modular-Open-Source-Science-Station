# Modular and Open-Source Science Station (MOSSS)
> **Safe stones gather moss.**

---

The Modular and Open-Source Science Station (MOSSS) features a decentralized environmental monitoring network designed for rugged, remote terrain. Utilizing a localized LoRaWAN mesh alongside EcoWitt meteorological sensors, the system aggregates real-time microclimate data, barometric pressure, and physical movement tracking.

At the core of the MOSSS physical tracking infrastructure is the **Wake on Interrupt Landslide Detector (WOILD)** subsystem—an open-source hardware and telemetry framework specifically engineered to monitor slope stability and ground acceleration.

All data streams converge on a centrally located, local Home Assistant Core gateway, which orchestrates automated mobile alerts via custom vector deviation matrices and bridges the network into broader commercial smart integrations.

![MOSSS Topology](./MOSSS_Topology.png)

---

## 🛠️ Hardware Stack

| Layer | Device | Sensors / Specs |
| :--- | :--- | :--- |
| **Field Nodes** | Heltec WiFi LoRa 32 V3 | MPU6050 Accelerometer, DHT22, 1W Solar |
| **Weather** | EcoWitt WN90 Array | Piezo Rain, Anemometer, Temp/Humid/Pres |
| **Gateways** | SenseCAP M2 & EcoWitt GW3001 | LoRaWAN (TTN), WiFi |
| **Core Gateway**| Raspberry Pi 4 / 5 | 32GB mSD, Home Assistant Core, Nabu Casa |

---

## 🗺️ Quick-Start Navigation Guide

If you are new to the repository, use this directory map to quickly find files for your current build phase:

| What you want to do | Go to Directory | Description & Key Files |
| :--- | :--- | :--- |
| **Build & Wire Hardware** | [`/hardware`](./hardware/) | Schematics (`v3_circuit.png`), PCB Gerbers, and enclosure guidelines. |
| **Flash Firmware & Add Payload Decoder** | [`/software`](./software/) | Arduino sketch for **WOILD v1.1.6** nodes, TTN JS decoder, and ESPHome YAMLs. |
| **Configure Home Assistant & Alerts** | [`/software/Home Assistant`](./software/Home%20Assistant/) | HAOS deployment, `databroker` M2M user, template sensors, and recorder settings. |
| **Read Notebooks & Field Research** | [`/docs`](./docs/) | Science Station Notebook, field logs, and research documentation. |
| **View Diagrams & Photos** | [`/images`](./images/) | Wiring diagrams, circuit photos, and dashboard UI captures. |

---

## 🚀 Step-by-Step Deployment Roadmap

To take a MOSSS station from raw parts to an active field deployment, follow these sequential steps:

1. **Hardware Assembly:** Refer to [`/hardware/README.md`](./hardware/README.md) and `images/v3_circuit.png` to wire your Heltec V3 board, accelerometer, and sensors.
2. **Flash WOILD v1.1.6 Firmware:** Navigate to [`/software/Landslide Detectors`](./software/Landslide%20Detectors/), set your LoRaWAN credentials (`DevEUI`, `AppEUI`, `AppKey`), and flash the MCU via Arduino IDE or PlatformIO.
3. **Configure TTN Payload Formatter:** Copy `WOILD_v1.1.6_payload_formatter.js` into your TTN or ChirpStack uplink payload formatter console.
4. **Provision Gateway Logic:** Follow [`/software/README.md`](./software/README.md) to route incoming payloads into Home Assistant, set up the `databroker` account, and configure the vector deviation matrix alerting.

---

## 🔄 How It Works (Data Pipeline)

1. **Telemetry Collection:** Solar-powered WOILD LoRa nodes monitor environmental metrics and structural movement on hillsides, remaining in deep sleep until periodic transmission or hardware motion interrupts trigger an uplink.
2. **Backhaul:** Data is pushed via LoRaWAN to the SenseCAP gateway (integrated with TTN), while weather data routes through the EcoWitt gateway. Remote ESP32 Wi-Fi nodes utilize encrypted Tailscale WireGuard tunnels.
3. **Processing & Action:** Home Assistant Core ingests all streams, evaluates slope movement against real-time rainfall data via custom matrix template equations, handles local valley logic, and pushes automated alerts to mobile clients.

---

## ⚖️ Credits and Disclaimers

### Project Credits
* **System Design & Architecture:** Developed by Brandon J. Winters, Ph.D.
* **Firmware & Core Logic:** Built on the WOILD v1.1.6 framework.

### AI Transparency & Media Disclosure
* **Documentation & Asset Assistance:** Portions of the configuration optimization, code documentation, and README organization were refined with the assistance of large language models (Gemini/ChatGPT).
* **Visual Imagery:** Conceptual diagrams, branding icons, or repository header graphics were generated using AI imaging tools with human-directed engineering prompts. These assets are intended purely for illustrative, conceptual, and repository-visual enhancement purposes.

### Academic Citation Note
If you are utilizing the WOILD framework, hardware PCB configurations, or the Home Assistant automation matrix in academic research, please cite the primary manuscript: