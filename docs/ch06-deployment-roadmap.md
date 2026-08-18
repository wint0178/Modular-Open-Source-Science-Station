## 🚀 Step-by-Step Deployment Roadmap

After purchasing all necessary hardware, assemble a MOSSS station from raw parts and field deploy it following these these sequential steps:

1. **Set-up Home Assistant Server (Provision Central Gateway):** Follow [Chapter 5](./ch05-home-assistant.md) to install HAOS, create the `databroker` M2M user account, and configure 30-day recorder filters before bringing field nodes online.
2. **Integrate Commercial Smart Equipment:** Unbox, power up, and integrate off-the-shelf meteorological, agricultural, or scientific hardware—such as the **EcoWitt GW3001** weather station gateway—directly into Home Assistant to establish real-time microlocal metrics (rainfall, wind vectors, barometric pressure, ...).
3. **Add WiFi sensors with ESPHome (Optional):** Set up option-based Wi-Fi or Tailscale VPN nodes using [Chapter 4](./ch04-esphome-tailscale.md).
4. **Hardware Assembly & Sensor Construction:** Refer to [Chapter 1](./ch01-hardware-bom.md) and `images/v3_circuit.png` to build physical enclosures, wire your Heltec V3 board, and connect accelerometers and environmental sensors.
5. **Setup TTN and LoRa transmissions:** Refer to [Chapter 5](./ch05-home-assistant.md) to setup TTN LoRa communication. Navigate to https://nam1.cloud.thethings.network and create an account. If no public gateway is available, setup your TTN LoRa gateway. Create an application and link it to the HA TTN integration using the application id, API Key, and TTN server address.
6. **Flash WOILD v1.1.6 Firmware & Calibrate:** Navigate to [Chapter 3](./ch03-woild-firmware.md), configure your LoRaWAN credentials (`DevEUI`, `AppEUI`, `AppKey`), assign unique node IDs (`NODE_ID`), set static baseline orientation angles, and flash the MCU via Arduino IDE or PlatformIO. Then, open `WOILD_v1.1.6_payload_formatter.js` in [`/software/Landslide-Detectors/`](/software/Landslide-Detectors/) and paste the decoder script into your TTN or ChirpStack uplink payload formatter console.
7. **Field Installation & Alert Verification:** Mount hardware nodes at your field station, run physical tilt/motion interrupt tests, and verify real-time data ingestion and vector deviation matrix alerting on your Home Assistant dashboard.

---