### [Configure the Home Assistant Gateway](../software/Home-Assistant/)
1. Navigate to `../software/Home-Assistant/` and review the installation guide to provision **Home Assistant OS (HAOS)** on a Raspberry Pi 4 or 5.
2. Create a dedicated Machine-to-Machine (M2M) profile called `databroker` (login: `databroker`) under **Settings > People** to handle incoming data streams securely.
3. Configure your local **Recorder** filter component using the provided template to restrict data-logging to a strict 30-day target, protecting your SD card from wear.

## 🏠 Stage 1: Home Assistant OS Bare-Metal Installation

This directory manages the initial physical provisioning and bare-metal OS installation for the central **Modular and Open-Source Science Station (MOSSS)** edge gateway.

---

## 📋 Prerequisites & Hardware Checklist

Before beginning, ensure your central hub hardware components match our verified specifications:

*   **Single Board Computer:** Raspberry Pi 4 or Raspberry Pi 5 (Minimum 4GB RAM recommended).
*   **Power Supply:** Official Raspberry Pi USB-C power supply (15W for Pi 4, 27W for Pi 5) to prevent undervoltage failures.
*   **Storage:** A 32GB high-endurance microSD card (UHS Speed Class 3 / V30 or better rated for continuous write cycles) or an external USB 3.0 SSD.
*   **Network:** An Ethernet cable connected directly to your local network switch/router for initial provisioning.

---

## 💿 Installation Step-by-Step

Because the gateway needs to compute vector matrices locally without internet dependence, we utilize the bare-metal **Home Assistant Operating System (HAOS)**.

> ⚠️ **CRITICAL ORDERING:** Adhere strictly to the sequence below. Misordering these setup steps will result in a failed headless boot or network mismatch.

---

### Step-by-Step Sequence

<ul style="list-style: none; padding-left: 0;">

  <!-- STEP 1 -->
  <li style="margin-bottom: 30px; border-bottom: 1px solid #30363d; padding-bottom: 20px;">
    <h3 style="margin-top: 0;">1. Download the Flashing Tool</h3>
    <p style="color: #8b949e; margin-top: -10px; font-size: 14px;"><em>Requires Mac, Windows, or Linux PC</em></p>
    Download and install the official <strong>Raspberry Pi Imager</strong> from <a href="https://www.raspberrypi.com/software/">raspberrypi.com/software</a>. Insert your storage media into your flashing computer.
  </li>

  <!-- STEP 2 -->
  <li style="margin-bottom: 30px; border-bottom: 1px solid #30363d; padding-bottom: 20px;">
    <h3 style="margin-top: 0;">2. Select the HAOS Image</h3>
    <p style="color: #8b949e; margin-top: -10px; font-size: 14px;"><em>Do not use default Pi OS</em></p>
    <ol>
      <li>Launch the Imager tool.</li>
      <li>Click <strong>Choose Device</strong> and select your model (e.g., Raspberry Pi 5).</li>
      <li>Click <strong>Choose OS</strong>, scroll down to select <strong>Other specific-purpose OS</strong>, click <strong>Home Assistant</strong>, and choose <strong>Home Assistant OS</strong>.</li>
    </ol>
  </li>

<!-- STEP 3 (Includes critical warning and image) -->
  <li style="margin-bottom: 30px; border-bottom: 1px solid #30363d; padding-bottom: 20px;">
    <h3 style="margin-top: 0;">3. Flash without OS Customization</h3>
    <p style="color: #8b949e; margin-top: -10px; font-size: 14px;"><em>Crucial Step</em></p>
    Select your target storage device and click <strong>Next</strong>.<br><br>
    
<!-- Styled Warning Callout Box -->
<div style="background-color: rgba(240, 185, 11, 0.1); border-left: 4px solid #f0b90b; padding: 15px; margin-bottom: 15px; border-radius: 0 6px 6px 0;">
<strong style="color: #f0b90b;">⚠️ CRITICAL:</strong> If the imager prompts you to apply OS customization settings (like setting up Wi-Fi or SSH), <strong>select NO</strong>. HAOS manages its own network initialization—applying custom configurations through the imager will corrupt the system container structure. Confirm and write the image.
</div>
    
<!-- Native HTML Image Element -->
<img src="../images/Pi-imager.png" alt="Raspberry Pi Imager Settings" style="max-width: 100%; height: auto; border-radius: 6px; margin-top: 10px;">
  </li>

  <!-- STEP 4 -->
  <li style="margin-bottom: 30px; border-bottom: 1px solid #30363d; padding-bottom: 20px;">
    <h3 style="margin-top: 0;">4. Headless Initial Boot</h3>
    <p style="color: #8b949e; margin-top: -10px; font-size: 14px;"><em>Takes 5-15 minutes</em></p>
    Insert the flashed storage into your Raspberry Pi. Connect an <strong>Ethernet cable</strong> to your router, then plug in the power supply. The Pi will boot headlessly; give it up to 15 minutes to automatically provision, unpack the environment, and fetch system dependencies.
  </li>

  <!-- STEP 5 -->
  <li style="margin-bottom: 30px; border-bottom: 1px solid #30363d; padding-bottom: 20px;">
    <h3 style="margin-top: 0;">5. Complete Onboarding UI</h3>
    <p style="color: #8b949e; margin-top: -10px; font-size: 14px;"><em>Web Browser Setup</em></p>
    On a computer connected to the same local network, open a browser window and navigate to:
    <code>http://homeassistant.local:8123</code>
    *(If the hostname fails to resolve, check your router's DHCP client list to find the Pi's IP address and navigate to <code>http://YOUR_PI_IP:8123</code>).* Follow the prompts to create your local admin account.
  </li>

</ul>

---

## 🧭 Next Step
Once you have landed on your fresh home assistant dashboard and created your main owner account, advance to user provisioning and system networking:

## 🔐 Stage 2: Operational Security (OpSec) & Network Provisioning

This directory defines the access permissions, secure data pipelines, and remote connectivity meshes required to protect the MOSSS gateway hub from corruption while allowing open scientific collaboration.

---

## 🛡️ Multi-Tiered User Access Tiers

Configure these explicit profiles under **Settings > People** on your newly installed dashboard:

### 1. Research Partner Profiles (`Admin`)
*   **Access Level:** Administrative configuration rights.
*   **Deployment:** System owner and shared strictly with active field engineers and collaborative research institution partners to tweak template filters or debug physical sensor links.

### 2. Public Observation Profile (`User / Non-Admin`)
*   **Access Level:** Read-Only view rights (Dashboard visualization access only).
*   **Deployment:** Provided to local community leaders, public donors, or visiting scientists. Completely blocks out system configuration menus, preventing accidental damage.

### 3. Machine-to-Machine (M2M) Data Pipeline (`databroker`)
*   **Access Level:** Non-admin, Data Authentication Account (Bypasses human UI interaction entirely).
*   **Deployment Name:** `databroker`
*   **Crucial Setup:** This profile handles incoming data streams from both your local network and remote field assets. Whether your nodes are feeding data locally via the Mosquitto Broker or connecting from remote field sectors using **ESPHome Tailscale** configurations, they use the `databroker` credentials to securely authenticate their data payloads.

> ⚠️ **SECURITY COMPLIANCE WARNING:** Never reuse the credentials for `databroker` on human user profiles. Isolating your automated data pipeline ensures that even if an external field node is physically tampered with, your core gateway administration remains entirely secure.

---

## ☁️ Remote Access & Network Resilience (The Dual-Path Pipeline)

To ensure operators can monitor landslide tracking grids securely from anywhere in the world, the gateway runs a completely redundant remote network design:

1. **Home Assistant Cloud (Nabu Casa) — Primary:** Serves as our primary encrypted pipeline for secure, zero-configuration remote monitoring, safely bypassing the need to modify inbound firewall rules.
2. **Tailscale — Secondary / Backup:** Provisions an independent, encrypted WireGuard mesh VPN network to handle direct peer-to-peer telemetry tunneling from remote microcontrollers. Suggested to setup, but keep turned off unless needed.

---

## 🧭 Next Step
Review the full applications setup documentation located inside this folder for specific step-by-step installation instructions for Tailscale, HACS, and cloud sync options:

## 🔌 MOSSS Gateway: Software Integrations & Add-ons Blueprint

This document details the software application layer, community integrations, and network translators deployed within the **Modular and Open-Source Science Station (MOSSS)** central gateway ecosystem. This configuration translates incoming environmental RF data into local vector matrices and safety alerts.

![Image Missing](../images/HA-Integrations.png)
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
*   
![Image Missing](../images/HA-Dessrumbes.png)
---

## 📡 Telemetry & Network Translators (Custom Integrations)

These integrations handle data parsing from your distributed field tracking arrays:

### 1. MQTT & The Things Network (TTN)
*   **Function:** Bridges the long-range LoRaWAN telemetry network with the Home Assistant automation bus.
*   **Data Pipeline:** Physical WOILD landslide edge nodes blast packets via LoRaWAN $\rightarrow$ The Things Network processes the decentralized frames $\rightarrow$ Telemetry data is funneled locally via **MQTT** to update our tracking matrices instantly.

### 2. EcoWitt & Weather.com
*   **Function:** Captures hyper-local meteorological profiles directly from our on-site microclimate array via local network broadcasts, complemented by regional historical projections via the Weather.com API.
*   **Data Pipeline:** Streams real-time rain accumulation rates and storm factors directly into our threshold matrices to correlate rainfall infiltration against immediate ground slope shift events.

![Descriptive Alt Text For Accessibility](../images/HA-Clima.png)

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

## ⚙️ Stage 3: Runtime Database Optimization & Configuration Guide

This directory houses the live production codebase running the MOSSS gateway. This includes your storage optimization matrices, mathematical template states, and active landslide threshold alert scripts.

---

## 💾 Database Optimization & MicroSD Storage Preservation

The gateway utilizes a **32GB high-endurance microSD card** to host the operating system and local database records. Because the `databroker` continuously ingests rapid telemetry updates, a default configuration will rapidly exhaust disk space or wear out the storage media through excessive write cycles.

### ⏱️ Data Ingestion Cadence & Write Mitigation

To drastically reduce write amplification on the 32GB storage media, the sensor array is tuned to discrete, physics-based reporting intervals rather than continuous streaming:

| Sensor Array | Firmware/Protocol | Normal Transmission Cadence | Engineering Justification |
| :--- | :--- | :--- | :--- |
| **EcoWitt Array** | Local Net Broadcast | **Every 5 Minutes** | Captures fast-moving meteorological fronts and acute rainfall accumulation without flooding the database. |
| **WOILD Nodes (v1.1.6)** | LoRaWAN / ESPHome | **Every 60 Minutes** | Establishes stable, ultra-low-power geological baselines. Provides early structural warning while keeping daily transaction writes minimal. |

Because transactions are structured on 5-minute and 60-minute boundaries rather than sub-second intervals, the local SQLite database engine (`home-assistant_v2.db`) easily remains compact and lightweight over your 30-day retention envelope. Ensure the `recorder:` filter block provided in `configuration.yaml` is applied to lock in this protection layer.

### 📡 End-to-End System Data Flow

```mermaid
graph LR
    %% Define Layout and Node Connections
    subgraph Edge Telemetry Layer
        NodeA[WOILD Field Node<br>Heltec V3 / MPU6050] -->|LoRaWAN Up-link| NodeB[LoRa Gateway<br>SenseCAP M2 / TTN]
        NodeC[Meteorology Array<br>EcoWitt WN90] -->|RF Broadcast| NodeD[EcoWitt Gateway<br>GW3001]
    end

    subgraph Central Processing Core
        NodeB -->|External MQTT Bus| NodeE[Mosquitto Broker<br>databroker M2M Channel]
        NodeD -->|Local or Remote Wi-Fi Push| NodeF[Home Assistant Core<br>Raspberry Pi 4 / 32GB mSD]
        
        NodeE -->|Raw Payload String| NodeG{2D Vector Deviation<br>Matrix Evaluation}
        NodeF -->|5-Min Rain Cadence| NodeG
    end

    subgraph Emergency Action Layer
        NodeG -->|Threshold Breach Trigger| NodeH[Automated Alerts<br>HA Companion App Push]
    end
```

---

## 🛠️ Step-by-Step Implementation

### Step 1: Access the Home Assistant Root Configuration Folder
To deploy or modify these files, access the root directory where your primary `configuration.yaml` file is hosted on your Raspberry Pi. This can be accomplished via:
* **The File Editor Add-on** via the Home Assistant sidebar (Highly Recommended)
* **The Studio Code Server Add-on** 
* **Samba share Add-on** utilizing a local network folder mapping

### Step 2: Apply Configuration Files

#### [A] configuration.yaml
1. Open your active local `configuration.yaml` file.
2. Append or merge your current settings with the updated layout provided in our production file.
3. Ensure that your core directory inclusion split lines look exactly like this, though there may be more such as rest, input-numbers, compensations, script, and secrets :
    ```yaml
    automation: !include automations.yaml
    template: !include templates.yaml
    ```

#### [B] templates.yaml
1. Open `templates.yaml` (create it in the root folder if it does not exist).
2. Overwrite the contents entirely with the sanitized `LD01` telemetry matrix code block provided.
3. Duplicate the needed code blocks for every sensor you intend to install (See Grid Expansion below).

#### [C] automations.yaml
1. Open `automations.yaml`.
2. Append or replace your active blocks with the updated `LD01` and `Heavy Rain` rules.
3. Duplicate the needed code blocks for every sensor you intend to install (See Grid Expansion below).
4. ⚠️ **CRITICAL VISUAL EDITOR WARNING:** Both automations utilize `mode: queued` and custom templates. Modifying these rules inside the Home Assistant Visual UI Editor may strip out or break the raw YAML syntax. **Always edit alerts directly in code.**

### Step 3: System Validation & Reloading
Before applying any configuration changes or restarting Home Assistant, you **MUST** validate the structural integrity of your YAML files:

1. In Home Assistant, navigate to: **Developer Tools > YAML**.
2. Click the **Check Configuration** button.
3. If any errors are flagged, double-check your spacing, indentation, and ensure no raw `<` or `>` characters are unquoted.
4. Once configuration validation passes successfully, scroll down to "YAML Configuration Reloading" on the same page and click: **"Reload All YAML Configuration"**.

---

## 🚀 Grid Expansion (Adding Hardware Nodes LD02 Through LD10)

When deployment of additional monitoring hardware is required in the field:

1. Open the target configuration file (`configuration.yaml`, `templates.yaml`, or `automations.yaml`).
2. Review the structural *Automation Scaling Note* commented at the top of the file.
3. Duplicate the relevant `LD01` code block.
4. Perform a localized **Find & Replace** inside **ONLY** your newly duplicated block:
   * Change all instances of `LD01` to your new index (e.g., `LD02`)
   * Change all instances of `ld01` to your new index (e.g., `ld02`)
   * Change all instances of `landslide_01` to your new index (e.g., `landslide_02`)
5. **Update Spatial Attributes:** Update the `baseline_x`/`baseline_y` coordinates in the Safety Matrix block and the physical `latitude`/`longitude` attributes in the Live Trackers block to match the specific, calibrated survey location of the new field hardware.
6. Re-run **Step 3 (Validation & Reloading)** to bring your new node online instantly.

---

### [Map Inputs to Your Meteorological Sensors](../software/EcoWitt/)
1. Ensure your local **EcoWitt** weather gateway is integrated with Home Assistant, calibrated to transmit wind, rain, and pressure data updates at a steady **5-minute ingestion cadence**.
    > 💡 *Note: For deployments were your Home Assistant server and GW3000 gateway are not on the same network, an access token can be used to provide remote data streaming and intermitant HA pulldown requests.*
2. Optional: Embed interactive vector models on your dashboard using a standard Webpage Card pointing to a customized Windy.com viewport string.

## 🌦️ EcoWitt GW3001 Integration Guide

This guide details two methods for integrating the **EcoWitt GW3001** gateway into Home Assistant: a low-latency, local network connection using Home Assistant's native integration, and a cloud-based RESTful API approach designed for remote off-grid deployments.

---

> [!CAUTION]
> **Operational Security (OpSec):** Never hardcode live API keys, application secrets, or MAC addresses directly in version-controlled `.yaml` files. Always store sensitive values in `secrets.yaml`.

---

## 🏠 Method 1: Local Network Integration (Native Webhook)

Use this method when the EcoWitt GW3001 gateway and your Home Assistant server reside on the same local network subnet.

### Step 1: Configure Custom Webhook in WS View Plus
1. Open the **WS View Plus** (or EcoWitt) mobile app on a device connected to the local Wi-Fi network.
2. Select your **GW3001** gateway device from the list.
3. Navigate to **Menu** $\rightarrow$ **Customized** (or **Weather Services** $\rightarrow$ **Customized**).
4. Enable the custom upload service and enter the following settings:
   * **Protocol Type:** `Ecowitt`
   * **Server IP / Hostname:** `<YOUR_HOME_ASSISTANT_LOCAL_IP>` (e.g., `192.168.1.100`)
   * **Path:** `/api/webhook/ecowitt`
   * **Port:** `8123` (or your local custom HA port)
   * **Upload Interval:** `300` seconds
5. Save and apply the configuration.

### Step 2: Enable Integration in Home Assistant
1. In Home Assistant, go to **Settings** $\rightarrow$ **Devices & Services**.
2. Click **Add Integration** in the bottom right corner.
3. Search for **Ecowitt** and select it.
4. Confirm the prompt to complete setup. Home Assistant will begin auto-discovering sensor channels as the GW3001 sends webhook payloads.

---

## ☁️ Method 2: Remote Deployment (RESTful Cloud API)

Use this method when the GW3001 is deployed at a remote, off-grid, or field station lacking a direct local network link back to Home Assistant.

## 🔑 Obtaining EcoWitt API Credentials & Hardware Identifiers

This guide outlines how to generate your **Application Key** and **API Key** from the EcoWitt Cloud portal, as well as how to locate your GW3001 hardware **MAC Address** for REST integration.

---

## 1. Generate Application Key & API Key

Both keys are generated inside your user account settings on the official EcoWitt platform:

1. Log into your account at **[ecowitt.net](https://www.ecowitt.net)**.
2. Click your profile icon/avatar in the top-right corner and select **User Center** (or **Profile**).
3. Navigate to the **API Management** tab in the sidebar menu.
4. **Application Key:** Click **Create Application Key** to generate a client identifier string.
5. **API Key:** Click **Create API Key** to generate your personal data authorization token.
6. Copy both generated strings and paste them securely into your deployment records or `secrets.yaml`.

---

## 2. Locate Your GW3001 MAC Address

The MAC address acts as the unique device ID for API query parameters. You can retrieve it using either method below:

### Method A: Via the WS View Plus App
1. Connect your phone to the same local Wi-Fi network as your GW3001.
2. Open the **WS View Plus** app and select **Device List**.
3. Locate your GW3001 gateway. The 12-character MAC address (e.g., `X1:X2:X3:X4:X5:X6`) will be displayed next to the device name.

### Method B: Via the EcoWitt Web Portal
1. Log into **[ecowitt.net](https://www.ecowitt.net)**.
2. Select your weather station dashboard.
3. Open **Device Settings** (gear icon) to view the gateway details, including the registered MAC address.

## 3. Add rest.yaml to Home Assistant
1. Download the rest.yaml file.
2. In Home Assistant, navigate to File Editor and create a new file in the same directory as your configuration.yaml file, named "rest.yaml"
3. Copy the full contents of the downloaded rest.yaml file into your newly created rest file in Home Assistant. Save the file.
4. Verify the yaml by navigating to Seetings->Developer Tools->YAML, Click "Check Configuration" and verify that it will not prevent HA from starting. If successful, click Restart and then select Quick Reload from the available options.
    > 💡 *Note: your configuration yaml must inlude the line "rest !include rest.ymal" or HA will not deploy these changes even after reloading.*
