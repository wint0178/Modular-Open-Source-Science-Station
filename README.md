# Modular and Open-Source Science Station
The Modular and Open-Source Science Station (MOSSS) features a decentralized 
environmental monitoring network designed for rugged, remote terrain. Utilizing 
a localized LoRaWAN mesh alongside EcoWitt meteorological sensors, the system 
aggregates real-time microclimate data, barometric pressure, and physical 
movement tracking. 

At the core of the MOSSS physical tracking infrastructure is the Wake on 
Interrupt Landslide Detector (WOILD) subsystem—an open-source hardware and 
telemetry framework specifically engineered to monitor slope stability and 
ground acceleration. 

All data streams converge on a centrally located, local Home Assistant Core 
gateway, which orchestrates automated mobile alerts via custom vector 
deviation matrices and bridges the network into broader commercial smart 
integrations.
<img src="./ss-topology-image.png" />

## 🛠️ Hardware Stack

| Layer | Device | Sensors / Specs |
| :--- | :--- | :--- |
| **Field Nodes** | Heltec WiFi LoRa 32 V3 | MPU6050 Accelerometer, DHT22, 1W Solar |
| **Weather** | EcoWitt WN90 Array | Piezo Rain, Anemometer, Temp/Humid/Pres |
| **Gateways** | SenseCAP M2 & EcoWitt GW3001 | LoRaWAN (TTN), WiFi |
| **Core Gateway**| Raspberry Pi 4 | 32GB mSD, Home Assistant Core, Nabu Casa |

## 🔄 How It Works (Data Pipeline)
1. **Telemetry Collection:** Solar-powered LoRa nodes monitor environmental metrics and structural movement on the hillsides.
2. **Backhaul:** Data is pushed via LoRaWAN to the SenseCAP gateway (integrated with TTN), while weather data routes through the EcoWitt gateway.
3. **Processing & Action:** Home Assistant Core ingests all streams, handles local logic to control smart infrastructure in the valley, and pushes automated alerts to mobile clients.

## ⚖️ Credits and Disclaimers

### Project Credits
* **System Design & Architecture:** Developed by Brandon J. Winters, Ph.D.
* **Firmware & Core Logic:** Built on the WOILD v1.1.3 framework.

### AI Transparency & Media Disclosure
* **Documentation & Asset Assistance:** Portions of the configuration optimization, code documentation, and README organization were refined with the assistance of large language models (Gemini/ChatGPT).
* **Visual Imagery:** Conceptual diagrams, branding icons, or repository header graphics were generated using AI imaging tools with human-directed engineering prompts. These assets are intended purely for illustrative, conceptual, and repository-visual enhancement purposes.

### Academic Citation Note
If you are utilizing the WOILD framework, hardware PCB configurations, or the Home Assistant automation matrix in academic research, please cite the primary manuscript:
