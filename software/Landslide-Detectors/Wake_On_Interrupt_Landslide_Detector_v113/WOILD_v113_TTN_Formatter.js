{\rtf1\ansi\ansicpg1252\cocoartf2822
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\margl1440\margr1440\vieww11520\viewh8400\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural\partightenfactor0

\f0\fs24 \cf0 function decodeUplink(input) \{\
  var bytes = input.bytes;\
\
  // Validation: Check for structural alignment with the new 8-byte payload\
  if (!bytes || bytes.length !== 8) \{\
    return \{\
      errors: ["Payload size mismatch. Expected 8 bytes, received " + (bytes ? bytes.length : 0)]\
    \};\
  \}\
\
  // 1. Byte 0 & Byte 1: Nonce and Remapped Battery Math\
  var random_byte = bytes[0];\
  var calc_volt = (bytes[1] / 100.0) + 2.0;\
\
  // 2. Byte 2: DHT22 Humidity\
  var raw_hum = bytes[2];\
\
  // 3. Bytes 3, 4, 5: Reconstruct 12-bit Signed Delta Angles\
  var x_upper = bytes[3];\
  var y_upper = bytes[4];\
  var shared_lower = bytes[5];\
\
  var x_lower = shared_lower & 0x0F;\
  var y_lower = (shared_lower >> 4) & 0x0F;\
\
  var x_raw = (x_upper << 4) | x_lower;\
  var y_raw = (y_upper << 4) | y_lower;\
\
  // Handle Two's Complement Sign Extension for 12-bit signed integers\
  if (x_raw & 0x800) \{ x_raw |= 0xF000; \}\
  if (y_raw & 0x800) \{ y_raw |= 0xF000; \}\
\
  // Cast to 16-bit signed values and decode resolution factor (0.01\'b0)\
  var x_int16 = (x_raw << 16) >> 16;\
  var y_int16 = (y_raw << 16) >> 16;\
  \
  var calc_x = x_int16 / 100.0;\
  var calc_y = y_int16 / 100.0;\
\
  // 4. Byte 6: Split Byte (System State & Peak Dynamic Vector)\
  var shared_state_byte = bytes[6];\
  var stateMode = shared_state_byte & 0x03;\
  var peak_vector = (shared_state_byte >> 2) & 0x3F;\
\
  // Map state strings to perfectly mirror previous system rules\
  var statusText = "unknown";\
  if (stateMode === 0) statusText = "timed";\
  else if (stateMode === 1) statusText = "alert";\
  else if (stateMode === 3) statusText = "tripwire"; // Dynamic expansion for the wire configuration\
\
  // 5. Byte 7: Remapped MPU Core Temperature Math\
  var calc_temp = (bytes[7] / 4.0) - 10.0;\
\
  return \{\
    data: \{ \
      sensor_id: "LD01", \
      nonce: parseInt(random_byte), \
      battery_voltage: parseFloat(calc_volt.toFixed(2)), \
      humidity: parseInt(raw_hum),\
      angle_x: parseFloat(calc_x.toFixed(2)),  \
      angle_y: parseFloat(calc_y.toFixed(2)),\
      peak_dynamic_vector_mg: parseInt(peak_vector),\
      temperature: parseFloat(calc_temp.toFixed(2)), // Direct-silicon core temp output for HA calibration\
      alert_active: (stateMode === 1 || stateMode === 3),\
      system_state: parseInt(stateMode),  \
      system_status: statusText\
    \}\
  \};\
\}}