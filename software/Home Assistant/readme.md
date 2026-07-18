# 🏠 MOSSS Gateway Hub: Home Assistant Orchestration Layer

This directory manages the central **Modular and Open-Source Science Station (MOSSS)** edge gateway. The gateway runs on bare-metal **Home Assistant Operating System (HAOS)** to compute vector matrices locally, completely independent of upstream cloud networks.

To make deployment structured, repeatable, and secure, the installation and code assets are isolated into a linear, three-stage pipeline. Execute these stages in chronological order:

---

## 🧭 Deployment Pipeline Matrix

### 🛠️ [Stage 1: HAOS-Setup](./HAOS-Setup/)
*   **Purpose:** Physical hardware verification and core operating system flashing.
*   **Key Operations:** Verifying Pi 4/5 power constraints, provisioning the 32GB high-endurance storage media using the Raspberry Pi Imager, executing a headless first-boot, and completing onboarding.

### 🔐 [Stage 2: OS-Provisioning](./OS-Provisioning/)
*   **Purpose:** Network configurations, community software extensions, and operational security (OpSec).
*   **Key Operations:** Mapping the four-tier user hierarchy (including the isolated M2M `databroker` channel), initializing Nabu Casa and Tailscale redundant network pathways, and authenticating HACS.

### ⚙️ [Stage 3: Runtime-Configuration](./Runtime-Configuration/)
*   **Purpose:** Live production software code, sensor tracking arrays, and alert logic.
*   **Key Operations:** Deploying optimized `recorder:` database configurations for the 30-day wear-mitigation window, implementing the 2D Vector Deviation Matrix template formulas, and binding EcoWitt 5-minute rain cadences to critical landslide alerts.

---

## 🏃 Getting Started
To begin provisioning your station gateway hub, move directly to the first folder:

👉 **Begin Stage 1:** [Go to HAOS-Setup](./HAOS-Setup/)
