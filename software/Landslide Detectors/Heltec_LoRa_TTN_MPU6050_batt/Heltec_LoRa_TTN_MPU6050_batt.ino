/**
 * This sketch was written starting with the "LoRAWAN_TTN" example 
 * from the "Heltec_ESP32_LoRa_V3" Library. It also uses portions of the 
 * "GetAllData" example from the "MPU6050_light" Library. 
 * Modifications have been made to both contributions by Brandon J. Winters.
 * 
 * 
 * FOR THIS EXAMPLE TO WORK, YOU MUST INSTALL THE "LoRaWAN_ESP32" LIBRARY USING
 * THE LIBRARY MANAGER IN THE ARDUINO IDE.
 * 
 * This code will send a two-byte LoRaWAN message every 15 minutes. The first
 * byte is a simple 8-bit counter, the second is the ESP32 chip temperature
 * directly after waking up from its 15 minute sleep in degrees celsius + 100.
 *
 * If your NVS partition does not have stored TTN / LoRaWAN provisioning
 * information in it yet, you will be prompted for them on the serial port and
 * they will be stored for subsequent use.
 *
 * See https://github.com/ropg/LoRaWAN_ESP32

 Get all possible data from MPU6050
 * Accelerometer values are given as multiple of the gravity [1g = 9.81 m/s²]
 * Gyro values are given in deg/s
 * Angles are given in degrees
 * Note that X and Y are tilt angles and not pitch/roll.
 *
 * License: MIT
 */

// Pause between sends in seconds, so this is every 15 minutes. (Delay will be
// longer if regulatory or TTN Fair Use Policy requires it.)
#define MINIMUM_DELAY 335 //spreads 256 transmissions over about 24 hours 

#include <heltec_unofficial.h>
#include <LoRaWAN_ESP32.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <MPU6050_light.h>
//#include "Timer.h"
#include "DHT.h"
//#include <Arduino.h>

//Timer timer;

LoRaWANNode* node;

extern TwoWire Wire1; //Setup Wire1 as second I2C bus

//create LSM6DSOX sensor and give it the nickname "sox"
MPU6050 mpu(Wire1);

#define VBAT_Read    1
#define	ADC_Ctrl    37
#define DHTPIN 45     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);

//Setup sequatial counting for data transmissions
RTC_DATA_ATTR uint8_t count = 0; //for LoRa transmissions
RTC_DATA_ATTR uint8_t trans = 0; //for serial debugging

void setup() {
  // put your setup code here, to run once:
 Wire1.begin(41, 42);

 Serial.begin(115200); //begin serial communication at the defined baud rate


// ============ TEST MODE ============
/** ## **How to Use:**

### For Live Testing:
1. Upload the modified code
2. Open Serial Monitor (115200 baud)
3. Press RST button
4. Immediately hold BOOT (PRG) button** for 3 seconds
5. You'll see:
```
   === ACCELEROMETER TEST MODE ===
   Press RESET to exit
   
   AccelMag: 1005.3 mg  |  Angles -> X: 0.50°  Y: -1.23°  Z: 0.00°
   AccelMag: 1008.1 mg  |  Angles -> X: 0.52°  Y: -1.21°  Z: 0.01°
```

6. Tilt, shake, move the board and watch values update in real-time!

###For Normal Operation:
1. Press **RESET** button
2. **Don't press BOOT** (or release it quickly)
3. Normal landslide detection code runs
*/

  Serial.println("\n=== BOOT BUTTON CHECK ===");
  Serial.println("Hold BOOT button for 3 seconds to enter test mode...");
  
  pinMode(0, INPUT_PULLUP);  // BOOT button is GPIO 0
  
  // Check if BOOT button is held
  bool testMode = true;
  for(int i = 0; i < 30; i++) {  // Check for 3 seconds
    if(digitalRead(0) == HIGH) {
      testMode = false;
      break;
    }
    delay(100);
  }
  
  if(testMode) {
    Serial.println("\n=== ACCELEROMETER TEST MODE ===");
    Serial.println("Press RESET to exit\n");
    
    byte status = mpu.begin();
    Serial.print("MPU6050 status: ");
    Serial.println(status);
    
    if(status == 0) {
      while(true) {  // Infinite test loop
        mpu.update();
        
        // Calculate acceleration magnitude
        float ax = mpu.getAccX() * 9.81;  // Convert to m/s²
        float ay = mpu.getAccY() * 9.81;
        float az = mpu.getAccZ() * 9.81;
        float accelMag = sqrt(ax*ax + ay*ay + az*az) * 1000;  // Convert to mg
        
        Serial.print("AccelMag: ");
        Serial.print(accelMag, 1);
        Serial.print(" mg  |  Angles -> X: ");
        Serial.print(mpu.getAngleX(), 2);
        Serial.print("°  Y: ");
        Serial.print(mpu.getAngleY(), 2);
        Serial.print("°  Z: ");
        Serial.print(mpu.getAngleZ(), 2);
        Serial.println("°");
        
        delay(200);  // 5 readings per second
      }
    } else {
      Serial.println("MPU6050 not found! Check wiring.");
      while(true) { delay(1000); }
    }
  }
  
  Serial.println("Starting normal operation...\n");
//=========End Test loop code//


 // timer.start();
 // if(timer.state() == RUNNING) Serial.println("timer running");
  //delay(1000);

  byte status = mpu.begin();
  //mpu.begin(0x68, &Wire1);
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  //Serial.println(F("Calculating offsets, do not move MPU6050"));
  //delay(1000);
  //mpu.calcOffsets(true,true); // gyro and accelero
  //Serial.println("Done!\n");

  pinMode(ADC_Ctrl,OUTPUT);
  pinMode(VBAT_Read,INPUT);
  //adcAttachPin(VBAT_Read);
  analogReadResolution(12);

// ADC resolution
  const int resolution = 12;
  const int adcMax = pow(2,resolution) - 1;
  const float adcMaxVoltage = 3.3;
  // On-board voltage divider
  const int R1 = 390;
  const int R2 = 100;
  // Calibration measurements
  const float measuredVoltage = 4.2;
  const float reportedVoltage = 4.095;
  // Calibration factor
  const float factor = (adcMaxVoltage / adcMax) * ((R1 + R2)/(float)R2) * (measuredVoltage / reportedVoltage); 
  digitalWrite(ADC_Ctrl,LOW);
  delay(100);
  int analogValue = analogRead(VBAT_Read);
  digitalWrite(ADC_Ctrl,HIGH);

  float floatVoltage = factor * analogValue;
  uint16_t voltage = (int)(floatVoltage * 1000.0);

  dht.begin(); 

  Serial.println("Continuing to data collection.");

 delay(100);
 
  //Reading battery voltage from jst connected Li-Ion battery
  //readBatteryVoltage();
  uint8_t rawb = (analogRead(VBAT_Read) / 4);
  //uint16_t anab = (analogValue);

 // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  uint8_t hum = dht.readHumidity();
  // Read temperature as Celsius (the default)
  //uint8_t tempc = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //uint8_t tempf = dht.readTemperature(true);

 mpu.update();

  //uint8_t xacc = mpu.getAccX();
  //uint8_t yacc = mpu.getAccY();
  //uint8_t zacc = mpu.getAccZ();
  uint16_t xang = (sq(mpu.getAngleX()) * 10000);
  uint16_t yang = (sq(mpu.getAngleY()) * 10000);
  uint16_t zang = (sq(mpu.getAngleZ()) * 10000);

//Tracks the number of consecutive serial transmissions
  Serial.println();  
  Serial.println(F("-------------------------"));
  Serial.print("Consecutive Transmissions: ");
  Serial.print(trans++);
  Serial.println();

  Serial.print("Raw Reading: ");
  Serial.print(analogRead(VBAT_Read)); //rough adjustment to provide actual battery voltage
  Serial.println(" ");
/*
  //Print battery voltage to serial
  Serial.print("Analog Reading: ");
  Serial.print(analogValue);
  Serial.println(" ");

  Serial.print("Battery Voltage: ");
  Serial.print(voltage);
  Serial.println(" mV");
*/
//Values printed to serial output from the MPU6050
  Serial.print(F("Humidity: "));Serial.print(hum);Serial.println(" %");
  //Serial.print(F("ACCELERO  X: "));Serial.print(mpu.getAccX());
  //Serial.print("\tY: ");Serial.print(mpu.getAccY());
  //Serial.print("\tZ: ");Serial.println(mpu.getAccZ());
  
  //Serial.print(F("GYRO      X: "));Serial.print(mpu.getGyroX());
  //Serial.print("\tY: ");Serial.print(mpu.getGyroY());
  //Serial.print("\tZ: ");Serial.println(mpu.getGyroZ());
  
  //Serial.print(F("ACC ANGLE X: "));Serial.print(mpu.getAccAngleX());
  //Serial.print("\tY: ");Serial.println(mpu.getAccAngleY());
    
  Serial.print(F("ANGLE     X: "));Serial.print(mpu.getAngleX());
  Serial.print("\tY: ");Serial.print(mpu.getAngleY());
  Serial.print("\tZ: ");Serial.println(mpu.getAngleZ());
  Serial.println(F("=====================================================\n"));

 heltec_setup();

 // initialize radio
  Serial.println("Radio init");
  int16_t state = radio.begin();
  if (state != RADIOLIB_ERR_NONE) {
    Serial.println("Radio did not initialize. We'll try again later.");
    goToSleep();
  }

  node = persist.manage(&radio);

  if (!node->isActivated()) {
    Serial.println("Could not join network. We'll try again later.");
    goToSleep();
  }

// If we're still here, it means we joined, and we can send something

// Manages uplink intervals to the TTN Fair Use Policy
  node->setDutyCycle(true, 1250);

//create LoRa data uplink structure and packet
  uint8_t uplinkData[8];
  uplinkData[0] = (rawb); //sends the raw analog read value for battery voltage
  uplinkData[1] = (hum); //Send humidity value from DHT22 to monitor moisture in the box
  uplinkData[2] = highByte(xang); //Package 16 bit, 2 byte angle value
  uplinkData[3] = lowByte(xang);
  uplinkData[4] = highByte(yang); //Package 16 bit, 2 byte angle value
  uplinkData[5] = lowByte(yang);
  uplinkData[6] = highByte(zang); //Package 16 bit, 2 byte angle value
  uplinkData[7] = lowByte(zang); 

//define LoRa downlink size
  uint8_t downlinkData[256];
  size_t lenDown = sizeof(downlinkData);

//Send of the LoRa data and establish wait for downlink
  state = node->sendReceive(uplinkData, sizeof(uplinkData), 1, downlinkData, &lenDown);

  if(state == RADIOLIB_ERR_NONE) {
    Serial.println("Message sent, no downlink received.");
  } else if (state > 0) {
    Serial.println("Message sent, downlink received.");
  } else {
    Serial.printf("sendReceive returned error %d, we'll try again later.\n", state);
  }

//timer.stop();
  //if(timer.state() == STOPPED) Serial.println("timer stopped");
  //Serial.print("time elapsed ms: ");
  //Serial.println(timer.read());

  goToSleep();    // Does not return, program starts over next round

}

void loop() 
{
  // put your main code here, to run repeatedly:

}

//Read Battery Voltage
//void readBatteryVoltage() {
  
//}

void goToSleep() {
  Serial.println("Going to deep sleep now");
  // allows recall of the session after deepsleep
  persist.saveSession(node);
  // Calculate minimum duty cycle delay (per FUP & law!)
  uint32_t interval = node->timeUntilUplink();
  // And then pick it or our MINIMUM_DELAY, whichever is greater
  uint32_t delayMs = max(interval, (uint32_t)MINIMUM_DELAY * 1000);
  Serial.printf("Next TX in %i s\n", delayMs/1000);
    Serial.println(F("*****-----*-----*-----*-----*-----*-----*-----*-----*-----*-----*-----*****"));
  Serial.println();
  delay(100);  // So message prints
  // and off to bed we go
  heltec_deep_sleep(delayMs/1000);
}

/*
The Javascript payload formatter for TTN is as follows:

function decodeUplink(input) {
  var rawb = input.bytes[0];
  var hum = (input.bytes[1]); //readjusted from the math done in Arduino
  var Xang = (input.bytes[2] << 8) | input.bytes[3];
  var Yang = (input.bytes[4] << 8) | input.bytes[5];
  var Zang = (input.bytes[6] << 8) | input.bytes[7];
  //var Yacc = (input.bytes[3] / 100);
  //var Zacc = (input.bytes[4] / 100);
  //var Xang = (input.bytes[5] / 100);
  //var Yang = (input.bytes[6] / 100);
  //var Zang = (input.bytes[7] / 100);

  return {
    data: { 
     L06_Raw: (rawb * 4),
     L06_Humidity: hum,
     //L03_Xaccel: ((Xacc) * 9.82),
     //L03_Yaccel: ((Yacc) * 9.82),
     //L03_Zaccel: ((Zacc) * 9.82),
     L06_Xangle: (Xang),
     L06_Yangle: (Yang),
     L06_Zangle: (Zang),
    },
  };
}

*/
