/**
 * WOILD v1.1.5 LoRaWAN Downlink Formatter
 * Repository: Modular Open Source Science Station
 * Decoding: 2-Byte Packed Binary Downlink
 */

function encodeDownlink(input) {
var data = input.data;
var bytes = [];
// Default fallback values if fields are omitted
var mot_lsb = (data.motion_threshold_lsb !== undefined) ? (data.motion_threshold_lsb & 0x07) : 1;
var sw_lsb = (data.software_threshold_lsb !== undefined) ? (data.software_threshold_lsb & 0x07) : 2;
var dur_idx = (data.duration_index !== undefined) ? (data.duration_index & 0x07) : 3;
// Byte 0: Motion LSB [2:0] | Software LSB [5:3]
bytes[0] = mot_lsb | (sw_lsb << 3);
// Byte 1: Duration Index [2:0]
bytes[1] = dur_idx;
return {
bytes: bytes,
fPort: 1
};
}