# 🌦️ EcoWitt GW3001 Integration Guide

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

# 🔑 Obtaining EcoWitt API Credentials & Hardware Identifiers

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
