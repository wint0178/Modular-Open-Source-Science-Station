# ESPHome BME680 Sensor Deployment Guides

This folder contains three distinct ESPHome YAML configurations for deploying an **ESP32-S3** microcontroller paired with a **BME680** environmental sensor (Temperature, Pressure, Humidity, Gas Resistance). 

Choose the configuration that matches your network architecture and remote connectivity needs.

---

## 1. Architectural Comparison

| Feature | `esp32-wifi-bme680.yaml` | `esphome-mqtt-bme680.yaml` | `esp32-tailscale-bme680.yaml` |
| :--- | :--- | :--- | :--- |
| **Primary Protocol** | Native Home Assistant API | MQTT Broker (`1883` / `8883`) | Native HA API over Tailscale VPN |
| **Scope** | **Local Network Only** | **Local or Remote** | **Remote Multi-Site** |
| **Network Dependency** | Direct LAN connection | Access to central MQTT Broker | Tailscale Mesh Network |
| **Encryption** | Noise Protocol (API Key) | Optional TLS (Port `8883`) | WireGuard / Tailscale Mesh |
| **HA Auto-Discovery** | Yes (mDNS / Native) | Yes (MQTT Discovery) | Yes (via VPN Subnet) |
| **On-Board Diagnostics** | None | Status LED (GPIO48) | Status LED (GPIO48) + Identify Button |

---

## 2. Configuration Breakdown & Use Cases

### Option A: Standard Local Network Deployment (`esp32-wifi-bme680.yaml`)
* **Best For:** Simple indoor or local outdoor setups where the ESP32 and Home Assistant reside on the exact same Wi-Fi subnet.
* **How it works:** Uses Home Assistant’s native API over direct TCP sockets. Auto-discovered via mDNS.
* **Key Requirements:** Home Assistant must be on the same local network.

### Option B: Decoupled / External Broker (`esphome-mqtt-bme680.yaml`)
* **Best For:** Environments without direct HA access, multi-broker topologies, or cloud-hosted instances (e.g., AWS, DigitalOcean, or public MQTT relays).
* **How it works:** Pushes sensor updates directly to an MQTT broker. Home Assistant reads data via the MQTT integration using Home Assistant MQTT Discovery.
* **Key Requirements:** Requires a reachable MQTT broker IP/domain and valid credentials.

### Option C: Secure Remote Multi-Site (`esp32-tailscale-bme680.yaml`)
* **Best For:** Secure, off-site deployments (remote field stations, secondary properties, or cellular gateways) that need to communicate with Home Assistant across the internet.
* **How it works:** Integrates the `esphome-tailscale` component directly on the ESP32-S3. The board joins your Tailscale mesh network, creating an encrypted WireGuard tunnel back to your Home Assistant instance.
* **Key Features:** Includes an onboard RGB LED "Identify" button sequence to locate physical units in multi-node fleets.

---

## 3. How to Use & Deploy

### Step 1: Set Up `secrets.yaml`
To avoid hardcoding sensitive parameters in your public repository, create a `secrets.yaml` file in your ESPHome working directory containing the following:

```yaml
# Network Credentials
wifi_ssid: "YOUR_WIFI_SSID"
wifi_password: "YOUR_WIFI_PASSWORD"
wifi_ap_password: "YOUR_FALLBACK_AP_PASSWORD"

# Security & API
esphome_api_key: "YOUR_32_BYTE_BASE64_KEY="
esphome_ota_password: "YOUR_OTA_PASSWORD"

# MQTT Parameters (If using Option B or C)
mqtt_broker_ip: "100.X.X.X" # Or public domain
mqtt_password: "YOUR_MQTT_PASSWORD"

# Tailscale Auth Key (If using Option C)
tailscale_auth_key: "tskey-auth-YOUR_REUSABLE_TAILSCALE_KEY"
```
---

👉 **Proceed to:** [Sensor Hardware Assembly](/hardware/).