WOILD HARDWARE PCB MANUFACTURING & ORDERING GUIDE (V1.1.3)
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
      <img src="/images/v3_circuit.png" alt="Circuit Schematic" width="320">
    </td>
    <td>
      <div style="font-weight: bold; font-size: 12px; text-transform: uppercase; font-family: sans-serif; margin-bottom: 4px;">2. Enclosures</div>
      <img src="/images/boxes.JPG" alt="Physical Deployment Enclosures" width="320">
    </td>
  </tr>
  <tr>
    <td>
      <div style="font-weight: bold; font-size: 12px; text-transform: uppercase; font-family: sans-serif; color: #52a3ff; margin-top: 10px; margin-bottom: 4px;">3. PCB Top Side</div>
      <img src="/images/v3_front.png" alt="PCB Front Layer Layout" width="320">
    </td>
    <td>
      <div style="font-weight: bold; font-size: 12px; text-transform: uppercase; font-family: sans-serif; color: #52a3ff; margin-top: 10px; margin-bottom: 4px;">4. PCB Bottom Side</div>
      <img src="/images/v3_back.png" alt="PCB Back Layer Layout" width="320">
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
