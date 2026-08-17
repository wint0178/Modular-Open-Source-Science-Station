/**
 * WOILD v1.1.6 LoRaWAN Payload Formatter
 * Repository: Modular Open Source Science Station
 * Decoding: 8-Byte Packed Binary Uplink
 */

function decodeUplink(input) {
  var bytes = input.bytes;

  if (!bytes || bytes.length !== 8) {
    return {
      errors: ["Payload size mismatch. Expected 8 bytes, received " + (bytes ? bytes.length : 0)]
    };
  }

  // 1. Byte 0: Nonce
  var random_byte = bytes[0];

  // 2. Extract Configuration Indices & Convert to Physical Units
  var cfg_motion_idx = bytes[1] & 0x07;                // Bits 0-2 of Byte 1
  var cfg_sw_idx     = (bytes[1] >> 3) & 0x07;         // Bits 3-5 of Byte 1
  var dur_low2       = (bytes[1] >> 6) & 0x03;         // Bits 6-7 of Byte 1
  var dur_top1       = (bytes[5] >> 4) & 0x01;         // Bit 4 of Byte 5
  var cfg_dur_idx    = (dur_top1 << 2) | dur_low2;     // Reconstruct 3-bit Index (0-7)

  // Physical Conversions
  var DURATION_LOOKUP_MS = [1, 3, 5, 7, 9, 11, 13, 15];
  var motion_mg_val = cfg_motion_idx * 32;
  var software_mg_val = cfg_sw_idx * 32;
  var duration_ms_val = DURATION_LOOKUP_MS[cfg_dur_idx & 0x07];

  // 3. Byte 2: Battery Voltage
  var raw_vbat  = bytes[2];
  var calc_volt = (raw_vbat / 100.0) + 2.0;

  // 4. Bytes 3, 4, 5: 9-bit Signed Angles (0.1 degree resolution)
  var x_upper = bytes[3];
  var y_upper = bytes[4];
  var shared_byte = bytes[5];

  var x_lsb = shared_byte & 0x01;
  var y_lsb = (shared_byte >> 1) & 0x01;

  var x_raw = (x_upper << 1) | x_lsb; // Reconstruct 9-bit
  var y_raw = (y_upper << 1) | y_lsb;

  // Sign extension for 9-bit signed integers (-256 to +255)
  if (x_raw & 0x100) { x_raw |= 0xFE00; }
  if (y_raw & 0x100) { y_raw |= 0xFE00; }

  var x_int16 = (x_raw << 16) >> 16;
  var y_int16 = (y_raw << 16) >> 16;
  
  var calc_x = x_int16 / 10.0; // Scaled by 10 for 0.1° resolution
  var calc_y = y_int16 / 10.0;

  // 5. Byte 5: System State & Dynamic Vector
  var stateMode   = (shared_byte >> 2) & 0x03; // Bits 2-3 of Byte 5
  var vector_3bit = (shared_byte >> 5) & 0x07; // Bits 5-7 of Byte 5
  var peak_vector_mg = vector_3bit * 8;         // Decoded step size (0, 8, 16, 24... 56 mg)

  var statusTextEn = "Unknown";
  var statusTextEs = "Desconocido";

  if (stateMode === 0) { 
    statusTextEn = "Timed"; 
    statusTextEs = "Programado"; 
  } else if (stateMode === 1) { 
    statusTextEn = "Alert"; 
    statusTextEs = "Alerta"; 
  } else if (stateMode === 2) { 
    statusTextEn = "Tripwire"; 
    statusTextEs = "Cable Roto"; 
  } else if (stateMode === 3) { 
    statusTextEn = "Reserved"; 
    statusTextEs = "Reservado"; 
  }

  // 6. Byte 6: DHT22 Relative Humidity
  var raw_hum = bytes[6];

  // 7. Byte 7: MPU Core Temperature (6 bits) & Configuration Flags (2 bits)
  var raw_temp  = bytes[7] & 0x3F;              // Lower 6 bits (Bits 0-5): 0°C to 63°C
  var flag_bit6 = (bytes[7] >> 6) & 0x01;      // Bit 6: USE_TRIPWIRE active flag
  var flag_bit7 = (bytes[7] >> 7) & 0x01;      // Bit 7 reserved for future expansion

  var calc_temp = raw_temp;                    // 1°C resolution

  var devId = (input && input.variables && input.variables.device_id) 
           || (input && input.device_id) 
           || "test-01";

  return {
    data: { 
      sensor_id: devId.toUpperCase(), 
      nonce: parseInt(random_byte), 
      
      // Configuration Indices (0-7 for Home Assistant Sliders/Selectors)
      config_motion_index: parseInt(cfg_motion_idx),
      config_software_index: parseInt(cfg_sw_idx),
      config_duration_index: parseInt(cfg_dur_idx),

      // Decoded Physical Values (For Display Gauges)
      config_motion_mg: parseInt(motion_mg_val),
      config_software_mg: parseInt(software_mg_val),
      config_duration_ms: parseInt(duration_ms_val),

      // Environmental Telemetry
      humidity: parseInt(raw_hum),
      battery_voltage: parseFloat(calc_volt.toFixed(2)), 
      temperature: parseInt(calc_temp),
      
      // Hardware Feature Flags
      use_tripwire: (flag_bit6 === 1),
      
      // Angles (0.1 degree resolution)
      angle_x: parseFloat(calc_x.toFixed(1)),  
      angle_y: parseFloat(calc_y.toFixed(1)),
      peak_dynamic_vector_mg: parseInt(peak_vector_mg),
      
      // System States
      alert_active: (stateMode === 1 || stateMode === 2),
      system_state: parseInt(stateMode),  
      system_status: statusTextEn,
      system_status_es: statusTextEs
    }
  };
}