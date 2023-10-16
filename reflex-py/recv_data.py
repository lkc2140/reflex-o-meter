import asyncio
from bleak import BleakScanner, BleakClient
import struct

TARGET_DEVICE_NAME = "Reflex-o-meter"  # The name you set in the Arduino code
OUTPUT_FILE = "data.txt"

IMU_AX_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b40"
IMU_AY_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b50"
IMU_AZ_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b60"
IMU_GX_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b70"
IMU_GY_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b80"
IMU_GZ_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b90"
IMU_C_UUID  = "7b53dc33-a8ca-41af-a269-2eb2bb432ba0"
IMU_F_UUID  = "7b53dc33-a8ca-41af-a269-2eb2bb432bb0"

WAIT_TIME = 10

async def run():
    print("Waiting %ds for device advertisements..." % WAIT_TIME)
    devices = await BleakScanner.discover(timeout=WAIT_TIME)
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
        with open(OUTPUT_FILE, "a") as f:
            f.write(f"ax, ay, az, gx, gy, gz, c, f\n")
            print(f"ax, ay, az, gx, gy, gz, c, f")
        while True:
            ax_data = await client.read_gatt_char(IMU_AX_UUID)
            ay_data = await client.read_gatt_char(IMU_AY_UUID)
            az_data = await client.read_gatt_char(IMU_AZ_UUID)
            gx_data = await client.read_gatt_char(IMU_GX_UUID)
            gy_data = await client.read_gatt_char(IMU_GY_UUID)
            gz_data = await client.read_gatt_char(IMU_GZ_UUID)
            c_data = await client.read_gatt_char(IMU_C_UUID)
            f_data = await client.read_gatt_char(IMU_F_UUID)

            ax_value = struct.unpack('f', ax_data)[0]
            ay_value = struct.unpack('f', ay_data)[0]
            az_value = struct.unpack('f', az_data)[0]
            gx_value = struct.unpack('f', gx_data)[0]
            gy_value = struct.unpack('f', gy_data)[0]
            gz_value = struct.unpack('f', gz_data)[0]
            c_value = struct.unpack('f', c_data)[0]
            f_value = struct.unpack('f', f_data)[0]
            
            with open(OUTPUT_FILE, "a") as f:
                f.write(f"{ax_value}, {ay_value}, {az_value}, {gx_value}, {gy_value}, {gz_value}, {c_value}, {f_value}\n")
                print(f"{ax_value}, {ay_value}, {az_value}, {gx_value}, {gy_value}, {gz_value}, {c_value}, {f_value}")

            await asyncio.sleep(1)

loop = asyncio.get_event_loop()
loop.run_until_complete(run())
