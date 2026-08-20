# 🤖 Chapter 4.2: ESPHome IDE

ESPHome is a system that lets you control your microcontroller boards (such as ESP32 and ESP8266) using simple, powerful YAML configuration files. In the MOSSS architecture, ESPHome allows field microcontrollers—like the ESP32-S3 paired with environmental sensors—to communicate back to Home Assistant seamlessly over local Wi-Fi, MQTT brokers, or encrypted remote mesh networks.

The Home Assistant ESPHome App gives users access to an integrated development environment (IDE) within the HA user interface directly and provides a mechanism for over-the-air (OTA) firmware updates.

![ESPHome Application](/docs/images/HA-ESPHome.png)

---

## 📐 Deployment Architectures

We provide three primary ESPHome deployment configurations based on an **ESP32-S3** microcontroller paired with a **BME680** environmental sensor (Temperature, Pressure, Humidity, Gas Resistance). Choose the architecture that aligns with your field network topology:

| Feature | Option A (`esphome-wifi-bme680.yaml`) | Option B (`esphome-mqtt-bme680.yaml`) | Option C (`esphome-tailscale.yaml`) |
| :--- | :--- | :--- | :--- |
| **Primary Protocol** | Native Home Assistant API | MQTT Broker (`1883` / `8883`) | Native HA API over Tailscale VPN |
| **Scope** | **Local Network Only** | **Local or Remote** | **Remote Multi-Site** |
| **Network Dependency** | Direct LAN connection | Access to central MQTT Broker | Tailscale Mesh Network |
| **Encryption** | Noise Protocol (API Key) | Optional TLS (Port `8883`) | WireGuard / Tailscale Mesh |
| **HA Auto-Discovery** | Yes (mDNS / Native) | Yes (MQTT Discovery) | Yes (via VPN Subnet) |
| **On-Board Diagnostics** | None | Status LED (GPIO48) | Status LED (GPIO48) + Identify Button |

---

## 🛠️ Configuration Breakdown & Use Cases

### Option A: Standard Local Network Deployment [`esphome-wifi-bme680.yaml`](../software/ESPHome/esphome-wifi-bme680.yaml)
* **Best For:** Simple indoor or local outdoor setups where the ESP32 and Home Assistant reside on the exact same Wi-Fi subnet.
* **Mechanism:** Uses Home Assistant’s native API over direct TCP sockets. Automatically discovered via mDNS.
* **Requirements:** Home Assistant must be on the same local network subnet.

### Option B: Decoupled / External Broker Deployment [`esphome-mqtt-bme680.yaml`](../software/ESPHome/esphome-mqtt-bme680.yaml)
* **Best For:** Environments without direct HA network access, multi-broker topologies, or cloud-hosted instances (e.g., AWS, DigitalOcean, or public MQTT relays).
* **Mechanism:** Pushes sensor updates directly to an MQTT broker. Home Assistant reads data via the native MQTT Integration using Home Assistant MQTT Discovery.
* **Requirements:** Requires a reachable MQTT broker IP/domain and valid authentication credentials.

### Option C: Secure Remote Multi-Site Deployment [`esphome-tailscale.yaml`](../software/ESPHome/esphome-tailscale.yaml)
* **Best For:** Secure, off-site deployments (remote field stations, secondary properties, or cellular gateways) that need to communicate with Home Assistant securely across the public internet.
* **Mechanism:** Compiles the `esphome-tailscale` component directly onto the ESP32-S3. On boot, the board joins your private Tailscale mesh network (`100.x.x.x` range), creating an encrypted WireGuard tunnel back to your Home Assistant gateway.
* **Diagnostics:** Includes an onboard RGB LED "Identify" button sequence on GPIO48 to quickly locate physical hardware units in multi-node field fleets.

---

![ESPHome IDE](/docs/images/HA-ESPHome2.png)

## ⚙️ Step-by-Step Hardware Provisioning & Setup

### Step 1: Set Up `secrets.yaml`
To keep sensitive credentials out of version control, create or update `secrets.yaml` inside your ESPHome working directory (`/config/esphome/`):

~~~yaml
# Network Credentials
wifi_ssid: "YOUR_WIFI_SSID"
wifi_password: "YOUR_WIFI_PASSWORD"
wifi_ap_password: "YOUR_FALLBACK_AP_PASSWORD"

# Security & API
esphome_api_key: "YOUR_32_BYTE_BASE64_KEY="
esphome_ota_password: "YOUR_OTA_PASSWORD"

# MQTT Parameters (If using Option B)
mqtt_broker_ip: "100.X.X.X" # Or public domain/IP
mqtt_password: "YOUR_MQTT_PASSWORD"

# Tailscale Auth Key (If using Option C)
tailscale_auth_key: "tskey-auth-YOUR_REUSABLE_TAILSCALE_KEY"
~~~

### Step 2: Provision & Flash Remote ESP32 Devices
1. Navigate to your ESPHome dashboard or workspace directory (`../software/ESPHome/`).
2. Open your targeted configuration file (e.g., `esphome-tailscale-bme680.yaml`) and update the `name` and `friendly_name` variables to match your deployment site. Ensure all secrets variables correctly reference your `secrets.yaml`.
3. Compile and flash your ESP32-S3 board over USB or OTA.
4. On initial boot, the board will attach to the local Wi-Fi, initialize the sensor pipeline, and automatically establish its communications link back to the central server.

---
## ⚠️ Essential ESPHome Operational Rules

### 1. Initial Flash vs. OTA (Over-the-Air) Updates
* **Initial Flash:** A brand-new, unprogrammed micro-controller **must** be connected via a physical USB cable to perform the first firmware install.
* **OTA Updates:** After the initial USB installation is complete, all subsequent updates can be compiled and sent wirelessly over Wi-Fi (Over-the-Air).

### 2. Wi-Fi Fallback AP (Captive Portal)
* **What Happens During Network Failure:** If a field board loses its primary Wi-Fi connection, ESPHome automatically spins up a local fallback Wi-Fi access point (e.g., `MOSSS-Field-AP`).
* **Field Recovery:** Rather than retrieving the physical node to re-flash it via USB, a technician can connect a smartphone directly to the board's fallback Wi-Fi AP to reconfigure credentials through a browser captive portal.
---

👉 **Proceed to [Chapter 4.3: TTN LoRa](./ch04-TTN-LoRa.md)**