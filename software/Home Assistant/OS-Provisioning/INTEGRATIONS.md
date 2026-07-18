# 🔌 MOSSS Gateway: Software Integrations & Add-ons Blueprint

This document details the software application layer, community integrations, and network translators deployed within the **Modular and Open-Source Science Station (MOSSS)** central gateway ecosystem. This configuration translates incoming environmental RF data into local vector matrices and safety alerts.

![Descriptive Alt Text For Accessibility](/images/HA-Dessrumbes.png)
---

## ☁️ Remote Access & Network Architecture (Optional Fail-Safes)

To ensure field technicians can monitor landslide tracking grids securely from anywhere globally, the gateway utilizes a flexible remote access design tailored to bypass complex network security limitations.

### 1. Home Assistant Cloud (Nabu Casa) — Primary
*   **Function:** Serves as our primary encrypted pipeline for secure, zero-configuration remote monitoring. It provides a direct, authenticated web link to the field station's dashboard while safely bypassing the need to modify inbound firewall rules or expose local ports to the open internet.

### 2. Tailscale — Optional / Advanced Tunneling (User's Discretion)
*   **Function:** Provisions an independent, encrypted WireGuard mesh VPN network. This component is **not absolutely necessary** for standard setups, but provides robust security advantages for multi-site field footprints.
*   **Primary Use Case:** Crucial for connecting remote **ESPHome** sensors and external **MQTT I/O devices (e.g., ESP32-S3 boards)** deployed at distant field sites across separate cellular or satellite relays. By compiling the `esphome-tailscale` component onto the microcontrollers, nodes join our private `100.x.x.x` mesh network to target the gateway directly at its static Tailscale IP without modifying public DNS records or exposing network port forwarding.
*   **⚠️ CRITICAL DEPLOYMENT WARNING:** If deployed, you must adhere to a strict sequence: Nabu Casa must be fully initialized and authenticated *before* Tailscale links are activated on the host server. Starting or keeping an active Tailscale connection during initial Nabu Casa provisioning can alter system routing matrices, preventing Home Assistant Cloud from launching its remote tunnel.

---

## 📦 Installed Add-ons (The Application Layer)
These applications run as isolated system containers managed by the Home Assistant Supervisor to provide local backend utilities:

*   **Mosquitto Broker:** Our high-performance, local MQTT message broker. It ingests data packets directly from our LoRaWAN network layout using the `databroker` M2M credentials and serves them to the automation engine.
*   **File Editor / Studio Code Server:** Provides a direct editor workspace to securely update configuration scripts and manage local asset logs inside the `3-Runtime-Configuration` directory.

---

## 📡 Telemetry & Network Translators (Custom Integrations)

These integrations handle data parsing from your distributed field tracking arrays:

### 1. MQTT & The Things Network (TTN)
*   **Function:** Bridges the long-range LoRaWAN telemetry network with the Home Assistant automation bus.
*   **Data Pipeline:** Physical WOILD landslide edge nodes blast packets via LoRaWAN $\rightarrow$ The Things Network processes the decentralized frames $\rightarrow$ Telemetry data is funneled locally via **MQTT** to update our tracking matrices instantly.

### 2. EcoWitt & Weather.com
*   **Function:** Captures hyper-local meteorological profiles directly from our on-site microclimate array via local network broadcasts, complemented by regional historical projections via the Weather.com API.
*   **Data Pipeline:** Streams real-time rain accumulation rates and storm factors directly into our threshold matrices to correlate rainfall infiltration against immediate ground slope shift events.

![Descriptive Alt Text For Accessibility](./images/HA-Clima.png)

---

## 🛠️ Power User Tools & Extended Frameworks

*   **HACS (Home Assistant Community Store) & Get HACS:** Deployed as our secondary package manager to unlock community-driven custom cards, integrations, and advanced backend tools not available in the core distribution line.
    > 🔐 **INSTALLATION PREREQUISITE:** To initialize the HACS environment on a fresh installation, you must have a valid **GitHub Account**. The setup process requires you to authenticate the local gateway using GitHub's secure OAuth device-pairing key protocol before the community repository manager can unlock.

---

## 📊 Dashboard UI & Environmental Visualization (HACS Frontend Layer)
To deliver a high-readability monitoring UI for emergency response teams and field technicians, our primary command dashboard leverages custom frontend assets installed via HACS:

### 1. Layout & Styling Engines
*   **UI eXtension (UIX) & card-mod:** Used to inject custom CSS styling variables across the frontend DOM. This enables advanced element customization required to dynamically alter card behaviors, backgrounds, and warning indicator colors.
*   **visionOS & iOS Liquid Glass Theme:** Deployed as our primary visual theme wrapper, creating an exceptionally clean, modern glassmorphic interface that optimizes visual scanning in low-light command environments.

### 2. Microclimate & Spatial Tracking Cards
*   **Weather Radar Card & Weather.com Integration:** Displays live, interactive radar precipitation scans directly alongside our sensor readouts to map approaching storm cells before they cross our physical monitoring area.
*   **Windy.com (Embedded Webpage Card Framework):** Bypasses the need for complex API keys or heavy code integrations. By dropping a standard Home Assistant **Webpage Card** onto the frontend layout, operators can embed live, interactive wind vector models and barometric tracking directly from Windy.com. This provides instant, resource-light visualization of real-time atmospheric patterns and incoming storm cells without putting extra processing strain on the Raspberry Pi's CPU.
*   **Clock Weather Card & Horizon Card:** Renders critical astronomical, dawn/dusk, and sun elevation metrics—essential data points when analyzing real-time solar harvesting performance on our battery-powered field arrays.
*   **Map Card:** Dynamically displays the precise geospatial coordinate markers ($X, Y$) of individual active WOILD nodes distributed across the hillsides, turning color-coded red if a vector deviation trigger trips.

---

## 🔔 Emergency Alerting & Safety Channels

*   **Mobile App Integration:** Links localized system notifications directly to the **Home Assistant Companion App** on field-technician smartphones. This setup bypasses standard cloud delivery delays to fire **high-priority critical alerts** with custom alert sounds immediately upon matrix breach.
*   **Google Translate Text-to-Speech (TTS):** Configured to broadcast voice alerts through local audio hardware connected to the gateway. If a slope matrix registers structural failure risks, the hub issues audible verbal emergency warnings locally alongside mobile push alerts.

---

## 🏛️ System Housekeeping Integrations (Native)
These integrations are natively provisioned by **Home Assistant OS (HAOS)** and handle foundational server parameters:
*   **Home Assistant Supervisor & Backup:** Manages local container health and automates daily system backups to safeguard historical sensor data.
*   **Raspberry Pi Hardware & Power Supply Checker:** Monitors system metrics, device thermals, and alerts if undervoltage issues threaten edge calculation stability.
*   **Local IP Address, Bluetooth, Meteorologisk institutt, Radio Browser, Shopping List, & Sun:** Tracks gateway network parameters, local weather projections, solar elevation vectors, and auxiliary system entities.
