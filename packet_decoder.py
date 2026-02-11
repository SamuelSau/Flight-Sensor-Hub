import struct
import serial

def crc16_ccitt(data):
    crc = 0xFFFF
    for byte in data:
        crc ^= byte << 8
        for _ in range(8):
            if crc & 0x8000:
                crc = (crc << 1) ^ 0x1021
            else:
                crc <<= 1
            crc &= 0xFFFF
    return crc

ser = serial.Serial('/dev/ttyACM0', 115200)
buf = b''

while True:
    buf += ser.read(ser.in_waiting or 1)
    # Search for sync word
    idx = buf.find(b'\x55\xAA')
    if idx >= 0 and len(buf) >= idx + 38:
        pkt = buf[idx:idx+38]
        buf = buf[idx+38:]

        fields = struct.unpack('<HBBIIfffffHHHHH', pkt)
        sync, ver, _, seq, ts, alt, ax, ay, az, ra, rx, ry, rz, crc = fields

        calc_crc = crc16_ccitt(pkt[:36])
        ok = "OK" if crc == calc_crc else "BAD"

        print(f"seq={seq:6d} alt={alt:8.2f}m "
              f"accel=({ax:+.3f}, {ay:+.3f}, {az:+.3f})g "
              f"CRC={ok}")