/**
 ============================================================================
 Wake-on-Interrupt Landslide Detection System - Version 1.1.3 (WOILD_V1.1.3)
 ============================================================================
UPDATES:
 - v1.1.3 | 2026-07-06 |
  * Added second gravitational threshold so that the wake on interrupt and LoRa tranmission have separate values.
  * Added a low voltage sleep mode when the node wakes at 3.0 V, it will sleep for 20 hours to charge.
  * Add

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

#define MINIMUM_DELAY 3600         // Deep sleep duration in seconds for normal routine (1 hour)
#define TRIP_ALERT_DELAY 600       // Alert interval in seconds when tripwire is broken (10 minutes)

// UNIFIED WAKE-ON-INTERRUPT SETTINGS
#define TARGET_THRESHOLD_MG 32     // Retained original target sensitivity for physical hardware wake-ups
#define MOTION_THRESHOLD (TARGET_THRESHOLD_MG / 32)
#define MOTION_DURATION 3          // Duration requirement: 1ms (minimum)

// Settings the set a software gate to control when the LoRa board transmits data
#define SOFTWARE_ALERT_THRESHOLD_MG 20    // The high fence: ESP32 qualifies the event before sending LoRaWAN
#define SOFTWARE_THRESHOLD (SOFTWARE_ALERT_THRESHOLD_MG / 32)

//#define CALM_DURATION 45000        // manditory wait period (in ms) before sending another alert message even if there is another trigger
#define ALERT_INTERVAL 30000       // Transmit every 30 seconds during alert mode
#define MPU_INT_PIN 4              // MPU6050 INT pin connected to GPIO4
#define TRIP_PIN 5                 // Physical security wire loop pin connected to GPIO5

#define BATTERY_CUTOFF_VOLTAGE 3.00  // Voltage threshold to suspend all operations
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
    
    // Explicitly turn off the Heltec OLED/Radio if the library provides shutdown methods
    // heltec_deep_sleep(); // or equivalent if available in your version

    // Put the ESP32 into deep sleep indefinitely to isolate the battery
    // Passing 0 as the microsecond parameter means sleep forever until a physical reset or power cycle occurs
    esp_deep_sleep(72000000000ULL); //Put the full board to sleep for 20 hours to charge
  }

  #if defined(REGION_US915) || defined(REGION_AU915)
  Serial.println(F("[LoRaWAN] Regional Profile Identified: US915/AU915 Sub-band 2 Active"));
  #endif
  
  // ROUTING DECISION MATRIX updated in v1.1.3
  // 1. EVALUATE WAKEUP REASONS WITH THE INTEGRATED PRODUCTION DEBOUNCE GATE
  if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT1) {
    
    // PRIORITY 1: Did the tripwire register a wake signal (even if other pins fired)?
    if (USE_TRIPWIRE && ((ext1_status & (1ULL << TRIP_PIN)) != 0)) {
      
      // DEBOUNCE GATE: Wait 150ms to verify if this is a real cut or just wire rattle
      delay(150); 
      
      if (digitalRead(TRIP_PIN) == HIGH) {
        // The wire is genuinely open and severed.
        Serial.println(F("*** ALERT: GENUINE TRIPWIRE BREAK CONFIRMED ***"));
        isTripwireBroken = true;
      } else {
        // The pin snapped back to LOW. It was a false positive!
        Serial.println(F("⚠️ [FALSE POSITIVE] Micro-break filtered on GPIO5. Aborting Radio Uplink."));
        isTripwireBroken = false; 
        alertMode = false;
        
        // Clear the physical MPU interrupt register right here so it doesn't trick the next boot
        Wire1.beginTransmission(MPU6050_ADDR);
        Wire1.write(MPU6050_INT_STATUS);
        Wire1.endTransmission(false);
        Wire1.requestFrom(MPU6050_ADDR, 1);
        if (Wire1.available()) Wire1.read();
        
        enterDeepSleep(MINIMUM_DELAY); // Put the board back to bed immediately
      }
    } 
    // PRIORITY 2: If the tripwire wasn't part of the wake mask, it's a pure accelerometer event
    else {
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
  
  // 2. PRESERVED FIELD SERVICE FEATURE
  // Runs on routine timer/motion wakes. Clears old alarms automatically if the wire is plugged back in.
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
  if (status != 0) {
    Serial.println(F("✗ Critical Error: MPU6050 Hardware Connection Failed. Entering Emergency Sleep..."));
    enterDeepSleep(MINIMUM_DELAY);
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
// SYSTEM STATE HANDLERS (With integrated Software Filtering Gates)
// ============================================================================

void handleAlertMode() {
  uint8_t transmissionState = 1; // 1 = Motion
  if (USE_TRIPWIRE && isTripwireBroken) transmissionState = 3; // 3 = Tripwire
  
  // 1. Run the active math window on wake to capture the peak dynamic force spike
  float max_delta_mg = 0.0;
  for (int i = 0; i < 20; i++) {
    mpu.update();
    float current_delta = readAccelMagnitude();
    if (current_delta > max_delta_mg) {
      max_delta_mg = current_delta;
    }
    delay(10);
  }

  // 2. THE LORA GATE: If it's a motion alarm but fails the software threshold check, abort transmission
  if (!isTripwireBroken && (max_delta_mg < SOFTWARE_ALERT_THRESHOLD_MG)) {
    Serial.printf("[IGNORE] Micro-wake event. Peak: %.2f mg fell below Software Gate: %d mg. Aborting Tx.\n", 
                  max_delta_mg, SOFTWARE_ALERT_THRESHOLD_MG);
    alertMode = false;
    enterDeepSleep(MINIMUM_DELAY); // Drop back into highly sensitive monitoring mode
  }

  // 3. Gate passed or tripwire forced: Transmit payload data
  Serial.printf("[ALERT] Gate Passed! Peak: %.2f mg >= Limit: %d mg. Transmitting...\n", 
                max_delta_mg, SOFTWARE_ALERT_THRESHOLD_MG);
  sendSensorData(transmissionState, max_delta_mg);
  
  alertMode = false;
  enterDeepSleep(ALERT_INTERVAL / 1000); 
}

void handleNormalMode() {
  uint8_t transmissionState = 0; // 0 = Timed Routine
  if (USE_TRIPWIRE && isTripwireBroken) transmissionState = 3; 
  
  // Scheduled timer wakeups sample a snapshot window to catch any concurrent shifts
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
// SEND SENSOR DATA (Precision Bitpacked Engine)
// ============================================================================

void sendSensorData(uint8_t systemState, float max_delta_mg) {
  updateBatteryReading();
  
  uint8_t hum = (uint8_t)constrain(round(dht.readHumidity()), 0.0, 100.0); 
  float raw_temp = mpu.getTemp();

  // Convert angles to 12-bit signed structures (0.01 degree resolution)
  int16_t x_scaled = (int16_t)constrain(round(mpu.getAngleX() * 100.0), -2048.0, 2047.0);
  int16_t y_scaled = (int16_t)constrain(round(mpu.getAngleY() * 100.0), -2048.0, 2047.0);

  // Isolate Upper 8 bits (shift right by 4 bits)
  uint8_t x_upper = (uint8_t)((x_scaled >> 4) & 0xFF);
  uint8_t y_upper = (uint8_t)((y_scaled >> 4) & 0xFF);

  // Isolate Lower 4 bits
  uint8_t x_lower = (uint8_t)(x_scaled & 0x0F);
  uint8_t y_lower = (uint8_t)(y_scaled & 0x0F);

  // Pack Shared Delta Byte (Byte 5)
  uint8_t shared_delta_byte = (y_lower << 4) | x_lower;

  // Compress Peak Dynamic Vector to 6 bits (Range: 0-63 mg)
  uint8_t peak_vector_6bit = (uint8_t)constrain(round(max_delta_mg), 0.0, 63.0);

  // Pack Shared State/Vector Byte (Byte 6)
  uint8_t shared_state_byte = (peak_vector_6bit << 2) | (systemState & 0x03);

  // Map MPU Temp: (T + 10) * 4 to fit -10°C to 53.75°C with 0.25°C steps into 1 byte
  uint8_t packed_temp = (uint8_t)constrain(round((raw_temp + 10.0) * 4.0), 0.0, 255.0);

  // Map Battery: (V - 2.0) * 100
  uint8_t packed_vbat = (uint8_t)constrain(round((rawb - 2.0) * 100.0), 0.0, 255.0);

  // Debug Print Verification Block
  Serial.println(F("\n====== BITPACKED PAYLOAD DICTIONARY ======"));
  Serial.printf("Battery: %0.2f V -> Encoded: %d\n", rawb, packed_vbat);
  Serial.printf("Angles -> X: %0.2f° | Y: %0.2f°\n", mpu.getAngleX(), mpu.getAngleY());
  Serial.printf("Peak Dynamic G Delta: %0.1f mg -> Encoded: %d\n", max_delta_mg, peak_vector_6bit);
  Serial.printf("Silicon Temp: %0.2f °C -> Encoded: %d\n", raw_temp, packed_temp);
  Serial.println(F("=========================================="));

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
  uplinkData[0] = random_byte;
  uplinkData[1] = packed_vbat;        
  uplinkData[2] = hum;        
  uplinkData[3] = x_upper;   
  uplinkData[4] = y_upper;   
  uplinkData[5] = shared_delta_byte;
  uplinkData[6] = shared_state_byte;      
  uplinkData[7] = packed_temp;   

  uint8_t downlinkData[256];
  size_t lenDown = sizeof(downlinkData);
  
  node->sendReceive(uplinkData, sizeof(uplinkData), 1, downlinkData, &lenDown);
  delay(200);
  yield();
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
  
  Wire1.beginTransmission(MPU6050_ADDR);
  Wire1.write(MPU6050_MOT_THR);
  Wire1.write(MOTION_THRESHOLD);
  Wire1.endTransmission(true);
  
  Wire1.beginTransmission(MPU6050_ADDR);
  Wire1.write(MPU6050_MOT_DUR);
  Wire1.write(MOTION_DURATION);
  Wire1.endTransmission(true);
  
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

  // =========================================================================
  // 1. STEP ONE: SURGICAL LOW-POWER ACCELEROMETER CYCLE SEQUENCE
  // =========================================================================  
  Wire1.beginTransmission(MPU6050_ADDR);
  Wire1.write(MPU6050_INT_PIN_CFG); 
  Wire1.write(0x20); 
  Wire1.endTransmission(true);

  Wire1.beginTransmission(MPU6050_ADDR);
  Wire1.write(0x6C); 
  Wire1.write(0x87); 
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

  // =========================================================================
  // 2. STEP TWO: SHUTDOWN ESP32 I2C ENGINE & ISOLATE BUS PINS
  // =========================================================================
  Wire1.end(); 
  
  pinMode(41, INPUT); 
  pinMode(42, INPUT); 

  // =========================================================================
  // 3. STEP THREE: SHUTDOWN THE HELTEC OLED DISPLAY
  // =========================================================================
  #ifndef DEACTIVATE_DISPLAY
    display.displayOff(); 
  #endif
  delay(10);

  digitalWrite(Vext, HIGH); 

  pinMode(36, OUTPUT);
  digitalWrite(36, LOW); 
  gpio_hold_en((gpio_num_t)36); 

  // =========================================================================
  // 4. STEP FOUR: PIN HOLDS & SLEEP MASK
  // =========================================================================
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