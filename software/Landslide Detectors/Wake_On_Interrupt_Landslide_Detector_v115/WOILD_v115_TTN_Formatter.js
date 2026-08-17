/**
 * WOILD v1.1.5 LoRaWAN Payload Formatter
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

  // Duration lookup map (ms) matching C++ DURATION_LOOKUP_MS
  var duration_lookup_ms = [1, 3, 5, 7, 9, 11, 13, 15];

  // Byte 0: Nonce
  var random_byte = bytes[0];

  // Byte 1: Motion (3b) | SW (3b) | Duration Index Bottom 2 Bits (2b)
  var cfg_motion_th = bytes[1] & 0x07;
  var cfg_sw_th = (bytes[1] >> 3) & 0x07;
  var dur_low2 = (bytes[1] >> 6) & 0x03;

  // Byte 2: Battery Voltage: (V - 2.0) * 100
  var calc_volt = (bytes[2] / 100.0) + 2.0;

  // Byte 3, 4, 5: 9-bit Signed Angles, System State, Duration Top Bit, 3-bit Vector
  var x_upper = bytes[3];
  var y_upper = bytes[4];
  var shared_byte5 = bytes[5];

  var x_lsb = shared_byte5 & 0x01;
  var y_lsb = (shared_byte5 >> 1) & 0x01;
  var stateMode = (shared_byte5 >> 2) & 0x03;
  var dur_top1 = (shared_byte5 >> 4) & 0x01;
  var vector_3bit = (shared_byte5 >> 5) & 0x07;

  // Reconstruct 3-bit Duration Index
  var dur_index = (dur_top1 << 2) | dur_low2;
  var cfg_dur_ms = duration_lookup_ms[dur_index];

  // Reconstruct 9-Bit Signed Angles (-256 to +255)
  var x_9bit = (x_upper << 1) | x_lsb;
  var y_9bit = (y_upper << 1) | y_lsb;

  // Convert 9-bit unsigned integer to signed integer
  if (x_9bit & 0x0100) { x_9bit |= 0xFE00; }
  if (y_9bit & 0x0100) { y_9bit |= 0xFE00; }

  var x_int16 = (x_9bit << 16) >> 16;
  var y_int16 = (y_9bit << 16) >> 16;

  var calc_x = x_int16 / 10.0;
  var calc_y = y_int16 / 10.0;

  // Byte 6: DHT22 Humidity
  var raw_hum = bytes[6];

  // Byte 7: MPU Core Temp
  var calc_temp = (bytes[7] / 4.0) - 10.0;

  // Vector mg conversion (8 mg per LSB)
  var peak_vector_mg = vector_3bit * 8;

  var statusTextEn = "unknown";
  var statusTextEs = "desconocido";
  if (stateMode === 0) { statusTextEn = "timed"; statusTextEs = "programado"; }
  else if (stateMode === 1) { statusTextEn = "alert"; statusTextEs = "alerta"; }
  else if (stateMode === 3) { statusTextEn = "tripwire"; statusTextEs = "cable roto"; }

  var devId = (input && input.variables && input.variables.device_id)
    || (input && input.device_id)
    || "LD01";

  return {
    data: {
      sensor_id: devId.toUpperCase(),
      nonce: parseInt(random_byte),
      // Configuration Confirmation Echo
      config_motion_threshold_mg: cfg_motion_th * 32,
      config_software_threshold_mg: cfg_sw_th * 32,
      config_motion_duration_ms: cfg_dur_ms,
      config_duration_index: dur_index,
      // Sensor Telemetry
      battery_voltage: parseFloat(calc_volt.toFixed(2)),
      humidity: parseInt(raw_hum),
      angle_x: parseFloat(calc_x.toFixed(1)),
      angle_y: parseFloat(calc_y.toFixed(1)),
      peak_dynamic_vector_mg: parseInt(peak_vector_mg),
      temperature: parseFloat(calc_temp.toFixed(2)),
      alert_active: (stateMode === 1 || stateMode === 3),
      system_state: parseInt(stateMode),
      system_status: statusTextEn,
      system_status_es: statusTextEs
    }
  };
}