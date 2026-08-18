# 📡 MOSSS Software & Configuration Deployment Guide

This directory contains the core software components, custom firmware, and automation logic that power the network topology of the **Modular and Open-Source Science Station (MOSSS)**. 

The software ecosystem is split into three modular pillars: **Long-Range LoRaWAN Nodes**, **Flexible Wi-Fi & Remote I/O Links**, and **Central Gateway Orchestration**.

---

## 📂 Directory Structure

*   **`../software/Home-Assistant/`**: Contains the central operating system guidelines, multi-tiered user privileges, custom template sensors, and data-logging optimizations required to process field inputs locally.
*   **`../software/Landslide-Detectors/`**: Contains the firmware sketches, hardware interrupt configurations, and JavaScript payload decoders for the primary, field-deployed **WOILD v1.1.6** (Wake-On-Interrupt Landslide Detector) LoRaWAN arrays.
*   **`../software/ESPHome/`**: Contains production-ready YAML firmware templates for ESP32 and ESP8266 microcontrollers. Supports local Wi-Fi telemetry ingestion, remote connectivity via DDNS and port forwarding, and specialized ESP32-S3 builds compiling Tailscale client binaries (esphome-tailscale.yaml) to tunnel encrypted telemetry across networks without firewall modifications.

---

## ⚡ How the Alert Logic Works

Once deployed, the software relies on a dual-trigger mechanism to eliminate false alerts from environmental noise (such as high wind vectors or passing animals):

1. **The Wake-on-Interrupt Event:** The physical landslide tracking node remains silent in deep sleep until an active geometric slope or acceleration threshold is crossed, waking up instantly to blast an alert payload (`stateMode = 1` or `2`) via LoRaWAN or ESPHome.
2. **The Vector Deviation Matrix Evaluation:** Home Assistant intercepts the payload using the `databroker` channel, runs it through internal matrix template equations, evaluates concurrent localized rain accumulation rate from your EcoWitt array, and instantly fires a high-priority mobile notification if both thresholds match critical slope-failure parameters.