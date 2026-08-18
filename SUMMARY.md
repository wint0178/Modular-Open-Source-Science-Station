# Table of Contents

* [Preface & Overview](README.md)
  * [System Topology & Architecture](README.md#-%EF%B8%8F-hardware-stack)
  * [Quick-Start Navigation Guide](README.md#-quick-start-navigation-guide)

* [Chapter 1: Hardware Engineering & BOM](hardware/README.md)
  * [WOILD Edge Node Components](hardware/README.md#1-woild-edge-nodes-per-node)
  * [Meteorological Subsystem](hardware/README.md#2-meteorological-subsystem-per-site)
  * [Central Gateway Hub](hardware/README.md#3-central-gateway-hub-per-grid)
  * [Circuit Interconnect & Power-Gating Specs](hardware/README.md#-critical-circuit-interconnect-requirements)

* [Chapter 2: Software Ecosystem & Data Pipeline](software/README.md)
  * [Directory Structure & Architecture](software/README.md#-directory-structure)
  * [Data Pipeline Mechanics](README.md#-how-it-works-data-pipeline)
  * [Alert Logic & Dual-Trigger Mechanism](software/README.md#-how-the-alert-logic-works)

* [Chapter 3: Edge Firmware (WOILD v1.1.6)](software/README.md#step-3-flash-the-woild-hardware-nodes)
  * [Arduino Setup & Firmware Flashing](software/README.md#step-3-flash-the-woild-hardware-nodes)
  * [The Things Network Payload Decoder](software/README.md#step-4-deploy-the-ttn--chirpstack-payload-decoder)

* [Chapter 4: Auxiliary & Mesh Communications](software/README.md#step-5-provision-your-remote-esp32esphome-devices-optional)
  * [ESPHome Wi-Fi Integration](software/README.md#step-5-provision-your-remote-esp32esphome-devices-optional)
  * [Tailscale Mesh Networking](software/README.md#step-5-provision-your-remote-esp32esphome-devices-optional)

* [Chapter 5: Central Gateway Orchestration](software/README.md#step-1-configure-the-home-assistant-gateway)
  * [Home Assistant OS Setup](software/README.md#step-1-configure-the-home-assistant-gateway)
  * [M2M Security & Data Brokers](software/README.md#step-1-configure-the-home-assistant-gateway)
  * [EcoWitt Meteorological Integration](software/README.md#step-2-map-inputs-to-your-meteorological-sensors)

* [Chapter 6: Field Deployment Roadmap](README.md#-step-by-step-deployment-roadmap)
  * [Sequential Deployment Steps](README.md#-step-by-step-deployment-roadmap)
  * [TTN & LoRaWAN Gateway Linking](README.md#-step-by-step-deployment-roadmap)
  * [Field Installation & Alert Verification](README.md#-step-by-step-deployment-roadmap)

* [Appendices](README.md#%EF%B8%8F-credits-and-disclaimers)
  * [Project Credits & Authorship](README.md#project-credits)
  * [AI & Media Disclosures](README.md#ai-transparency--media-disclosure)
  * [Academic Citation Guidelines](README.md#academic-citation-note)