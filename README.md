# Reflex-o-meter

An IoT reflex measurement device that streams real-time IMU sensor data over Bluetooth Low Energy (BLE) from a [Seeed XIAO nRF52840 Sense](https://wiki.seeedstudio.com/XIAO_BLE/) to a Python client for logging and analysis.

## How It Works

```
┌─────────────────────┐        BLE        ┌─────────────────────┐
│   XIAO nRF52840     │  ─────────────►   │   Python Client     │
│                     │                   │                     │
│  LSM6DS3 IMU Sensor │   8 float chars   │  bleak library      │
│  - Accelerometer    │   @ 1 Hz          │  - Scan & connect   │
│  - Gyroscope        │                   │  - Read GATT chars  │
│  - Temperature      │                   │  - Write to CSV     │
└─────────────────────┘                   └─────────────────────┘
```

The device reads accelerometer (X/Y/Z), gyroscope (X/Y/Z), and temperature (C/F) from the onboard LSM6DS3 sensor, then publishes each value as a BLE GATT characteristic. The Python client discovers the device, connects, and continuously logs all 8 values to `data.txt`.

## Project Structure

```
reflex-device/       Arduino firmware — BLE + IMU (main application)
reflex-py/           Python BLE client for data collection
  recv_data.py         Main data receiver script
  experiment/          BLE scanning & connection test scripts
xiao-imu/            Standalone IMU serial demo (no BLE)
arj-code/            LED + button debounce example
arduino-libs/        Required Arduino library (LSM6DS3)
3-d models           Enclosure design files
```

## Getting Started

### Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software) or `arduino-cli`
- Seeed XIAO nRF52840 Sense board
- Python 3.x

### Firmware Setup

1. Install the **Seeeduino nRF52** board package in Arduino IDE ([guide](https://wiki.seeedstudio.com/XIAO_BLE/))
2. Install the **Seeed Arduino LSM6DS3** library (zip provided in `arduino-libs/`)
3. Open `reflex-device/reflex-device.ino`
4. Select board: **Seeed XIAO nRF52840 Sense**
5. Compile and upload

```bash
# Or via CLI:
arduino-cli compile --fqbn Seeeduino:nrf52:xiaonRF52840Sense reflex-device/
arduino-cli upload  --fqbn Seeeduino:nrf52:xiaonRF52840Sense -p <PORT> reflex-device/
```

### Python Client Setup

```bash
cd reflex-py
pip install bleak
python recv_data.py
```

The script scans for a device named **"Reflex-o-meter"**, connects, and writes CSV data to `data.txt`:

```
ax, ay, az, gx, gy, gz, c, f
0.0012, -0.0034, 1.0021, 0.42, -0.15, 0.08, 25.3, 77.5
```

## BLE Service

| Characteristic | UUID | Description |
|---|---|---|
| Accel X | `...432b40` | Accelerometer X (g) |
| Accel Y | `...432b50` | Accelerometer Y (g) |
| Accel Z | `...432b60` | Accelerometer Z (g) |
| Gyro X  | `...432b70` | Gyroscope X (dps) |
| Gyro Y  | `...432b80` | Gyroscope Y (dps) |
| Gyro Z  | `...432b90` | Gyroscope Z (dps) |
| Temp C  | `...432ba0` | Temperature (Celsius) |
| Temp F  | `...432bb0` | Temperature (Fahrenheit) |

Service UUID: `4401d1bb-95c6-461e-9579-cf6989cccddc`
All characteristics are 4-byte little-endian floats (read-only).

## References

- [Seeed XIAO nRF52840 Sense Wiki](https://wiki.seeedstudio.com/XIAO_BLE/)
- [Seeed LSM6DS3 Library](https://github.com/Seeed-Studio/Seeed_Arduino_LSM6DS3)
- [Bleak — BLE library for Python](https://github.com/hbldh/bleak)
