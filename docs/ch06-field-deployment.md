# 🧗 Chapter 6: Field Deployment & Weather Station Integration

Deploying WOILD edge nodes requires transitioning from clean bench testing to harsh, unconditioned alpine and slope environments. Proper mechanical anchoring, weatherproofing, RF alignment, and auxiliary environmental integration (such as local weather stations) are critical to ensure years of uninterrupted monitoring and prevent false alarms.

---

## 📡 System-Wide Antenna Polarization & RF Propagation

Radio Frequency (RF) performance on steep, forested slopes depends heavily on signal polarization and clearing obstacles in the local propagation zone.

| RF Parameter | Requirement / Specification | Mitigation / Notes |
| :--- | :--- | :--- |
| **System Antenna Alignment** | **Strict Vertical Polarization** across ALL nodes, repeaters, and gateways. | Mismatched polarization (e.g., vertical node to horizontal gateway) causes up to **$20\text{ dB}$ signal loss**, severely reducing range in dense foliage. |
| **Ground Clearance** | Mount antennas at least $1.5–2\text{ m}$ above the soil surface. | Keeps the primary Fresnel zone clear of ground reflection loss and dense underbrush. |
| **Drip Loop & Connectors** | Form a downward loop on all coaxial pigtails; wrap connectors in self-amalgamating tape. | Prevents water ingress into SMA/N-Type interfaces during heavy rain. |

> ⚠️ **CRITICAL REQUIREMENT:** All LoRaWAN antennas across the entire system—including field nodes, auxiliary weather stations, and central gateways—MUST be installed with the exact same physical orientation (vertically pointing straight up). Never mount a node antenna horizontally unless the receiving gateway antenna is also mounted horizontally.

---

## 🌧️ Auxiliary Weather Monitoring: Ecowitt GW3001 Calibration

Rainfall intensity and cumulative soil saturation are the primary triggers for slope instability. Integrating an **Ecowitt GW3001** (or Wittboy array) alongside WOILD nodes provides localized precipitation tracking to correlate with physical motion events.

### Rain Gauge Calibration Procedure
The Ecowitt GW3001 piezo or tipping-bucket sensor allows manual calibration to ensure accurate local rainfall rates and total accumulation metrics:

1. Connect to the GW3001 gateway web console or WS View Plus app.
2. Navigate to **Sensor Settings $\rightarrow$ Calibration $\rightarrow$ Rain Gain**.
3. Measure actual accumulation using a standard high-precision reference rain gauge during local rain events.
4. Apply the linear adjustment formula to refine the gain factor:
   $$\text{New Gain} = \text{Current Gain} \times \left( \frac{\text{Actual Rain Measured}}{\text{GW3001 Reported Rain}} \right)$$
5. Save settings to ensure all upstream Home Assistant or TTN rainfall entities reflect calibrated, ground-truth precipitation totals.

---

## 🛠️ Physical Mounting & Mechanical Ground Coupling

To capture structural land motion accurately without measuring enclosure wobble, nodes must be rigidly coupled to the slope's substrate.

### Anchoring Options by Terrain
* **Bedrock / Hard Rock:** Drill using a masonry bit ($10\text{ mm}$), insert stainless steel expansion anchor bolts, and torque the base plate down directly.
* **Soil / Loose Debris:** Drive $1\text{ m}$ galvanised steel ground stakes into the earth. Secure the node enclosure near the top of the stake using heavy-duty U-bolts.
* **Structural Poles / Trees:** Use wide, weather-resistant stainless steel hose clamps or tree straps. *Note: Tree mounting should be avoided for primary accelerometers due to wind-induced trunk swaying.*

---

## ⚡ Enclosure Weatherproofing & Power Systems

| Component | Standard Specification | Installation Checklist |
| :--- | :--- | :--- |
| **Enclosure Rating** | **IP67 / IP68** UV-stabilized polycarbonate | Check for clean rubber gasket seating prior to screwing down the lid. |
| **Cable Glands** | PG7 / PG9 strain-relief glands | Tighten around all external sensor, tripwire, and solar cables. Point glands **downward**. |
| **Internal Moisture Control** | 10g silica gel desiccant pack | Place inside enclosure prior to final seal to prevent internal condensation on optics/sensors. |
| **Solar Panel Alignment** | Azimuth facing True South (Northern Hemisphere) at a $45^\circ–60^\circ$ tilt angle | Maximizes winter solar output and prevents snow accumulation on the panel face. |

---

## 🚀 Step-by-Step Field Commissioning Procedure

Follow this operational flow once on-site:

1. **RF Site Survey:** Before permanently drilling or anchoring, power on the node and execute a manual baseline transmit. Verify that the TTN console records an RSSI better than $-115\text{ dBm}$ and an SNR higher than $-5\text{ dB}$.
2. **Mount Hardware:** Securely anchor the enclosure to the ground stake or rock bolt.
3. **Set Zero-Reference Baseline:** Level the accelerometer relative to the resting slope. Power the node on to execute its initial calibration routine (writing zero-offsets to NVS).
4. **Arm Physical Tripwire:** Rig the continuity loop across the geological rift or fault line. Ensure sufficient tension without triggering a false breakaway.
5. **Verify Telemetry:** Confirm that Home Assistant or TTN reflects the correct initial `stateMode` (Mode 0: Timed Baseline) and healthy battery voltage.

---

👉 **Proceed to [Chapter 7: Maintenance & Troubleshooting](./ch07-troubleshooting.md)**