# 🏠 Stage 1: Home Assistant OS Bare-Metal Installation

This directory manages the initial physical provisioning and bare-metal OS installation for the central **Modular and Open-Source Science Station (MOSSS)** edge gateway.

---

## 📋 Prerequisites & Hardware Checklist

Before beginning, ensure your central hub hardware components match our verified specifications:

*   **Single Board Computer:** Raspberry Pi 4 or Raspberry Pi 5 (Minimum 4GB RAM recommended).
*   **Power Supply:** Official Raspberry Pi USB-C power supply (15W for Pi 4, 27W for Pi 5) to prevent undervoltage failures.
*   **Storage:** A 32GB high-endurance microSD card (UHS Speed Class 3 / V30 or better rated for continuous write cycles) or an external USB 3.0 SSD.
*   **Network:** An Ethernet cable connected directly to your local network switch/router for initial provisioning.

---

## 💿 Installation Step-by-Step

Because the gateway needs to compute vector matrices locally without internet dependence, we utilize the bare-metal **Home Assistant Operating System (HAOS)**.

> ⚠️ **CRITICAL ORDERING:** Adhere strictly to the sequence below. Misordering these setup steps will result in a failed headless boot or network mismatch.

---

### Step-by-Step Sequence

<ul style="list-style: none; padding-left: 0;">

  <!-- STEP 1 -->
  <li style="margin-bottom: 30px; border-bottom: 1px solid #30363d; padding-bottom: 20px;">
    <h3 style="margin-top: 0;">1. Download the Flashing Tool</h3>
    <p style="color: #8b949e; margin-top: -10px; font-size: 14px;"><em>Requires Mac, Windows, or Linux PC</em></p>
    Download and install the official <strong>Raspberry Pi Imager</strong> from <a href="https://www.raspberrypi.com/software/">raspberrypi.com/software</a>. Insert your storage media into your flashing computer.
  </li>

  <!-- STEP 2 -->
  <li style="margin-bottom: 30px; border-bottom: 1px solid #30363d; padding-bottom: 20px;">
    <h3 style="margin-top: 0;">2. Select the HAOS Image</h3>
    <p style="color: #8b949e; margin-top: -10px; font-size: 14px;"><em>Do not use default Pi OS</em></p>
    <ol>
      <li>Launch the Imager tool.</li>
      <li>Click <strong>Choose Device</strong> and select your model (e.g., Raspberry Pi 5).</li>
      <li>Click <strong>Choose OS</strong>, scroll down to select <strong>Other specific-purpose OS</strong>, click <strong>Home Assistant</strong>, and choose <strong>Home Assistant OS</strong>.</li>
    </ol>
  </li>

  <!-- STEP 3 (Includes critical warning and image) -->
  <li style="margin-bottom: 30px; border-bottom: 1px solid #30363d; padding-bottom: 20px;">
    <h3 style="margin-top: 0;">3. Flash without OS Customization</h3>
    <p style="color: #8b949e; margin-top: -10px; font-size: 14px;"><em>Crucial Step</em></p>
    Select your target storage device and click <strong>Next</strong>.<br><br>
    
    > ⚠️ **CRITICAL:** If the imager prompts you to apply OS customization settings (like setting up Wi-Fi or SSH), **select NO**. HAOS manages its own network initialization—applying custom configurations through the imager will corrupt the system container structure. Confirm and write the image.<br><br>
    
    ![descriptive-alt-text-for-accessibility](./images/Pi-imager.png)
  </li>

  <!-- STEP 4 -->
  <li style="margin-bottom: 30px; border-bottom: 1px solid #30363d; padding-bottom: 20px;">
    <h3 style="margin-top: 0;">4. Headless Initial Boot</h3>
    <p style="color: #8b949e; margin-top: -10px; font-size: 14px;"><em>Takes 5-15 minutes</em></p>
    Insert the flashed storage into your Raspberry Pi. Connect an <strong>Ethernet cable</strong> to your router, then plug in the power supply. The Pi will boot headlessly; give it up to 15 minutes to automatically provision, unpack the environment, and fetch system dependencies.
  </li>

  <!-- STEP 5 -->
  <li style="margin-bottom: 30px; border-bottom: 1px solid #30363d; padding-bottom: 20px;">
    <h3 style="margin-top: 0;">5. Complete Onboarding UI</h3>
    <p style="color: #8b949e; margin-top: -10px; font-size: 14px;"><em>Web Browser Setup</em></p>
    On a computer connected to the same local network, open a browser window and navigate to:
    <code>http://homeassistant.local:8123</code>
    *(If the hostname fails to resolve, check your router's DHCP client list to find the Pi's IP address and navigate to <code>http://YOUR_PI_IP:8123</code>).* Follow the prompts to create your local admin account.
  </li>

</ul>

---

## 🧭 Next Step
Once you have landed on your fresh home assistant dashboard and created your main owner account, advance to user provisioning and system networking:

👉 **Proceed to Stage 2:** [Go to 2-OS-Provisioning](../2-OS-Provisioning/)
