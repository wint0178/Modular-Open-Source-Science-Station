# Modular and Open-Source Science Station (MOSSS)
> **Ensuring safe stones gather moss.**

---

The Modular and Open-Source Science Station (MOSSS) features a decentralized environmental monitoring network designed for rugged, remote terrain. Utilizing a localized LoRaWAN mesh alongside EcoWitt meteorological sensors, the system aggregates real-time microclimate data, barometric pressure, and physical movement tracking.

At the core of the MOSSS physical tracking infrastructure is the **Wake on Interrupt Landslide Detector (WOILD)** subsystem—an open-source hardware and telemetry framework specifically engineered to monitor slope stability and ground acceleration.

All data streams converge on a centrally located, local Home Assistant Core gateway, which orchestrates automated mobile alerts via custom vector deviation matrices and bridges the network into broader commercial smart integrations.

![MOSSS Topology](../images/MOSSS_Topology.png/)

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

---