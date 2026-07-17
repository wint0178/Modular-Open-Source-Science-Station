==============================================================================
LANDSLIDE DETECTION INTEGRATION SYSTEM DEPLOYMENT GUIDE (WOILD_V1.1.3)
==============================================================================

This directory contains four primary files managing your landslide grid:
  1. configuration.yaml - Core framework & derivative velocity tracking
  2. templates.yaml     - Sensor mathematical matrices & state logic
  3. automations.yaml   - Direct push alerts configured for 'your_phone_name'
  4. readme.txt         - This instruction document

------------------------------------------------------------------------------
STEP 1: ACCESS THE HOME ASSISTANT ROOT CONFIGURATION FOLDER
------------------------------------------------------------------------------
To deploy or modify these files, access the root directory where your primary 
configuration.yaml file is hosted. This can be accomplished via:
  * The Studio Code Server Add-on (Highly Recommended)
  * The File Editor Add-on via the Home Assistant sidebar
  * A local network mapping utilizing the Samba share Add-on

------------------------------------------------------------------------------
STEP 2: APPLY CONFIGURATION FILES
------------------------------------------------------------------------------

[A] CONFIGURATION.YAML
  1. Open your active 'configuration.yaml' file.
  2. Append or merge your current settings with the updated layout provided.
  3. Ensure that the inclusion lines look exactly like this:
        automation: !include automations.yaml
        template: !include templates.yaml
        sensor: !include configuration.yaml (or embedded directly below)

[B] TEMPLATES.YAML
  1. Open 'templates.yaml' (create it in the root folder if it does not exist).
  2. Overwrite the contents entirely with the sanitized LD01 telemetry matrix.

[C] AUTOMATIONS.YAML
  1. Open 'automations.yaml'.
  2. Append or replace your active blocks with the updated 'LD01' and 
     'Heavy Rain' rules.
  3. NOTE: Both automations utilize 'mode: queued' and custom templates. 
     Modifying these rules inside the Home Assistant Visual UI Editor may 
     strip out or break the raw YAML syntax. Always edit alerts in YAML.

------------------------------------------------------------------------------
STEP 3: SYSTEM VALIDATION & RELOADING
------------------------------------------------------------------------------
Before applying any configuration changes or restarting Home Assistant, you 
MUST validate the structural integrity of your YAML files:

  1. In Home Assistant, navigate to: Developer Tools > YAML.
  2. Click the "Check Configuration" button.
  3. If any errors are flagged, double-check your spacing and line indentation.
  4. Once configuration validation passes successfully, scroll down to 
     "YAML Configuration Reloading" on the same page and click:
     
     * "Reload All YAML Configuration" 
     
     (Alternatively, individually click: Reload Automations, Reload Template 
     Entities, and Reload Command Line Entities & Sensors).

------------------------------------------------------------------------------
STEP 4: GRID EXPANSION (ADDING HARDWARE NODES LD02 THROUGH LD10)
------------------------------------------------------------------------------
When deployment of additional monitoring hardware is required:

  1. Open the target configuration file (configuration.yaml, templates.yaml, 
     or automations.yaml).
  2. Review the structural "Automation Scaling Note" commented at the top.
  3. Duplicate the relevant 'LD01' code block.
  4. Perform a localized Find & Replace inside ONLY your newly duplicated block:
     * Change all instances of "LD01" to your new index (e.g., "LD02")
     * Change all instances of "ld01" to your new index (e.g., "ld02")
     * Change all instances of "landslide_01" to your new index ("landslide_02")
  5. CRITICAL FOR TEMPLATES: Update the 'baseline_x'/'baseline_y' coordinates 
     in the Safety Matrix block and the physical 'latitude'/'longitude' attributes 
     in the Live Trackers block to match the specific field location of the new hardware.
  6. Re-run Step 3 (Validation & Reloading) to bring the new node online.
==============================================================================