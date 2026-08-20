# ☁️ Chapter 3.2: Remote Access & Network Architecture

To ensure field technicians and operators can securely monitor landslide tracking grids from anywhere globally, the gateway utilizes a redundant, dual-path remote access design tailored to bypass complex network security limitations.

### Architecture Components

**1. Home Assistant Cloud / Nabu Casa (Primary Pipeline)**
* **Function:** Serves as the primary encrypted pipeline for zero-configuration remote monitoring.
* **Mechanism:** Provides a direct, authenticated web link (`https://[your-subdomain].ui.nabu.casa`) to the field station dashboard, safely bypassing local firewall modifications or open ports.

**2. Tailscale / WireGuard (Secondary / Advanced Mesh VPN)**
* **Function:** Provisions an independent, encrypted WireGuard mesh VPN network (`100.x.x.x` private range).
* **Primary Use Case:** Connects remote **ESPHome** sensors and external **MQTT I/O devices (e.g., ESP32-S3 boards)** deployed across separate cellular or satellite relays. Microcontrollers compiled with the `esphome-tailscale` component communicate directly with the gateway's static Tailscale IP without requiring public DNS records or port forwarding.

> ⚠️ **CRITICAL DEPLOYMENT WARNING**  
> Nabu Casa must be fully initialized and authenticated **before** activating Tailscale links on the host server. Activating Tailscale during initial Nabu Casa provisioning can alter host system routing matrices, preventing Home Assistant Cloud from establishing its remote tunnel. Keep Tailscale inactive during initial setup.

---

### Field & Off-Grid Deployment Steps

Select and configure one of the following remote endpoint options based on your field site requirements:

#### Option A: Home Assistant Cloud / Nabu Casa (Automated)
1. Navigate to **Settings** → **Home Assistant Cloud** in the web UI and log in to your account.
2. The Home Assistant Companion App automatically ingests the encrypted Nabu Casa endpoint URL.
3. The app dynamically routes traffic between the local IP and the remote cloud endpoint depending on connection state.

#### Option B: Tailscale / WireGuard VPN (Encrypted Mesh)
1. Navigate to **Settings** → **Add-ons** (or **Apps**) and install the **Tailscale** or **WireGuard** add-on.
2. Install the matching client application on the mobile or field device.
3. Authenticate the node to establish the encrypted mesh tunnel, enabling direct local HA IP access over cellular or satellite relays.

#### Option C: Reverse Proxy / Dynamic DNS (Custom Endpoint)
1. Establish a public domain entry using Cloudflare Tunnels, NGINX Reverse Proxy, or DuckDNS paired with SSL certificates.
2. Open the mobile companion app and navigate to **Settings** → **Companion App** → **[Server Name]**.
3. In the **External URL** field, input your public domain endpoint (e.g., `https://my-field-station.duckdns.org`).

---

### ⚠️ Risks, Trade-offs & Limitations Summary

| Option | Primary Risks | System Limitations & Trade-offs |
| :--- | :--- | :--- |
| **Option A: Nabu Casa** | Publicly reachable login page; subdomains can be enumerated via public DNS records if credentials or MFA are weak. | Requires a recurring monthly subscription ($6.50/mo); fully dependent on Nabu Casa infrastructure availability. |
| **Option B: Tailscale Mesh** | Central account control (if your Tailscale/SSO account is breached, all mesh nodes are accessible). | Requires the Tailscale client application to be installed and actively running on every mobile or field device; non-admin users cannot easily access via a bare browser link. |
| **Option C: DDNS & Port Forwarding** | High attack surface; directly exposes Home Assistant to public internet port scanners, credential stuffing, and unpatched zero-day exploits. | Dynamic public IPs can change before DDNS syncs; requires manual router/firewall configuration; local SSL setup often causes internal IP certificate warnings. |

---

👉 **Proceed to [Chapter 4.1: MQTT](./ch04-MQTT.md)**