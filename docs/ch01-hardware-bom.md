# 🛠️ MOSSS Hardware Deployment & Bill of Materials (BOM)

This directory contains the physical infrastructure designs, schematic references, and hardware requirements for the **Modular and Open-Source Science Station (MOSSS)** framework. 

To deploy a standard operational tracking cell, you will need components for three distinct layers: the physical telemetry edge nodes (WOILD), the microclimate parsing node (EcoWitt), and the localized gateway hub.

---

## 📋 Comprehensive Bill of Materials (BOM)

### 1. WOILD Edge Nodes (Per Node)
These low-power field units are custom-fabricated to monitor earth movement and acceleration over remote slopes. Total cost per node is about $70.00.

| Component | Quantity | Description / Function | Key Specification | Est. Unit Cost (USD) |
|---|---|---|---|---|
| [**MCU Development Board**](https://www.amazon.com/V3-ESP32-Development-Dual-core-Meshtastic/dp/B0DRSJV1ZL?crid=339BN9HW3NYG8&dib=eyJ2IjoiMSJ9.NJhDb1YD9My6v5XZYl4t5LEEFrmlwTxkBpRLiGceVMmXafsKHkM_sTy8QGKrnOZESJAYEEiK4o582T3GDGimqNXu7zz60jJ9dIinAnh6_VlGVfwL6Yrreoizo80bIx4gv3SToTX-Fe-1kF1R9LmGWH_uTA6BNgwPJno2M4NUN9GIAH5eFlaXYOQuSvqkCofL3JoByMP1wDCG2mGpZWIqsPv-OVvJa1zbUePZWhUyhRw.ebcemUN5VpJFbroS4GYuXc16LJ_mMszrR2cux-BpU9k&dib_tag=se&keywords=heltec%2Bwifi%2Blora%2B32%2Bv3&qid=1787006249&sprefix=Heltec%2Bwifi%2Bl%2Caps%2C191&sr=8-1&th=1) | 1 | Core microprocessor processing telemetry and deep-sleep states. | Heltec WiFi LoRa 32 V3 (Must support external RTC GPIO interrupts) | $20.00 |
| [**Inclinometer / Accelerometer**](https://www.amazon.com/HiLetgo-MPU-6050-Accelerometer-Gyroscope-Converter/dp/B00LP25V1A?crid=1B4TKT1ZEW8FZ&dib=eyJ2IjoiMSJ9.JZYA1_lUIkS_I9N1KkjNeEq_6ypNudVsXzPlBIp3HBtQovEJHJW6e9Cx7tgdCmOEYTO8AIAa8blerj-qXGu5GivnygRUW1LQEaxgdjzwXNmgX1OKvkfS9JmtIys40rD3FjcZUUsGxk7bFniNJ478ZoCaM8Jh32VfT1_izM9IXm9upuJjQfgdOTTJwnWKgnOEQITaQxC_yXSLkILNcnESLF_WEDL0ve4lUz_gB4yeQtU.3Mfbofpit5V1MnR24IyQ9oc18oFHFI_wXoArSGOVyvM&dib_tag=se&keywords=mpu6050&qid=1787006224&sprefix=mpu6050%2Caps%2C201&sr=8-3&th=1) | 1 | Senses ground acceleration and shifts; outputs low-pulse hardware interrupt. | MPU6050 breakout board | $4.00 |
| [**Custom 2-Layer PCB**](https://oshpark.com/) | 1 | Integrates MCU, power regulation, and sensor lines (See gerber files). | FR-4 substrate, ENIG finish (highly recommended for high humidity) | $15.00 |
| [**Power Storage**](https://www.amazon.com/Rechargeable-Battery-PH2-0-Speakers-Packs/dp/B0F1D5HVJ8?crid=1Y4JGCHOE0TKO&dib=eyJ2IjoiMSJ9.jmuW3orDbmi0WRfCrgpjjk_k7cmkf7qP5T1LwGvyDKTu1k1pCGRv2blxfXfuo__sMrOzarD0EmVu6oCjftd42gT9Azn8qfakl5cG_QxdWItsr0W4IZqvDaNHMh86hq7ewe09sF8YD-HI7pvtociIZ1kwyDCb4E7gWPrT6u2u75OhkZ1nAoUZKUbrSP433sZ55E9asiwppUf3qBZMlX2vK2Yp3j01827HEZQSBgYHv1g.PPZ0JABs6mSJBmigudTRf9IhG0-qq_i9EMxoxFWa9Bw&dib_tag=se&keywords=lithium+batteries+18650&qid=1787006188&sprefix=lithium+batteries+18650%2Caps%2C217&sr=8-3) | 1 | High-capacity outdoor power delivery. | 3.7V 18650 Li-ion battery (2500mAh+), with 2 pin, 2.54 mm pitch JST | $10.00 |
| [**Solar Harvesting**](https://www.amazon.com/Panels-Electric-Materials-Photovoltaic-System/dp/B0B8YHP7NR?crid=1VP2AP08YMJC9&dib=eyJ2IjoiMSJ9.ON2ReISPuUizhYDLRySQaivR92Z56dylIyf4clbmsXBANbfS8JYMu9XijJExyr7VqgJrBuCLA-xMMGq1ZSIfgqVGU91_ysVVMTGueTxKxGZHwdP6p5e3cLQMMYxmqy3TqY7F-UXYQyAjYzuvhLoQoWqaj4J-vEhh-ty9KpwUVjgyzTdbU2osx_bJAHZUqwDGM9yN1wx-Bw_WBctj-hjUqCe_rwGGSYXHF6Pu4mytzNc.M1kkyvHzDDJFYttliuY4MdLknCSCqxvGZAc7l4lxqkQ&dib_tag=se&keywords=5v+solar+panel&qid=1787006132&sprefix=5v+solar%2Caps%2C186&sr=8-3) | 1 | Charges internal cells under light canopy tracking conditions. | 5V 1W to 2W Monocrystalline solar panel | $3.00 |
| [**Humidity Sensor**](https://www.amazon.com/SATUY-DHT22-AM2302-Temperature-Electronic/dp/B0FCLX5GTZ?crid=71NL2QF0M3AK&dib=eyJ2IjoiMSJ9.ZZOLcObu4INMAGv48yJpqtouz_sslly3qMESowS7foJbjz9KrWzdInijHaSF6AB2mUsSkiCwNavL1uCIrF5CmRLDgTzPA0PlP-2B2F87bCQo_oLofP4CoOtPf_tUuPspVYAltuWY9IpDYeV5ydYfl_GPwde83VoJTj0s6fzOMfMA6pmDpzUSHNQ614PAI0oIXrl08zz-SZx5S4sornm287eI4GBRA8C3Mw1NMNnU9iM.xv6I9eFF5G9BPUq9tJKUKY8z_80m6L3RVdtkBoOvxhw&dib_tag=se&keywords=DHT22&qid=1787006094&sprefix=dht2%2Caps%2C195&sr=8-4&th=1) | 1 | Detects water in enclosure. | DHT22 breakout board | $2.00 |
| [**Solar Charger**](https://www.amazon.com/ACEIRMC-Charger-Lithium-Application-Charging/dp/B0DXZ5CWXQ?crid=XG290KS1GQAS&dib=eyJ2IjoiMSJ9.ZYCW0SCMkXffFQ2E7s1QQWywMWI2JzbJdCOTmziZfa6qKUjLeX6GI-iHAOQkcmkvXsAQ69yixMBPJzHz6znt9SxVx2NrqC5i_8HonE1M5SvNaf5ZCpg7r5-tDe3BsW7c5C0NmaVylRQYjOvc-ioAW73iGukRvY5tQKWaFt5qv6Y52UxYNAeSQSY_UQKpa8BXpRbmBSY6C4Wi55b0OfdmJIfg4Rzn8VG5vtfAQ3CJpzQ.yM_niQ6iuVtBSlZGu9LoJjB_C3uKOkQrinK8WR636iU&dib_tag=se&keywords=CN3065&qid=1787005426&sprefix=cn3065%2Caps%2C183&sr=8-1&th=1) | 1 | Manages solar power and battery charging. | CN3065 solar charge board | $2.00 |
| [**Enclosure**](https://www.amazon.com/Zulkit-Dustproof-Waterproof-Electrical-150x100x70mm/dp/B08KY7VK8W?crid=ZBY62RPVKM8V&dib=eyJ2IjoiMSJ9.fPk1B951PedA7HzTHwKy1AVvN_yz2zYm5ix61EiOqKdZ1gT4hZsE3lqOwaYtMnHqGB-IcDtBaja5e3Hjwi7FvYdUMuuew4DKbEZDcnAdityW8SNSnPtwyca-HEn3UheYMpNARVX0rJ5bCb0J43mzQesrmlIWeLHpRr7Eak2ChRdndGDl2eJOdze-l3-Lo-hJ3BNU-z0y16Qfv5w5cFN04y_Znw4h55CAIMaUEuAnj-2aFK09085BDuUAGtDBRGNsPOFowOz5ZgCAomnKslLs73WguE4vHqFcJx9dS-vfWsg.wxylpwVzigIzz1QTlrTXZoYOFbkBYdqKeyNR1qIdTd8&dib_tag=se&keywords=weather%2Bproof%2Bproject%2Bbox&qid=1787006461&sprefix=weather%2Bproof%2Bprject%2Caps%2C210&sr=8-7&th=1) | 1 | Ruggedized environmental protection box. | IP67/IP68 weatherproof junction box with cable glands | $5.00 - $10.00 |
| [**4.7 kOhm resistor**](https://www.amazon.com/EDGELEC-Resistor-Tolerance-Resistance-Optional/dp/B07HDFHPP3?crid=39OREMR6K56Y1&dib=eyJ2IjoiMSJ9.edd2pFVhHikqXpztWdQtGtDwpTH0BGScqY4L1ad6n52--SGaljJmEu7QXAIKvut1-wna-pp2BYMzbozMBx2Z87PCTyR6p4In5xel9Vg4CMAKAhvRPINHRYF9i1VvAU6OGrJnc1duage4dnPN1iZ5ympWgToTYIBAXsi3chiGFn_9GObHj1rAkR6pjXnKDQCBSWYtFCFM5hC9Wk5sSkJ9TJgnasL630ZkUIjDR7bCHAY.WnSVYU3rWhqa4dyXLfypAtD9uVJaRf3YDekzxPyuEYM&dib_tag=se&keywords=4.7%2Bkohm%2Bresistor&qid=1787006429&sprefix=4.7%2Caps%2C223&sr=8-6&th=1) | 2 | Used to pull up SDA and SCL pins for I2C | Must solder to through holes of PCB | $0.10 |
| [**Female Header Pins**](https://www.amazon.com/Ferwooh-Single-Straight-Connector-Spacing/dp/B0CZ6X313F?crid=3L8G0KER3SFF1&dib=eyJ2IjoiMSJ9.my1z_XJlQFrNgZNH_tsevyGhxeroAV_GrkD1v3mq-l9dyyje6eiinpb0p_JQzLmhXdIePUepf0lC8ycCltD-Ybp25BiqOSz6OpdOA4sehbD3QIq5peeGfLpEsG1gStQ9mTIRdXB_0KEnmCQnf08Pc0uMwHj7DNciKfTcAxVD5hEUOQFOpmMffCzO66eBDddxwUQEccVPXAEXfTLQxQrU6Yy8GRb0MxB1DtpMYnL-BPQ.bDSGFc_iuabnWINR_RO7qYJnKs-FYnhYyr2ZiOFen-I&dib_tag=se&keywords=female+header+pins&qid=1787005507&sprefix=female+hea%2Caps%2C202&sr=8-4) | 3 | Soldered to PCB to accept boards | 40 pin, 2.54 mm pitch | $0.75 |
| [**Screw Terminal Block**](https://www.amazon.com/Screw-Terminal-Block-Circuit-Connectors/dp/B0FNQX9MMT?crid=L74EXI3ADYGX&dib=eyJ2IjoiMSJ9.QEweruKTRzxZydbykf9Db6AT5yVFkqIA9c7NnGsjIiPnPDkH2MjGUbh0tBMRXZ4gB0RgmkpHCpPMbYdBaLRGAYcXIKBoiJmE1bDzMHA_LyB6drJFPCBgZDccLs-wUNMROosOlKAqbk5r3UbhvQdvNA2OL_MpWeYnXxYYm7m0ayBLH9qtkvmhg1IeuEOZbtY9b8m_GLVnna8FJ9N1FCDCyvpfoSKAeUOqaeCApEsRhBk.H8cjzpXM-J1EdbWwba6Af3TfGFI0JsY6Lbck_Jdarv0&dib_tag=se&keywords=screw+terminal&qid=1787006038&sprefix=screw+terminal%2Caps%2C259&sr=8-3) | 2 | Solder to PCB for solar and tripwire | 2 pin, 5 mm | $0.25 |
| [**JST Battery Connector**](https://www.amazon.com/Letool%C2%AE30-1-25mm-Female-Connector-Cables/dp/B013JRWCBU?crid=2U4VSS5VKCL6E&dib=eyJ2IjoiMSJ9.nmGCDJaX0FLDAuihcI3ef8Rk0WGTrwwYICDUYRWNCze9xJRFltDaH7-k1QA2imxBM7iJfV5Tgp9_FQa4OEb_oZ2MaGoN6jle1UJEKnU3fcVi9KRVFWZLZl6trQvZNk2BCZ5r-DlCGFWP47qKIAPZkmw_naZP9DZ35nKCHtHRnqA7FPqOX1mFn1cj0t-RL7FS7AfkG2bgNwGZRMRmHGK5FSGxtGwau_qvCEP-a-te7ag.4fa_cj-chPjZVDUZa2G4lxMpiDG8cwlEN5znyQ5m47o&dib_tag=se&keywords=jst+1.25+connector+kit&qid=1787006403&sprefix=jst+1.25%2Caps%2C221&sr=8-4) | 1 | Solder to CN5065 for battery connection | 2 pin, 1.25 mm pitch | $0.30 |
| [**22 AWG Wire**](https://www.amazon.com/Conductor-Electrical-Parallel-0-3mm%C2%B2%EF%BC%8C-Projects/dp/B0FH4YYJ3Y?crid=3QT0M60D67B4E&dib=eyJ2IjoiMSJ9.0is0xfeGIMj9TEgcksIBU9jsAL1q5Vx57pYKE14YckDleDBYIFI7YmWp75AmAnFpmhNzAiCzbE_lv2J9mnvaLvtJfnF-jJM8G21p2zr5qy0fYMAmTc8X8YaO5-getnl5uHYEra-iEg-brF4Vqh_3PJ0BE1F3n9XibU1BwurH9rvBKOMhWAL8F1ROHz2ICm2P5638ldoBP-kkmqtvCsTL03jxnpkQZKON0Vmyf70aEaBMSzhuacPHyIxjcvZY05p47eYJDBDm8HpxgFnxmMnFOKQwgYK_tfQFUfSRetDRSkU.p7654FxWbtkrRaYfAfKxP1rk2Yk3CtPmk9iSUuFXY8Y&dib_tag=se&keywords=22AWG&qid=1787005778&sprefix=22awg%2Caps%2C191&sr=8-9&th=1) | 1 | Solder to solar pannel | 2 Conductor, tinned, 10 meters | $10.00 |
| [**Tripwire**](https://www.amazon.com/dp/B003NN1XZM?th=1) | 1 | Disconnected audio port Will serve as tripwire | 1/8" audio TRS extender, 6 ft | $2.00 |


### 2. Meteorological Subsystem (Per Site)
Aggregates hyper-local microclimate profiles and storm cell attributes to correlate with ground data.

| Component | Description / Function | Key Specification | Est. Unit Cost (USD) |
|---|---|---|---|
| [**EcoWitt Weather Station**](https://www.amazon.com/ECOWITT-Ethernet-Humidity-Anemometer-Barometric/dp/B0DTPXKP8B?dib=eyJ2IjoiMSJ9.vmuq2wixRRtQ4HUwB4Tx1Cv_TuTXVxVoF8i1jnqL0hY6v-vdyNxGuueYBpybWMOQ7pM067amLJ9BFM5YXjrDcGb1jDSqgnFqU5l4anfboObyk_chcJk4R5FNT79abqa9jXNDUn0L6iitYGxAjOXJV8jEhw3KsAfgs-0m_hhKNmmcz0kyPy2I6bAZypTCu1t3_RYHbeSlg9fzPx3w6Qz-YuWK6RH7a0A1Kqw7eDLt2V40IfwUQ_ru4wMD6h6wkgLngRp28WIEKBUQFUupRkcYWamxl0MjnKqwsMa2gJEbXF0.90WJBZWs4VZ1ySQaDd7E71DnfBFPnfJ7x14SSzmTBJE&dib_tag=se&keywords=Ecowitt%2Bgw3001&qid=1787006353&sr=8-1&th=1) | Solid-state array and WiFi Gateway | GW3000 with WS90 7-in-1 Sensor Array | $200.00 |

### 3. Central Gateway Hub (Per Grid)
The edge-computing collection facility that runs the vector evaluation code and processes emergency alerts locally.

| Component | Description / Function | Key Specification | Est. Unit Cost (USD) |
|---|---|---|---|
| [**Edge Server Platform**](https://www.amazon.com/CanaKit-Raspberry-4GB-Starter-Kit/dp/B07V5JTMV9?crid=3RE7EDWUJJFS4&dib=eyJ2IjoiMSJ9.6RZammJY5JsyJpwezt2mnyqVgX0u7giYKyl3toVQa_qfXyYKEmozfs484V3nYWBadviMM-Fihl_1q04siCLseotSgPo7PCskGUcgw2kfHFgG3pcoxlilaZ1Gx43ArYVky8dleAZ8FKyzR6FazPpzjVF2K_Uidfhg8TQcJR8P6Dtg8MoMitGZb_tf2CleyYEozVCWUwwPhnCloAsni5mKy6rhD_tojl-sMCCjk602gqk.gLGW8RGSe0cYae2lPj4jXlbktZ_VHEJQwYcgpWmGtEQ&dib_tag=se&keywords=raspberry%2Bpi%2B4&qid=1787006277&sprefix=Raspberry%2Caps%2C185&sr=8-3&th=1) | Dedicated machine executing the central operations stack. | Raspberry Pi 4 (4GB+)| $160.00 |
| [**LoRaWAN Gateway**](https://www.amazon.com/dp/B0CG98XDLX?ref=nb_sb_ss_w_as-reorder_k0_1_11&amp=&crid=11M1J45DYKKXW&amp=&sprefix=sensecap+m2) | Acts as the local network concentrator to intercept WOILD node signals. | SenseCAP M2 indoor LoRa gateway or similar multichannnel gateway | $70.00 - $120.00 |

---

## 🔌 Critical Circuit Interconnect Requirements

To successfully leverage the ultra-low-power **Wake on Interrupt** performance characteristics of the WOILD node ($<700\mu\text{A}$ current draw while asleep), verify your PCB layout complies with the following trace paths:

1. **The Interrupt Rail:** Connect the **`INT`** pin of your accelerometer module directly to an **RTC-capable GPIO pin** on the microprocessor board (e.g., GPIO 12, 14, or 15 on a standard ESP32). Non-RTC pins cannot capture states when the core processor clock is powered down.
2. **I2C Interface:** Establish direct lines from `SDA` and `SCL` pins with appropriate $4.7\text{k}\Omega$ pull-up resistors to maintain baseline data stability when checking telemetry frames.
3. **Power-Gating Transistors:** Implement a P-channel MOSFET network to cut off parasitic current leaking into the LoRa module during deep-sleep monitoring intervals.

WOILD HARDWARE PCB MANUFACTURING & ORDERING GUIDE (V1.1.6)
==============================================================================

This directory contains the production-ready Gerber files exported from KiCad 
for the Multi-Node Landslide Detection hardware. You can use the included 
ZIP archive to order custom printed circuit boards from any standard quick-turn 
PCB fabrication house.

------------------------------------------------------------------------------
RECOMMENDED FABRICATION HOUSES
------------------------------------------------------------------------------
The exported files use standard RS-274X/X2 formats compatible with all major 
global manufacturers. Popular high-quality options include:
  * JLCPCB (jlcpcb.com)
  * PCBWay (pcbway.com)
  * OSH Park (oshpark.com) - Perfect for high-precision, US-made prototypes

## 🛠️ WOILD Node v4 Hardware Design

The hardware subsystem is engineered to bridge abstract telemetry logic with resilient environmental deployment. The panel matrix below tracks our complete progression from the baseline circuit traces to our physical field installation array.

<table>
  <tr>
    <td>
      <div style="font-weight: bold; font-size: 12px; text-transform: uppercase; font-family: sans-serif; margin-bottom: 4px;">1. Circuit Logic</div>
      <img src="../images/v3_circuit.png" alt="Circuit Schematic" width="320">
    </td>
    <td>
      <div style="font-weight: bold; font-size: 12px; text-transform: uppercase; font-family: sans-serif; margin-bottom: 4px;">2. Enclosures</div>
      <img src="../images/boxes.JPG" alt="Physical Deployment Enclosures" width="320">
    </td>
  </tr>
  <tr>
    <td>
      <div style="font-weight: bold; font-size: 12px; text-transform: uppercase; font-family: sans-serif; color: #52a3ff; margin-top: 10px; margin-bottom: 4px;">3. PCB Top Side</div>
      <img src="../images/v3_front.png" alt="PCB Front Layer Layout" width="320">
    </td>
    <td>
      <div style="font-weight: bold; font-size: 12px; text-transform: uppercase; font-family: sans-serif; color: #52a3ff; margin-top: 10px; margin-bottom: 4px;">4. PCB Bottom Side</div>
      <img src="../images/v3_back.png" alt="PCB Back Layer Layout" width="320">
    </td>
  </tr>
</table>

------------------------------------------------------------------------------
STEP-BY-STEP ORDERING INSTRUCTIONS
------------------------------------------------------------------------------

  1. Locate the Archive:
     * Find the bundled Gerber zip file in this directory (typically named 
       something like 'woild_v1.1.3_gerbers.zip'). Do NOT unzip it. 

  2. Upload to the Fabricator:
     * Navigate to your chosen manufacturer's website and click on their 
       "PCB Instant Quote" or "Order Now" tool.
     * Drag and drop the untouched .zip file directly into their online Gerber 
       viewer upload box.

  3. Verify the Upload:
     * Most modern fabricators will automatically parse the zip archive and 
       display a live visual preview of the top and bottom layers of the board.
     * Ensure the board dimensions are automatically and correctly detected 
     * by the online system.

------------------------------------------------------------------------------
RECOMMENDED FABRICATION SPECIFICATIONS (PRODUCTION SETTINGS)
------------------------------------------------------------------------------
When completing the configuration form on the manufacturer's website, use the 
following baseline settings to ensure physical reliability in outdoor terrain:

  * Material Type: FR-4 (Standard)
  * Layer Count: 2 Layers (Double-sided)
  * PCB Thickness: 1.6 mm (Standard thickness for rugged field enclosures)
  * Surface Finish: HASL with lead (Cost-effective) OR ENIG (Electroless Nickel 
    Immersion Gold). 
    --> NOTE: ENIG is strongly recommended if you are deploying nodes in highly 
        humid, tropical environments, as it offers superior oxidation resistance.
  * Copper Weight: 1 oz (Standard)
  * Solder Mask Color: Your choice (Green, Blue, Black, etc. Does not affect performance)
  * Silkscreen Color: White (Or contrasting color for component label readability)

------------------------------------------------------------------------------
COMPONENT SOURCING (BOM & CPL)
------------------------------------------------------------------------------
  * Component Placement: The board layout utilizes clear silkscreen labeling 
    identifying resistor, capacitor, microcontroller, and sensor pinouts.
  * Assembly: This board is optimized for hand-soldering. If you intend to use 
    an automated turn-key assembly service (PCBA), you will need to provide the 
    manufacturer with the Bill of Materials (BOM) and Component Placement List 
    (CPL / Centroid file) exported separately from the main design repository.
==============================================================================
