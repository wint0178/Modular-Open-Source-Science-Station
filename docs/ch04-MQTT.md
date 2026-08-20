# Chapter 4.1: MQTT

### 1. MQTT & The Things Network (TTN)
*   **Mosquitto Broker:** Our high-performance, local MQTT message broker. It ingests data packets directly from our LoRaWAN network layout using the `databroker` M2M credentials and serves them to the automation engine.

*   **Function:** Bridges the long-range LoRaWAN telemetry network with the Home Assistant automation bus.
*   **Data Pipeline:** Physical WOILD landslide edge nodes blast packets via LoRaWAN $\rightarrow$ The Things Network processes the decentralized frames $\rightarrow$ Telemetry data is funneled locally via **MQTT** to update our tracking matrices instantly.

---

👉 **Proceed to [Chapter 4.2: ESPHome](./ch04-esphome.md)**