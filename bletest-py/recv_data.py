import asyncio
from bleak import BleakScanner, BleakClient
import struct

TARGET_DEVICE_NAME = "Reflex-o-meter"  # The name you set in the Arduino code
OUTPUT_FILE = "data.txt"

IMU_X_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b40"
IMU_Y_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b50"
IMU_Z_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b60"

async def run():
    devices = await BleakScanner.discover(10)
    target_device = None

    print("Found %d devices" % len(devices))
    for device in devices:
        print(device.name, device.address)

    for device in devices:
        if device.name is not None and TARGET_DEVICE_NAME in device.name:
            target_device = device
            break

    if not target_device:
        print(f"Device with name {TARGET_DEVICE_NAME} not found!")
        return
    else:
        print(f"Found device {TARGET_DEVICE_NAME} with address {target_device.address}")

    print("Connecting to device...")
    async with BleakClient(target_device.address) as client:
        print("Reading data...")
        while True:
            x_data = await client.read_gatt_char(IMU_X_UUID)
            y_data = await client.read_gatt_char(IMU_Y_UUID)
            z_data = await client.read_gatt_char(IMU_Z_UUID)

            x_value = struct.unpack('f', x_data)[0]
            y_value = struct.unpack('f', y_data)[0]
            z_value = struct.unpack('f', z_data)[0]
            
            with open(OUTPUT_FILE, "a") as f:
                f.write(f"{x_value}, {y_value}, {z_value}\n")
                print(f"{x_value}, {y_value}, {z_value}")

            await asyncio.sleep(1)

loop = asyncio.get_event_loop()
loop.run_until_complete(run())
