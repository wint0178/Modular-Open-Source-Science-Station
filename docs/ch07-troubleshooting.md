# 🛠️ Chapter 7: Field Maintenance, Diagnostics & Troubleshooting

Operating low-power edge hardware in isolated, high-risk alpine environments requires systematic diagnostic flows and routine preventive maintenance. This chapter provides field technicians with detailed fault isolation trees, battery health metrics, over-the-air (OTA) recovery operations, and scheduled field service protocols for the WOILD edge monitoring fleet.

---

## 🔍 System Diagnostic & Fault Matrix

Use this quick-reference table to identify, isolate, and resolve operational failures in field-deployed nodes.

| Symptom / Failure Mode | Probable Root Cause | Diagnostic Indicator | Corrective Action |
| :--- | :--- | :--- | :--- |
| **Node Completely Silent** *(No Uplinks)* | Battery depleted below minimum MCU operating threshold ($<2.7\text{V}$). | Solar charge controller failure or physical battery disconnect. | Measure battery terminal voltage. Charge manually via external USB power bank or replace the cell. |
| **Node Transmits, But No TTN Live Data** | Network session keys out of sync; wrong frequency plan or subband configured. | Serial output displays persistent `EV_JOIN_FAILED` or `LMIC_SESSION_MISMATCH`. | Re-check `DevEUI`, `AppKey`, and `NwkKey` in TTN. Re-flash NVS storage or trigger a clean join cycle. |
| **Spurious Alert Mode Triggers** *(False Alarm)* | Acceleration threshold set too sensitive; enclosure mechanical instability; high wind on mounting pole. | Consecutive `stateMode: 1` uplinks without physical ground displacement. | Issue OTA downlink to increase `cfg_motion` threshold. Tighten enclosure mounting brackets and ground stakes. |
| **Persistent Tripwire Error** *(Mode 2 Stuck)* | Breakaway cable snapped, corroded connector, or loose wire terminal block. | Node locks in `stateMode: 2` continuous high-frequency alert. | Inspect external physical continuity wire. Clean oxidized terminals or issue OTA command to disable tripwire interlock (`use_tripwire: false`). |
| **Incorrect Angle / Tilt Drift** | Physical zero-point baseline drifted post-settling; loose sensor PCB inside enclosure. | Sensor telemetry shows constant non-zero angular offsets at rest. | Re-run mechanical baseline calibration routine or hardcode zero-offset adjustments in Home Assistant template parameters. |
| **Missing Precipitation / Weather Data** | Ecowitt GW3001 gateway offset error, low sensor battery, or blocked funnel. | Home Assistant rainfall entities remain flatlined at $0.0\text{ mm}$ during active storms. | Clear debris from rain collector funnel. Re-check rain calibration gain settings in the WS View Plus application. |

---

## 🔋 Battery Health & Low-Power Auto-Recovery

WOILD nodes run on a managed power budget to ensure multi-year survival without direct maintenance. Understanding power states prevents unnecessary field dispatches:

* **Normal Operation ($>3.6\text{V}$):** All features active. Radio runs full transmit power, accelerometers run active sampling, and downlinks process cleanly.
* **Low Voltage Warning ($3.0\text{V} - 3.5\text{V}$):** Node operates normally but flags a low-battery state bit in telemetry. Scheduled heartbeats remain at nominal intervals.
* **Hibernation / Auto-Recovery Mode ($<3.0\text{V}$):** The firmware enforces a continuous **20-hour deep-sleep hibernation cycle** with radio modules disabled. This prevents battery damage and brownout loops, giving small solar panels time to recharge the cell above $3.3\text{V}$ before resuming LoRaWAN operations.

> 💡 **Field Tip:** If a node goes offline following extended overcast weather, wait for a full sunny day before traveling to the site. The node will automatically recover and rejoin the network once the solar array restores battery voltage above $3.3\text{V}$.

---

## 🔄 Remote Command & Control (OTA Troubleshooting)

For nodes running **WOILD v1.1.5 or v1.1.6**, configuration settings can be adjusted remotely via TTN 2-byte downlink payloads on **FPort 1**. This eliminates the need to ascend high-risk slopes for firmware adjustments.

### Common Downlink Command Formats

| Target Adjustment | Downlink Hex (FPort 1) | Command Function |
| :--- | :---: | :--- |
| **Increase Motion Threshold** | `0x08 0x00` | Sets wake-on-motion threshold higher to suppress wind noise and false alarms. |
| **Disable Tripwire Interlock** | `0x00 0x40` | Toggles hardware tripwire monitoring off remotely (useful for broken loop recovery). |
| **Adjust Baseline Sleep Interval** | `0x00 0x0C` | Adjusts `MINIMUM_DELAY_LOOKUP_SEC` to reduce heartbeat frequency during long static periods. |

---

## 📋 Scheduled Field Service Checklist

Perform these routine physical checks twice yearly (spring post-thaw and autumn pre-freeze):

1. **Enclosure Integrity:** Check rubber lid gaskets for drying or cracking. Replace the internal **10g silica gel desiccant pack**.
2. **Solar Panel Maintenance:** Wipe down solar glass to remove pollen, dust, pine needles, or bird droppings. Verify mounting bracket tightness and cable gland seals.
3. **Antenna Check:** Confirm the antenna remains in a **strict vertical orientation**. Inspect SMA connector waterproofing tape for cracking or UV degradation.
4. **Mechanical Coupling:** Confirm ground stakes or anchor bolts have not loosened due to frost heave or soil erosion.
5. **Physical Tripwire Inspection:** Check continuity cable for abrasion, corrosion, or vegetation entanglement. Ensure breakaway force remains within expected tolerance.

---

👉 **Proceed to [Appendices](./appendix.md)**