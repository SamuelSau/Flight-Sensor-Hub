# FlightSensorHub

Bare-metal flight sensor hub for the STM32F446RE Nucleo board. Uses FreeRTOS for deterministic 100 Hz real-time scheduling with interrupt-driven UART and ADC+DMA drivers written at the register level (CMSIS only, no HAL).

Simulated sensors: barometric altimeter and 3-axis accelerometer. Telemetry is transmitted as CRC-protected binary packets over UART2 at 115200 baud.

## Motivation

The aim of this project is to build out a flight sensor hub seen in most military devices like drones and satellites on an embedded board.

## Architecture

```
ADC1/DMA ISR ──signal──> sensor_task (100Hz, prio 6)
                              │
                         raw_sample_queue (depth 8)
                              │
                         processing_task (event-driven, prio 4)
                              │
                         processed_data_queue (depth 4)
                              │
                         telemetry_task (event-driven, prio 3)
                              │
                         UART2 TX (interrupt-driven, ring buffer)
```

| Task | Rate | Priority | Role |
|------|------|----------|------|
| sensor_task | 100 Hz (`vTaskDelayUntil`) | 6 | Triggers ADC scan, waits DMA semaphore, pushes raw data |
| processing_task | Event-driven | 4 | Converts ADC to altitude (m) and acceleration (g) via FPU |
| telemetry_task | Event-driven | 3 | Assembles CRC-16 binary packet, sends over UART2 |

### Clock Tree

HSI 16 MHz → PLL → **180 MHz SYSCLK** (overdrive enabled)

- AHB = 180 MHz
- APB1 = 45 MHz (USART2, timers)
- APB2 = 90 MHz (ADC1)

### ADC Channel Mapping

| Pin | ADC Channel | Sensor |
|-----|-------------|--------|
| PA0 | ADC1_IN0 | Barometric pressure (altimeter) |
| PA1 | ADC1_IN1 | Accelerometer X-axis |
| PA4 | ADC1_IN4 | Accelerometer Y-axis |
| PA5 | ADC1_IN5 | Accelerometer Z-axis |

### Telemetry Packet Format (38 bytes)

| Offset | Size | Field |
|--------|------|-------|
| 0 | 2 | Sync word (`0xAA55`) |
| 2 | 1 | Version |
| 3 | 1 | Reserved |
| 4 | 4 | Sequence counter |
| 8 | 4 | Timestamp (tick count) |
| 12 | 4 | Altitude (float, meters) |
| 16 | 4 | Accel X (float, g) |
| 20 | 4 | Accel Y (float, g) |
| 24 | 4 | Accel Z (float, g) |
| 28 | 2 | Raw altimeter ADC |
| 30 | 2 | Raw accel X ADC |
| 32 | 2 | Raw accel Y ADC |
| 34 | 2 | Raw accel Z ADC |
| 36 | 2 | CRC-16/CCITT-FALSE |

## Quick Start

### Toolchain

Install the ARM bare-metal cross-compiler and newlib:

**Arch Linux / CachyOS:**
```bash
sudo pacman -S arm-none-eabi-gcc arm-none-eabi-newlib arm-none-eabi-binutils
```

**Ubuntu / Debian:**
```bash
sudo apt install gcc-arm-none-eabi libnewlib-arm-none-eabi
```

**macOS (Homebrew):**
```bash
brew install arm-none-eabi-gcc
```

### Build Tools

- CMake 3.20+
- Make or Ninja

### Flash Tools (optional)

- [stlink](https://github.com/stlink-org/stlink) (`st-flash`) **or**
- [OpenOCD](https://openocd.org/)

## Usage

### Configure and Build

```bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE=arm-none-eabi-gcc.cmake
cmake --build build
```

This produces:
- `build/src/FlightSensorHub.elf` — ELF with debug symbols
- `build/src/FlightSensorHub.bin` — Raw binary for flashing
- `build/src/FlightSensorHub.map` — Linker map file

### Build Types

```bash
# Debug build (default) — includes debug symbols, -Og optimization
cmake -B build -DCMAKE_TOOLCHAIN_FILE=arm-none-eabi-gcc.cmake -DCMAKE_BUILD_TYPE=Debug

# Release build — -O2 optimization, no debug symbols
cmake -B build -DCMAKE_TOOLCHAIN_FILE=arm-none-eabi-gcc.cmake -DCMAKE_BUILD_TYPE=Release
```

### Clean Rebuild

```bash
rm -rf build
cmake -B build -DCMAKE_TOOLCHAIN_FILE=arm-none-eabi-gcc.cmake
cmake --build build
```

## Flashing

### Using st-flash

```bash
st-flash write build/src/FlightSensorHub.bin 0x08000000
```

### Using OpenOCD

```bash
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
    -c "program build/src/FlightSensorHub.elf verify reset exit"
```

## Verifying Output

Connect to the ST-Link Virtual COM Port at **115200 baud, 8N1**. The board will immediately start transmitting binary telemetry packets at 100 Hz.

### Quick Serial Check (Linux)

```bash
# Find the serial device (usually /dev/ttyACM0)
ls /dev/ttyACM*

# Raw hex dump of incoming packets
stty -F /dev/ttyACM0 115200 raw -echo
xxd /dev/ttyACM0 | head -20
```

### What to Look For

1. **Sync word**: Every 38-byte packet starts with bytes `55 AA` (little-endian `0xAA55`)
2. **Sequence counter**: Bytes 4-7 increment monotonically (0, 1, 2, ...)
3. **100 Hz rate**: ~10 ms between packets
4. **CRC integrity**: Last 2 bytes of each packet are a valid CRC-16/CCITT-FALSE over the preceding 36 bytes

### Python Packet Decoder (example)

```python
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
```

## Project Structure

```
FlightSensorHub/
├── CMakeLists.txt                        # Top-level build
├── arm-none-eabi-gcc.cmake               # Cortex-M4F toolchain
├── linker/
│   └── STM32F446RE.ld                    # 512K Flash, 128K SRAM
├── startup/
│   └── startup_stm32f446xx.c             # Vector table + Reset_Handler
├── cmsis/                                # ARM + ST CMSIS headers (vendored)
├── freertos/
│   ├── FreeRTOSConfig.h                  # 180MHz, 1kHz tick, 32KB heap
│   └── kernel/                           # FreeRTOS-Kernel source (vendored)
└── src/
    ├── main.c                            # BSP init, queue/task creation
    ├── bsp/
    │   ├── clock.c/h                     # PLL + overdrive → 180 MHz
    │   ├── gpio.c/h                      # Pin alternate functions
    │   ├── uart.c/h                      # USART2 interrupt-driven TX/RX
    │   ├── adc.c/h                       # ADC1 scan mode + DMA trigger
    │   └── dma.c/h                       # DMA2 Stream0 for ADC1
    ├── app/
    │   ├── flight_data.h                 # Data structures + packet format
    │   ├── sensor_task.c/h               # 100 Hz ADC sampling
    │   ├── processing_task.c/h           # Altitude/accel conversion
    │   └── telemetry_task.c/h            # CRC-16 packet assembly
    └── util/
        └── ring_buffer.c/h              # Lock-free SPSC ring buffer
```

## Design Notes

- **No HAL**: All peripheral access is direct register writes via CMSIS headers
- **DMA single-scan**: Sensor task triggers each conversion for deterministic timing
- **Non-blocking queues**: Samples are dropped (not blocked on) if downstream is full
- **Lock-free ring buffer**: UART ISR ↔ task communication without mutexes
- **Hardware FPU**: Cortex-M4F single-precision FPU used for altitude/acceleration math

## Contributing

To contribute to this repository or creating their own fork, ensure to install the necessary dependencies and have the required hardware to make this work. 