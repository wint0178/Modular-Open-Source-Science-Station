# 🌿 Modular and Open-Source Science Station (MOSSS)

> **Ensuring safe stones gather moss.**

An open-source, decentralized environmental monitoring and slope-stability tracking network designed for remote, rugged terrain. Utilizing localized LoRaWAN mesh communication alongside EcoWitt meteorological sensors, the system aggregates real-time microclimate data, barometric pressure, and physical movement tracking.

At the core of the MOSSS physical tracking infrastructure is the **Wake on Interrupt Landslide Detector (WOILD)** subsystem—an open-source hardware and telemetry framework specifically engineered to monitor slope stability and ground acceleration.

All data streams converge on a centrally located, local Home Assistant Core gateway, which orchestrates automated mobile alerts via custom vector deviation matrices and bridges the network into broader commercial smart integrations.

![MOSSS Topology](/MOSSS_Topology.png)

---

## 📚 Official Project Book & Documentation

All technical guides, hardware schematics, firmware source code explanations, and step-by-step deployment instructions are hosted in our interactive **mdBook**:

👉 **[Read the full Documentation: Modular and Open-Source Science Station (MOSSS)](https://wint0178.github.io/Modular-Open-Source-Science-Station/)**

### 📑 Book Chapters
1. **[Chapter 1: Hardware & Bill of Materials](https://wint0178.github.io/Modular-Open-Source-Science-Station/ch01-hardware-bom.html)** – Component lists, enclosure assemblies, and pinouts.
2. **[Chapter 2: Home Assistant Setup](https://wint0178.github.io/Modular-Open-Source-Science-Station/ch02-home-assistant.html)** – Central gateway provisioning and data retention rules.
3. **[Chapter 3: Home Assistant Integrations](https://wint0178.github.io/Modular-Open-Source-Science-Station/ch03-home-assistant-integrations.html)** – HA Companion App, Native Integrations, Apps (Add-ons), HACS, and remote access options.
4. **[Chapter 4: TTN, LoRa, and MQTT Data Transmission](https://wint0178.github.io/Modular-Open-Source-Science-Station/ch04-TTN-LoRa.html)** – SenseCAP gateway setup, TTN application routing, and MQTT bridges.
5. **[Chapter 5: WOILD Edge Node Firmware](https://wint0178.github.io/Modular-Open-Source-Science-Station/ch05-woild-firmware.html)** – Wake-on-Interrupt Landslide Detector configuration and JavaScript payload decoders.
6. **[Chapter 6: Field Deployment](https://wint0178.github.io/Modular-Open-Source-Science-Station/ch06-field-deployment.html)** – Physical installation, tilt verification, and weather station calibration.
7. **[Chapter 7: Troubleshooting & Maintenance](https://wint0178.github.io/Modular-Open-Source-Science-Station/ch07-troubleshooting.html)**
8. **[Appendix]((https://wint0178.github.io/Modular-Open-Source-Science-Station/appendix.html))** - Credits, AI disclaimers, Copyright. 

---

## 🗺️ Quick-Start Navigation Guide

If you are browsing the repository directly on GitHub, use this directory map to quickly find files for your current build phase:

| What you want to do | Go to Directory | Description & Key Files |
| :--- | :--- | :--- |
| **Build & Wire Hardware** | [`/hardware`](./hardware/) | Schematics (`v3_circuit.png`), PCB Gerbers, and enclosure guidelines. |
| **Flash Firmware & Add Payload Decoder** | [`/software`](./software/) | Arduino sketch for **WOILD v1.1.6** nodes, TTN JS decoder, and ESPHome YAMLs. |
| **Configure Home Assistant & Alerts** | [`/software/Home-Assistant`](./software/Home-Assistant/) | HAOS deployment, `databroker` M2M user, template sensors, and recorder settings. |
| **Read Notebooks & Field Research** | [`/docs`](./docs/) | Science Station Notebook, field logs, and research documentation. |
| **View Diagrams & Photos** | [`/images`](./images/) | Wiring diagrams, circuit photos, and dashboard UI captures. |

---

## 🛠️ Hardware Stack Overview

| Layer | Component / Device | Function / Specs |
| :--- | :--- | :--- |
| **Field Nodes** | Heltec WiFi LoRa 32 V3 | MPU6050 Accelerometer, DHT22, 1W Solar |
| **Weather Array** | EcoWitt WN90 & GW3001 | Piezo Rain, Anemometer, Temp/Humid/Pres |
| **Gateways** | SenseCAP M2 & EcoWitt | LoRaWAN (TTN), Local Webhooks / Wi-Fi |
| **Core Gateway** | Raspberry Pi 4 / 5 | Local Home Assistant Core server, Nabu Casa |

---

## 🔄 How It Works (Data Pipeline)

1. **Telemetry Collection:** Solar-powered WOILD LoRa nodes monitor environmental metrics and structural movement on hillsides, remaining in deep sleep until periodic transmission or hardware motion interrupts trigger an uplink.
2. **Backhaul:** Data is pushed via LoRaWAN to the SenseCAP gateway (integrated with TTN), while weather data routes through the EcoWitt gateway. Remote ESP32 Wi-Fi nodes utilize encrypted Tailscale WireGuard tunnels.
3. **Processing & Action:** Home Assistant Core ingests all streams, evaluates slope movement against real-time rainfall data via custom matrix template equations, handles local valley logic, and pushes automated alerts to mobile clients.

---

## ⚖️ Credits and Disclaimers

### Project Credits
* **System Design & Architecture:** Developed by Brandon J. Winters, Ph.D., Murphy O'Malley, and Evan Streeter
* **Firmware & Core Logic:** Built on the WOILD v1.1.6 framework.

### AI Transparency & Media Disclosure
* **Documentation & Asset Assistance:** Portions of the configuration optimization, code documentation, and README organization were refined with the assistance of large language models.
* **Visual Imagery:** Conceptual diagrams, branding icons, or repository header graphics were generated using AI imaging tools for illustrative and repository-visual enhancement purposes.

### Academic Citation Note
If you are utilizing the WOILD framework, hardware PCB configurations, or the Home Assistant automation matrix in academic research, please cite the primary manuscript.

---

## 📄 License
This project is open-source under the [MIT License](LICENSE). Contributions, issues, and field feedback are welcome!