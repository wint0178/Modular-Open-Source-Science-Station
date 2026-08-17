# 🔐 Stage 2: Operational Security (OpSec) & Network Provisioning

This directory defines the access permissions, secure data pipelines, and remote connectivity meshes required to protect the MOSSS gateway hub from corruption while allowing open scientific collaboration.

---

## 🛡️ Multi-Tiered User Access Tiers

Configure these explicit profiles under **Settings > People** on your newly installed dashboard:

### 1. Primary System Administrator (`Owner`)
*   **Access Level:** Full root privileges.
*   **Deployment:** Restricted entirely to the project founder. Used for structural YAML modification, managing network cryptographic keys, and container orchestration.

### 2. Research Partner Profiles (`Admin`)
*   **Access Level:** Administrative configuration rights.
*   **Deployment:** Shared strictly with active field engineers and collaborative research institution partners to tweak template filters or debug physical sensor links.

### 3. Public Observation Profile (`User / Non-Admin`)
*   **Access Level:** Read-Only view rights (Dashboard visualization access only).
*   **Deployment:** Provided to local community leaders, public donors, or visiting scientists. Completely blocks out system configuration menus, preventing accidental damage.

### 4. Machine-to-Machine (M2M) Data Pipeline (`databroker`)
*   **Access Level:** Data Authentication Account (Bypasses human UI interaction entirely).
*   **Deployment Name:** `databroker`
*   **Crucial Setup:** This profile handles incoming data streams from both your local network and remote field assets. Whether your nodes are feeding data locally via the Mosquitto Broker or connecting from remote field sectors using **ESPHome Tailscale** configurations, they use the `databroker` credentials to securely authenticate their data payloads.

> ⚠️ **SECURITY COMPLIANCE WARNING:** Never reuse the credentials for `databroker` on human user profiles. Isolating your automated data pipeline ensures that even if an external field node is physically tampered with, your core gateway administration remains entirely secure.

---

## ☁️ Remote Access & Network Resilience (The Dual-Path Pipeline)

To ensure operators can monitor landslide tracking grids securely from anywhere in the world, the gateway runs a completely redundant remote network design:

1. **Home Assistant Cloud (Nabu Casa) — Primary:** Serves as our primary encrypted pipeline for secure, zero-configuration remote monitoring, safely bypassing the need to modify inbound firewall rules.
2. **Tailscale — Secondary / Backup:** Provisions an independent, encrypted WireGuard mesh VPN network to handle direct peer-to-peer telemetry tunneling from remote microcontrollers.

---

## 🧭 Next Step
Review the full applications setup documentation located inside this folder for specific step-by-step installation instructions for Tailscale, HACS, and cloud sync options:

👉 **Read Application Guide:** [INTEGRATIONS.md](./INTEGRATIONS.md)  
👉 **Proceed to Stage 3:** [Go to Runtime-Configuration](../Runtime-Configuration/)
