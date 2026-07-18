# 🛠️ MOSSS Hardware Deployment & Bill of Materials (BOM)

This directory contains the physical infrastructure designs, schematic references, and hardware requirements for the **Modular and Open-Source Science Station (MOSSS)** framework. 

To deploy a standard operational tracking cell, you will need components for three distinct layers: the physical telemetry edge nodes (WOILD), the microclimate parsing node (EcoWitt), and the localized gateway hub.

---

## 📋 Comprehensive Bill of Materials (BOM)

### 1. WOILD Edge Nodes (Per Node)
These low-power field units are custom-fabricated to monitor earth movement and acceleration over remote slopes.

| Component | Description / Function | Key Specification | Est. Unit Cost (USD) |
|---|---|---|---|
| **MCU Development Board** | Core microprocessor processing telemetry and deep-sleep states. | Heltec WiFi LoRa 32 V3 (Must support external RTC GPIO interrupts) | $4.00 - $6.00 |
| **Inclinometer / Accelerometer** | Senses ground acceleration and shifts; outputs low-pulse hardware interrupt. | MPU6050 breakout board | $2.00 - $4.00 |
| **Custom 2-Layer PCB** | Integrates MCU, power regulation, and sensor lines (See gerber files). | FR-4 substrate, ENIG finish (highly recommended for high humidity) | $2.00 |
| **Power Storage** | High-capacity outdoor power delivery. | 3.7V 18650 Li-ion battery (2500mAh+) | $4.00 - $6.00 |
| **Solar Harvesting** | Charges internal cells under light canopy tracking conditions. | 5V 1W to 2W Monocrystalline solar panel | $3.00 - $5.00 |
| **Humidity Sensor** | Detects water in enclosure. | DHT22 breakout board | $5.00 - $10.00 |
| **Solar Charger** | Manages solar power and battery charging. | CN3065 solar charge board | $5.00 - $10.00 |
| **Enclosure** | Ruggedized environmental protection box. | IP67/IP68 weatherproof junction box with cable glands | $5.00 - $10.00 |

### 2. Meteorological Subsystem (Per Site)
Aggregates hyper-local microclimate profiles and storm cell attributes to correlate with ground data.

| Component | Description / Function | Key Specification | Est. Unit Cost (USD) |
|---|---|---|---|
| **EcoWitt Weather Station** | Solid-state array tracking rainfall rate, wind vector, and humidity. | Wittboy (GW2001) or WS90 7-in-1 Sensor Array | $130.00 - $150.00 |
| **EcoWitt Gateway** | Receives RF signals locally and passes customized data payloads to the hub. | GW1100, GW1200, or GW2000 Local Wi-Fi Gateway | $30.00 - $45.00 |

### 3. Central Gateway Hub (Per Grid)
The edge-computing collection facility that runs the vector evaluation code and processes emergency alerts locally.

| Component | Description / Function | Key Specification | Est. Unit Cost (USD) |
|---|---|---|---|
| **Edge Server Platform** | Dedicated machine executing the central operations stack. | Raspberry Pi 4/5 (4GB+), Home Assistant Green, or Mini PC | $35.00 - $80.00 |
| **Storage Infrastructure** | High-write endurance platform storage for database protection. | High-Endurance MicroSD card (64GB+) or M.2 NVMe SSD | $15.00 - $30.00 |
| **LoRaWAN Gateway** | Acts as the local network concentrator to intercept WOILD node signals. | RAKWireless RAK7246 / RAK7289 or similar multichannnel gateway | $70.00 - $120.00 |

---

## 🔌 Critical Circuit Interconnect Requirements

To successfully leverage the ultra-low-power **Wake on Interrupt** performance characteristics of the WOILD node ($<700\mu\text{A}$ current draw while asleep), verify your PCB layout complies with the following trace paths:

1. **The Interrupt Rail:** Connect the **`INT`** pin of your accelerometer module directly to an **RTC-capable GPIO pin** on the microprocessor board (e.g., GPIO 12, 14, or 15 on a standard ESP32). Non-RTC pins cannot capture states when the core processor clock is powered down.
2. **I2C Interface:** Establish direct lines from `SDA` and `SCL` pins with appropriate $4.7\text{k}\Omega$ pull-up resistors to maintain baseline data stability when checking telemetry frames.
3. **Power-Gating Transistors:** Implement a P-channel MOSFET network to cut off parasitic current leaking into the LoRa module during deep-sleep monitoring intervals.
