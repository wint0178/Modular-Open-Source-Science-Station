/**
 ============================================================================
 Wake-on-Interrupt Landslide Detection System - Version 1.1.5 (WOILD_V1.1.5)
 ============================================================================
UPDATES:
 - v1.1.5 | 2026-07-25 | Based on v1.1.3: Dynamic NVS Thresholds & Bitpacked Payload Overhaul
  * Added NVS flash persistence (Preferences API) to store dynamic motion settings across sleep cycles.
  * Integrated 2-byte LoRaWAN downlink processing (`processDownlink`) to update thresholds over-the-air.
  * Dynamically updates 3 active parameters: Motion Threshold (0–224 mg), Software Gate (0–224 mg), and Duration Index (10–2000 ms).
  * Restructured 8-byte uplink payload (`buildUplinkPayload`) to echo active NVS configuration states back to TTN/Home Assistant[cite: 4].
 
 - v1.1.4 | 2026-07-08 | Branched version, not carried into v1.1.5
  * Fixed tripwire lockup loop by checking pin status at top of `handleAlertMode()` to auto-clear stuck RTC flags[cite: 4].
  * Added active movement evaluation (`SOFTWARE_ALERT_THRESHOLD_MG`) at end of alert mode to dynamically decide next sleep cycle[cite: 4].
  * Integrated timer wake tracking (`alertMode` state retention across timer wakeups) to maintain 30s transmissions while hillside is actively moving[cite: 4].
  * Fine-tuned MPU6050 hardware registers (`MOT_DETECT_CTRL` = 0x15, `0x6C` = 0x87) for hardware wake stability[cite: 4].

 - v1.1.3 | 2026-07-06 |
  * Added second gravitational threshold so that the wake on interrupt and LoRa tranmission have separate values.
  * Added a low voltage sleep mode when the node wakes at 3.0 V, it will sleep for 20 hours to charge.

 - v1.1.2 | 2026-07-05 |
  * Put MPU6050 gyroscopes into hardware standby during deep sleep.
  * Forced 20Hz low-power accelerometer cycle mode, dropping sleep floor from ~3.5mA down to true micro-amps.
 
 - v1.1.1 | 2026-07-05 | Fixed OLED glitch
  * Fixed a sporatic glitch where the Heltec onboard OLED would display static, thus draing battery power
 
 - v1.1.0 | 2026-07-01 | First deployment version
  * Fixed parasitic battery drain on enterDeepSleep() due to incorrect internal resistor position
 
 - v1.0.4 | 2026-06-23 | Production Integrity & Field Diagnostics Update
  * Integrated a 15-second defensive timeout fallback to prevent runaway battery drain on gateway dropouts.
  * Added a local 3-second BOOT button hardware override to launch real-time telemetry serial streams on-site.
  * Stabilized deep-sleep recovery cycles utilizing FreeRTOS core yield balancing flags.
  * Cleaned redundant library initializations to keep active awake times under 500ms.
 
 - v1.0.3 test4 | 2026-06-22 |
  * Implemented 8-byte high-efficiency bit-packed payload with 12-bit angle precision, mapped battery/temp, and 6-bit dynamic vector capture.
  * Added USE_TRIPWIRE true/false configuration switch to cleanly toggle off loop code for specific units.
  * Dual-sensor configuration: Humidity from DHT22, Temperature from internal MPU6050 core.                                    
                                   
 - v1.0.3 test3 | 2026-06-16 | Defensive Battery & State Isolation
  * Stripped high-power while loops from Wake-On-Interrupt tracking to preserve battery under field faults[cite: 552].
  * Extended normal routine deep sleep interval cleanly to 1 hour (3600 seconds) to lower routine drain[cite: 565].
  * Isolated tripwire alert reporting down to a 10-minute cycle (600 seconds) to limit loop transmission storm[cite: 656, 657].
  * Rewrote 'sendSensorData' to dynamically inject explicit system state byte arguments (0 = Timed, 1 = Motion, 3 = Tripwire)[cite: 552, 573].

 - v1.0.3 test2 | 2026-06-16 | Integration of Physical Security Loop
  * Added hardware-level security loop break detection tracking using EXT1 wake monitoring[cite: 277].
  * Allocated GPIO 5 (TRIP_PIN) as the physical security line with persistent software pull-up definitions[cite: 411, 566].
  * Implemented 'isTripwireBroken' boolean flag inside persistent RTC non-volatile memory to survive deep sleep cycles[cite: 571, 662, 663].
  * Configured adaptive sleep mask routing to disable the RTC_PERIPH power domain entirely once a broken wire is flagged, preventing parasitic loop leak.
 
 - v1.0.3 test1 | 2026-06-13 | Security Loop & Breakout Optimization
  * Configured multi-pin EXT1 bitmask to monitor both MPU6050 and the new tripwire.
  * Added hardware-level break detection tripwire logic on GPIO 5 with internal pull-up.
  * Embedded esp_sleep_get_ext1_wakeup_status() diagnostic tracking into setup() routine.
 
 - v1.0.3  | 2026-06-12 | Hardware-Level Power Architecture & Battery Fixes
  * Swapped analog monitoring path from standard library call to true hardware trace (GPIO 1).
  * Isolated the battery divider network (GPIO 37) to eliminate continuous parasitic drain.
  * Resolved deep sleep lockup by brute-forcing the ESP32-S3 RTC pad hold registers on wake.
  * Preserved Wake-on-Interrupt capability by keeping the sensor power rail (GPIO 36) active.
============================================================================


 HARDWARE:
 - Heltec WiFi LoRa 32 V3 (ESP32-S3)
 - MPU6050 (Accelerometer/Gyroscope for tilt detection)
 - DHT22 (Temperature/Humidity sensor)
 - 3000mAh LiIon Battery
 - 5V, 1W Solar Panel
 - CN3065 Solar battery charger

 FUNCTIONALITY:
 - Monitors ground tilt angles (X and Y) for landslide detection
 - SLEEP MODE: Deep sleep, wakes every 1 hour OR on motion interrupt (~20µA)
 - ALERT MODE: Continuous monitoring when motion detected, transmits every 30s (~100mA)
 - Returns to sleep after 45 seconds of calm (testing) or 5 mins (production)
 - TEST MODE: Hold BOOT button on startup for live accelerometer monitoring
 - MOTION TEST MODE: Set MOTION_TEST_MODE true to test without LoRa/TTN

 POWER CONSUMPTION:
 - Sleep Mode: ~20µA (ESP32 sleeping, MPU6050 monitoring motion)
 - Alert Mode: ~100mA (continuous reading + LoRa transmissions)

 WIRING:
 - MPU6050: SDA=GPIO41, SCL=GPIO42, INT=GPIO4, VCC=3.3V, GND=GND
 - DHT22: Data=GPIO45, VCC=3.3V, GND=GND
 - Battery voltage: Automatically handled via native library calls
 ============================================================================
 */
// ============================================================================
// CONFIGURATION
// ============================================================================

#define MOTION_TEST_MODE false      // Set to true to test accelerometer without LoRa/TTN
#define USE_TRIPWIRE true          // Set to false if physical security loop is not installed!

#define MINIMUM_DELAY 3600          // Deep sleep duration in seconds for normal routine (1 hour)
#define TRIP_ALERT_DELAY 600        // Alert interval in seconds when tripwire is broken (10 minutes)

#define ALERT_INTERVAL 30000        // Transmit every 30 seconds during alert mode
#define MPU_INT_PIN 4               // MPU6050 INT pin connected to GPIO4
#define TRIP_PIN 5                  // Physical security wire loop pin connected to GPIO5

#define BATTERY_CUTOFF_VOLTAGE 3.00 // Voltage threshold to suspend all operations

// ============================================================================
// LIBRARIES
// ============================================================================

#include "heltec_unofficial.h"
#include <LoRaWAN_ESP32.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <MPU6050_light.h>
#include "DHT.h"
#include <Preferences.h>

Preferences prefs;

// Look-up table mapping 3-bit indices (0-7) to duration in milliseconds
const uint16_t DURATION_LOOKUP_MS[8] = { 1, 3, 5, 7, 9, 11, 13, 15 };

// Active system settings (NVS backed)
uint8_t motion_threshold_lsb   = 1; // Default: 1 (32 mg)
uint8_t software_threshold_lsb = 2; // Default: 2 (64 mg)
uint8_t duration_index         = 3; // Default: 7 (3 ms)

// =========================================================================
// NVS LOAD & SAVE FUNCTIONS
// =========================================================================
void loadSystemSettings() {
  prefs.begin("woild_cfg", true);
  motion_threshold_lsb   = prefs.getUChar("mot_th", 1);
  software_threshold_lsb = prefs.getUChar("sw_th", 1);
  duration_index         = prefs.getUChar("dur_idx", 1);
  prefs.end();

  Serial.println(F("\n[NVS SETTINGS LOADED]"));
  Serial.printf("  Motion Threshold LSB:   %d (%d mg)\n", motion_threshold_lsb, motion_threshold_lsb * 32);
  Serial.printf("  Software Threshold LSB: %d (%d mg)\n", software_threshold_lsb, software_threshold_lsb * 32);
  Serial.printf("  Motion Duration Index:  %d (%d ms)\n\n", duration_index, DURATION_LOOKUP_MS[duration_index & 0x07]);
}

void saveSystemSettings(uint8_t new_mot, uint8_t new_sw, uint8_t new_dur_idx) {
  prefs.begin("woild_cfg", false);
  prefs.putUChar("mot_th", new_mot);
  prefs.putUChar("sw_th", new_sw);
  prefs.putUChar("dur_idx", new_dur_idx & 0x07);
  prefs.end();

  motion_threshold_lsb   = new_mot;
  software_threshold_lsb = new_sw;
  duration_index         = new_dur_idx & 0x07;

  Serial.println(F(">>> [SETTINGS OVERWRITTEN & SAVED TO NVS FLASH] <<<"));
}

// =========================================================================
// UPLINK PAYLOAD CONSTRUCTOR (8 BYTES, NONCE PRESERVED)
// =========================================================================
void buildUplinkPayload(uint8_t* payload, uint8_t nonce, uint8_t systemState, float dynamicVectorMg, int16_t x_scaled, int16_t y_scaled) {
  // Byte 0: Nonce
  payload[0] = nonce;

  // Byte 1: Motion (3b) | SW (3b) | Duration Index Bottom 2 Bits (2b)
  payload[1] = (motion_threshold_lsb & 0x07) |
               ((software_threshold_lsb & 0x07) << 3) |
               ((duration_index & 0x03) << 6);

  // Extract 9-bit Signed Angles (-256 to +255)
  uint16_t x_9bit = (uint16_t)(x_scaled & 0x01FF);
  uint16_t y_9bit = (uint16_t)(y_scaled & 0x01FF);

  uint8_t x_upper = (uint8_t)((x_9bit >> 1) & 0xFF);
  uint8_t y_upper = (uint8_t)((y_9bit >> 1) & 0xFF);
  uint8_t x_lsb   = (uint8_t)(x_9bit & 0x01);
  uint8_t y_lsb   = (uint8_t)(y_9bit & 0x01);

  // Compress Vector to 3 bits (8 mg per LSB, range 0-56 mg)
  uint8_t vector_3bit = (uint8_t)constrain(round(dynamicVectorMg / 8.0), 0.0, 7.0);

  // Top bit of duration index
  uint8_t duration_top_bit = (duration_index & 0x04) >> 2;

  // Byte 3 & 4: Angle Upper Bytes
  payload[3] = x_upper;
  payload[4] = y_upper;

  // Byte 5: Shared Bits Block
  payload[5] = (x_lsb & 0x01) |
               ((y_lsb & 0x01) << 1) |
               ((systemState & 0x03) << 2) |
               ((duration_top_bit & 0x01) << 4) |
               ((vector_3bit & 0x07) << 5);
}

// ============================================================================
// HARDWARE OBJECTS & PERSISTENCE
// ============================================================================

LoRaWANNode* node;
extern TwoWire Wire1;                    
MPU6050 mpu(Wire1);

#define VBAT_Read     1
#define ADC_Ctrl     37
#define DHTPIN       45                  
#define DHTTYPE      DHT22                  
DHT dht(DHTPIN, DHTTYPE);

// MPU6050 REGISTER ADDRESSES
#define MPU6050_ADDR 0x68
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_INT_PIN_CFG 0x37
#define MPU6050_INT_ENABLE 0x38
#define MPU6050_INT_STATUS 0x3A
#define MPU6050_MOT_THR 0x1F
#define MPU6050_MOT_DUR 0x20
#define MPU6050_MOT_DETECT_CTRL 0x69
#define MPU6050_ACCEL_XOUT_H 0x3B

// ============================================================================
// RTC MEMORY VARIABLES (Persist through deep sleep)
// ============================================================================

RTC_DATA_ATTR uint8_t count = 0;
RTC_DATA_ATTR uint8_t trans = 0;             
RTC_DATA_ATTR bool alertMode = false;        
RTC_DATA_ATTR unsigned long lastMotionTime = 0;
RTC_DATA_ATTR uint32_t alertCount = 0;
RTC_DATA_ATTR float rawb = 0;        
RTC_DATA_ATTR bool isTripwireBroken = false; 

// FUNCTION PROTOTYPES
void configureMPU6050Interrupt();
bool isMotionCalm();
void enterDeepSleep(uint32_t seconds);
float readAccelMagnitude();
void sendSensorData(uint8_t systemState, float max_delta_mg);
void handleAlertMode();
void handleNormalMode();
void updateBatteryReading();

// ============================================================================
// HELPERS
// ============================================================================

void updateBatteryReading() {
  #if ESP_ARDUINO_VERSION_MAJOR >= 3
    gpio_deep_sleep_hold_dis();
  #endif
  
  gpio_hold_dis((gpio_num_t)36);
  gpio_hold_dis((gpio_num_t)37);
  if (USE_TRIPWIRE) {
    gpio_hold_dis((gpio_num_t)TRIP_PIN); 
  }

  pinMode(36, INPUT);
  pinMode(37, INPUT);
  delay(10);

  pinMode(36, OUTPUT);
  digitalWrite(36, LOW);

  pinMode(37, OUTPUT);
  digitalWrite(37, HIGH);
  
  delay(350);

  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  uint32_t mv_accumulator = 0;
  for(int i = 0; i < 10; i++) {
    mv_accumulator += analogReadMilliVolts(1);
    delay(5);
  }
  uint32_t mv = mv_accumulator / 10;

  rawb = ((float)mv / 1000.0) * 4.90;
  digitalWrite(37, LOW);

  Serial.printf("[ADC SUCCESS] True Battery GPIO1 Raw: %lu mV | Calculated Float: %0.2f V\n", mv, rawb);
  Serial.flush();

  if (rawb < 1.0) rawb = 0.0;
}

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  Serial.begin(115200);
  heltec_setup();

  loadSystemSettings(); // Retrieve persistent config from NVS flash
  
  Wire1.begin(41, 42); 
  delay(500);
  analogReadResolution(12);
  pinMode(MPU_INT_PIN, INPUT);
  
  if (USE_TRIPWIRE) {
    pinMode(TRIP_PIN, INPUT_PULLUP);
  }

  yield();

  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  uint64_t ext1_status = esp_sleep_get_ext1_wakeup_status();

  updateBatteryReading();
  dht.begin(); 

  Serial.println();
  Serial.println(F("=========================================================="));
  Serial.printf("Initial Boot Battery Voltage: %0.2f V\n", rawb);

  if (rawb < 3.0) {
    Serial.println(F("CRITICAL: Battery below 3.0V! Entering low-power isolation sleep."));
    Serial.flush();
    esp_deep_sleep(72000000000ULL); // Put the full board to sleep for 20 hours to charge
  }

  #if defined(REGION_US915) || defined(REGION_AU915)
  Serial.println(F("[LoRaWAN] Regional Profile Identified: US915/AU915 Sub-band 2 Active"));
  #endif
  
  // ROUTING DECISION MATRIX
  if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT1) {
    if (USE_TRIPWIRE && ((ext1_status & (1ULL << TRIP_PIN)) != 0)) {
      delay(150); // Debounce gate
      
      if (digitalRead(TRIP_PIN) == HIGH) {
        Serial.println(F("*** ALERT: GENUINE TRIPWIRE BREAK CONFIRMED ***"));
        isTripwireBroken = true;
      } else {
        Serial.println(F("⚠️ [FALSE POSITIVE] Micro-break filtered on GPIO5. Aborting Radio Uplink."));
        isTripwireBroken = false; 
        alertMode = false;
        
        Wire1.beginTransmission(MPU6050_ADDR);
        Wire1.write(MPU6050_INT_STATUS);
        Wire1.endTransmission(false);
        Wire1.requestFrom(MPU6050_ADDR, 1);
        if (Wire1.available()) Wire1.read();
        
        enterDeepSleep(MINIMUM_DELAY);
      }
    } else {
      Serial.println(F("*** MOTION DETECTED - ENTERING ALERT MODE ***"));
      alertMode = true;
      alertCount++;
      lastMotionTime = millis(); 
    }
  } else if (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER) {
    Serial.println(F("Woke from timer - routine check"));
  } else {
    Serial.println(F("Power-on or hardware reset pin pulled"));
    alertMode = false;
    isTripwireBroken = false; 
  }
  
  if (USE_TRIPWIRE && digitalRead(TRIP_PIN) == LOW && isTripwireBroken) {
    isTripwireBroken = false;
    Serial.println(F("🔧 [FIELD SERVICE] Physical link restored. Clearing Alarm state parameters."));
  }
  
  Serial.println(F("\n=== BOOT BUTTON DIAGNOSTIC CHECK ==="));
  Serial.println(F("Hold BOOT button down for 3 seconds to run local hardware telemetry test..."));
  pinMode(0, INPUT_PULLUP);
  bool diagnosticActive = true;
  for(int i = 0; i < 30; i++) {
    if(digitalRead(0) == HIGH) {
      diagnosticActive = false;
      break;
    }
    delay(100);
  }
  if(diagnosticActive) {
    Serial.println(F("▶️ Entering Continuous Real-Time Diagnostic Mode (Reset Node to Exit)"));
    byte mpu_diag = mpu.begin();
    while(mpu_diag != 0) { Serial.println(F("✗ MPU Link Failed")); delay(1000); }
    while(true) {
      mpu.update();
      float current_magnitude = readAccelMagnitude();
      Serial.printf("Telemetry Frame | AngleX: %5.2f° | AngleY: %5.2f° | MotionDelta: %5.2f mg | Tripwire Pin State: %s\n", 
                    mpu.getAngleX(), mpu.getAngleY(), current_magnitude, (digitalRead(TRIP_PIN) == HIGH) ? "OPEN/BROKEN" : "CLOSED/OK");
      delay(250);
    }
  }

  pinMode(0, INPUT); 
  byte status = mpu.begin();
  uint8_t mpuAttempts = 0;
  while(status != 0 && mpuAttempts < 5) { 
    delay(500);
    status = mpu.begin();
    mpuAttempts++;
  } 

  configureMPU6050Interrupt();
  pinMode(DHTPIN, INPUT_PULLUP); 
  dht.begin();
  delay(100);

  // --- MASTER RADIO INITIALIZATION ---
  if (!MOTION_TEST_MODE) {
    radio.begin();
    node = persist.manage(&radio);
    if (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER || wakeup_reason == ESP_SLEEP_WAKEUP_EXT0 || wakeup_reason == ESP_SLEEP_WAKEUP_EXT1) {
      persist.loadSession(node);
    }
    node->setADR(false); 
    node->setDatarate(3); 
    node->setDutyCycle(false, 1250); 
  }

  if (alertMode) {
    handleAlertMode();
  } else {
    handleNormalMode();
  }
} 

void loop() {}

// ============================================================================
// SYSTEM STATE HANDLERS
// ============================================================================

void handleAlertMode() {
  uint8_t transmissionState = 1; // 1 = Motion
  if (USE_TRIPWIRE && isTripwireBroken) transmissionState = 3; // 3 = Tripwire
  
  // Sample window to capture the peak dynamic force spike
  float max_delta_mg = 0.0;
  for (int i = 0; i < 20; i++) {
    mpu.update();
    float current_delta = readAccelMagnitude();
    if (current_delta > max_delta_mg) {
      max_delta_mg = current_delta;
    }
    delay(10);
  }

  // DYNAMIC SOFTWARE GATE CHECK
  uint16_t active_sw_gate_mg = software_threshold_lsb * 32;
  if (!isTripwireBroken && (max_delta_mg < active_sw_gate_mg)) {
    Serial.printf("[IGNORE] Micro-wake event. Peak: %.2f mg fell below Software Gate: %d mg. Aborting Tx.\n", 
                  max_delta_mg, active_sw_gate_mg);
    alertMode = false;
    enterDeepSleep(MINIMUM_DELAY);
  }

  Serial.printf("[ALERT] Gate Passed! Peak: %.2f mg >= Limit: %d mg. Transmitting...\n", 
                max_delta_mg, active_sw_gate_mg);
  sendSensorData(transmissionState, max_delta_mg);
  
  alertMode = false;
  enterDeepSleep(ALERT_INTERVAL / 1000); 
}

void handleNormalMode() {
  uint8_t transmissionState = 0; // 0 = Timed Routine
  if (USE_TRIPWIRE && isTripwireBroken) transmissionState = 3; 
  
  float max_delta_mg = 0.0;
  for (int i = 0; i < 20; i++) {
    mpu.update();
    float current_delta = readAccelMagnitude();
    if (current_delta > max_delta_mg) {
      max_delta_mg = current_delta;
    }
    delay(10);
  }
  
  sendSensorData(transmissionState, max_delta_mg);
  enterDeepSleep((USE_TRIPWIRE && isTripwireBroken) ? TRIP_ALERT_DELAY : MINIMUM_DELAY);
}

// ============================================================================
// SEND SENSOR DATA (8-Byte Payload & Downlink Processing)
// ============================================================================

void sendSensorData(uint8_t systemState, float max_delta_mg) {
  updateBatteryReading();
  
  uint8_t hum = (uint8_t)constrain(round(dht.readHumidity()), 0.0, 100.0); 
  float raw_temp = mpu.getTemp();

  // Convert angles to 9-bit signed structures (0.1 degree resolution, range -25.6° to +25.5°)
  int16_t x_scaled = (int16_t)constrain(round(mpu.getAngleX() * 10.0), -256.0, 255.0);
  int16_t y_scaled = (int16_t)constrain(round(mpu.getAngleY() * 10.0), -256.0, 255.0);

  // Map MPU Temp: (T + 10) * 4 to fit -10°C to 53.75°C with 0.25°C steps into 1 byte
  uint8_t packed_temp = (uint8_t)constrain(round((raw_temp + 10.0) * 4.0), 0.0, 255.0);

  // Map Battery: (V - 2.0) * 100
  uint8_t packed_vbat = (uint8_t)constrain(round((rawb - 2.0) * 100.0), 0.0, 255.0);

  // Debug Print Verification Block
  Serial.println(F("\n====== 8-BYTE BITPACKED PAYLOAD DICTIONARY ======"));
  Serial.printf("Battery: %0.2f V -> Encoded: %d\n", rawb, packed_vbat);
  Serial.printf("Humidity: %d %%\n", hum);
  Serial.printf("Angles (0.1° Res) -> X: %0.1f° | Y: %0.1f°\n", mpu.getAngleX(), mpu.getAngleY());
  Serial.printf("Peak Dynamic G Delta: %0.1f mg\n", max_delta_mg);
  Serial.printf("Silicon Temp: %0.2f °C -> Encoded: %d\n", raw_temp, packed_temp);
  Serial.println(F("================================================"));

  if (MOTION_TEST_MODE) return;

  if (!node->isActivated()) {
    Serial.println(F("[LoRaWAN] Not activated yet. Attempting to join network..."));
    unsigned long joinStartTime = millis();
    while (!node->isActivated() && (millis() - joinStartTime < 15000)) {
      yield(); 
      delay(100);
    }
    if (!node->isActivated()) {
      Serial.println(F("✗ Could not join network. Executing emergency safety sleep..."));
      enterDeepSleep(MINIMUM_DELAY);
    }
  }

  yield(); delay(100);
  
  uint8_t random_byte = (uint8_t)(esp_random() % 256);
  uint8_t uplinkData[8]; 
  
  // Pack Nonce, Config, Angles, State, and Vector into Bytes 0, 1, 3, 4, and 5
  buildUplinkPayload(uplinkData, random_byte, systemState, (uint8_t)max_delta_mg, x_scaled, y_scaled);
  
  // Assign remaining telemetry bytes cleanly
  uplinkData[2] = packed_vbat;   // Byte 2: Battery Voltage
  uplinkData[6] = hum;           // Byte 6: Relative Humidity (0-100%)
  uplinkData[7] = packed_temp;   // Byte 7: MPU Silicon Temperature

  uint8_t downlinkData[256];
  memset(downlinkData, 0, sizeof(downlinkData)); 
  size_t lenDown = 0; 
  
  // Transmit Uplink and Listen for RX1/RX2 Downlinks
  node->sendReceive(uplinkData, sizeof(uplinkData), 1, downlinkData, &lenDown);
  delay(200);

  // =========================================================================
  // DOWNLINK CONFIG OVERWRITE HANDLER (2-BYTE)
  // =========================================================================
  if (lenDown >= 2) {
    uint8_t byte0 = downlinkData[0];
    uint8_t byte1 = downlinkData[1];

    uint8_t new_motion_lsb   = byte0 & 0x07;
    uint8_t new_software_lsb = (byte0 >> 3) & 0x07;
    uint8_t new_duration_idx = byte1 & 0x07;

    Serial.println(F("\n====== [DOWNLINK OVERWRITE RECEIVED] ======"));
    Serial.printf("Raw Downlink Bytes (HEX): 0x%02X 0x%02X\n", byte0, byte1);
    Serial.printf("-> New Motion Threshold LSB: %d (%d mg)\n", new_motion_lsb, new_motion_lsb * 32);
    Serial.printf("-> New Software Threshold LSB: %d (%d mg)\n", new_software_lsb, new_software_lsb * 32);
    Serial.printf("-> New Motion Duration Index:  %d (%d ms)\n", new_duration_idx, DURATION_LOOKUP_MS[new_duration_idx]);
    
    saveSystemSettings(new_motion_lsb, new_software_lsb, new_duration_idx);
    Serial.println(F("===========================================\n"));
  } else if (lenDown == 1) {
    Serial.println(F("[DOWNLINK WARNING] Received 1 byte, expected 2 bytes (Byte0: Thresh, Byte1: Duration)."));
  } else {
    Serial.println(F("[DOWNLINK] No downlink payload received in RX window."));
  }
}

// ============================================================================
// CONFIGURE MPU6050 INTERRUPT
// ============================================================================

void configureMPU6050Interrupt() {
  Wire1.beginTransmission(MPU6050_ADDR);
  Wire1.write(MPU6050_PWR_MGMT_1);
  Wire1.write(0x00);
  Wire1.endTransmission(true);
  delay(100);
  
  Wire1.beginTransmission(MPU6050_ADDR);
  Wire1.write(MPU6050_INT_PIN_CFG); 
  Wire1.write(0x10);                
  Wire1.endTransmission(true);
  
  // Dynamic Motion Threshold from NVS (1 LSB = 32 mg)
  Wire1.beginTransmission(MPU6050_ADDR);
  Wire1.write(MPU6050_MOT_THR);
  Wire1.write(motion_threshold_lsb); // Index 0-7 directly sets 0-224 mg
  Wire1.endTransmission(true);
  
  // Dynamic Motion Duration Count from NVS lookup index (1 count = 1 ms)
  Wire1.beginTransmission(MPU6050_ADDR);
  Wire1.write(MPU6050_MOT_DUR);
  Wire1.write(DURATION_LOOKUP_MS[duration_index & 0x07]); // Writes 1, 3, 5, 7, 9, 11, 13, or 15 ms
  Wire1.endTransmission(true);
  
  // Set Motion Detect Counter decrement rate (0x15 = 1ms decrement, matching DURATION_LOOKUP_MS)
  Wire1.beginTransmission(MPU6050_ADDR);
  Wire1.write(MPU6050_MOT_DETECT_CTRL);
  Wire1.write(0x15); 
  Wire1.endTransmission(true);
  
  Wire1.beginTransmission(MPU6050_ADDR);
  Wire1.write(MPU6050_INT_ENABLE);
  Wire1.write(0x40); 
  Wire1.endTransmission(true);
  
  Wire1.beginTransmission(MPU6050_ADDR);
  Wire1.write(MPU6050_INT_STATUS);
  Wire1.endTransmission(false);
  Wire1.requestFrom(MPU6050_ADDR, 1);
  if (Wire1.available()) Wire1.read();
}

float readAccelMagnitude() {
  Wire1.beginTransmission(MPU6050_ADDR);
  Wire1.write(MPU6050_ACCEL_XOUT_H);
  Wire1.endTransmission(false);
  Wire1.requestFrom(MPU6050_ADDR, 6, true);
  if (Wire1.available() < 6) return 0.0;

  int16_t ax = (Wire1.read() << 8) | Wire1.read();
  int16_t ay = (Wire1.read() << 8) | Wire1.read();
  int16_t az = (Wire1.read() << 8) | Wire1.read();
  float total_mg = sqrt((float)ax*ax + (float)ay*ay + (float)az*az) / 16.384;
  
  static float last_total_mg = 1000.0;
  float delta_motion = abs(total_mg - last_total_mg);
  last_total_mg = (total_mg * 0.2) + (last_total_mg * 0.8);
  return delta_motion;
}

void enterDeepSleep(uint32_t seconds) {
  if (!MOTION_TEST_MODE) {
    persist.saveSession(node);
    uint32_t interval = node->timeUntilUplink();
    uint32_t delayMs = max(interval, seconds * 1000);
    seconds = delayMs / 1000;
  }
  
  delay(200);

  // 1. SURGICAL LOW-POWER ACCELEROMETER CYCLE SEQUENCE
  Wire1.beginTransmission(MPU6050_ADDR);
  Wire1.write(MPU6050_INT_PIN_CFG); 
  Wire1.write(0x20); 
  Wire1.endTransmission(true);

  Wire1.beginTransmission(MPU6050_ADDR);
  Wire1.write(0x6C); 
  Wire1.write(0xC7); 
  Wire1.endTransmission(true);

  Wire1.beginTransmission(MPU6050_ADDR);
  Wire1.write(MPU6050_INT_STATUS); 
  Wire1.endTransmission(false);
  Wire1.requestFrom(MPU6050_ADDR, 1);
  if (Wire1.available()) Wire1.read(); 

  Wire1.beginTransmission(MPU6050_ADDR);
  Wire1.write(MPU6050_PWR_MGMT_1); 
  Wire1.write(0x20); 
  Wire1.endTransmission(true);

  // 2. SHUTDOWN ESP32 I2C ENGINE & ISOLATE BUS PINS
  Wire1.end(); 
  
  pinMode(41, INPUT); 
  pinMode(42, INPUT); 

  // 3. SHUTDOWN THE HELTEC OLED DISPLAY
  #ifndef DEACTIVATE_DISPLAY
    display.displayOff(); 
  #endif
  delay(10);

  digitalWrite(Vext, HIGH); 

  pinMode(36, OUTPUT);
  digitalWrite(36, LOW); 
  gpio_hold_en((gpio_num_t)36); 

  // 4. PIN HOLDS & SLEEP MASK
  pinMode(37, OUTPUT);
  digitalWrite(37, HIGH);
  gpio_hold_en((gpio_num_t)37); 
  
  pinMode(45, INPUT); 
  
  esp_sleep_enable_timer_wakeup(seconds * 1000000ULL);
  
  uint64_t pin_bitmask = 0;
  
  if (USE_TRIPWIRE) {
    if (!isTripwireBroken) {
      pin_bitmask = (1ULL << MPU_INT_PIN) | (1ULL << TRIP_PIN);
      esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);

      pinMode(TRIP_PIN, INPUT_PULLUP); 
      gpio_pullup_en((gpio_num_t)TRIP_PIN);
      gpio_pulldown_dis((gpio_num_t)TRIP_PIN);
      gpio_hold_en((gpio_num_t)TRIP_PIN); 
    } else {
      pin_bitmask = (1ULL << MPU_INT_PIN);
      esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
    }
  } else {
    pin_bitmask = (1ULL << MPU_INT_PIN);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  }
  
  esp_sleep_enable_ext1_wakeup(pin_bitmask, ESP_EXT1_WAKEUP_ANY_HIGH); 
  esp_deep_sleep_start();
}