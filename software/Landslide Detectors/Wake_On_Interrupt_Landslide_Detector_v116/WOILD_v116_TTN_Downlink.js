/**
 * WOILD v1.1.6 LoRaWAN Downlink Formatter
 * Repository: Modular Open Source Science Station
 * Decoding: 2-Byte Packed Binary Downlink
 */

function encodeDownlink(input) {
  var data = input.data || {};
  var bytes = [];

  // 1. Default fallback values if fields are omitted
  var mot_lsb   = (data.motion_threshold_lsb !== undefined)   ? (data.motion_threshold_lsb & 0x07)   : 1;
  var sw_lsb    = (data.software_threshold_lsb !== undefined) ? (data.software_threshold_lsb & 0x07) : 2;
  var dur_idx   = (data.duration_index !== undefined)         ? (data.duration_index & 0x07)         : 3;
  var delay_idx = (data.minimum_delay_index !== undefined)    ? (data.minimum_delay_index & 0x03)    : 2;
  var trip_idx  = (data.trip_alert_index !== undefined)       ? (data.trip_alert_index & 0x03)       : 2;

  // Extract use_tripwire boolean (defaults to true/1 if omitted)
  var trip_bit = 1;
  if (data.use_tripwire !== undefined) {
    trip_bit = (data.use_tripwire === true || data.use_tripwire === 1 || data.use_tripwire === "true") ? 1 : 0;
  }

  // 2. Byte 0: Motion LSB [Bits 0-2] | Software LSB [Bits 3-5]
  bytes[0] = (mot_lsb & 0x07) | ((sw_lsb & 0x07) << 3);

  // 3. Byte 1: Duration [Bits 0-2] | Tripwire Flag [Bit 3] | Routine Delay [Bits 4-5] | Trip Alert Delay [Bits 6-7]
  bytes[1] = (dur_idx & 0x07) | (trip_bit << 3) | ((delay_idx & 0x03) << 4) | ((trip_idx & 0x03) << 6);

  return {
    bytes: bytes,
    fPort: 1
  };
}

function decodeDownlink(input) {
  var bytes = input.bytes;
  var data = {};

  if (bytes && bytes.length >= 2) {
    // Reconstruct Byte 0 fields
    data.motion_threshold_lsb   = bytes[0] & 0x07;
    data.software_threshold_lsb = (bytes[0] >> 3) & 0x07;

    // Reconstruct Byte 1 fields
    data.duration_index         = bytes[1] & 0x07;
    data.use_tripwire           = ((bytes[1] >> 3) & 0x01) === 1;
    data.minimum_delay_index    = (bytes[1] >> 4) & 0x03;
    data.trip_alert_index       = (bytes[1] >> 6) & 0x03;
  }

  return {
    data: data
  };
}