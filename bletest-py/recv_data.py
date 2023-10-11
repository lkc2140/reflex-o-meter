import asyncio
from bleak import BleakScanner, BleakClient

TARGET_DEVICE_NAME = "Reflex-o-meter"  # The name you set in the Arduino code
OUTPUT_FILE = "data.txt"

IMU_X_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b40"
IMU_Y_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b50"
IMU_Z_UUID = "7b53dc33-a8ca-41af-a269-2eb2bb432b60"

async def run():
    devices = await BleakScanner.discover(10)
    target_device = None

    for device in devices:
        if device.name is not None and TARGET_DEVICE_NAME in device.name:
            target_device = device
            break

    if not target_device:
        print(f"Device with name {TARGET_DEVICE_NAME} not found!")
        return

    async with BleakClient(target_device.address) as client:
        while True:
            x_value = float(await client.read_gatt_char(IMU_X_UUID))
            y_value = float(await client.read_gatt_char(IMU_Y_UUID))
            z_value = float(await client.read_gatt_char(IMU_Z_UUID))
            
            with open(OUTPUT_FILE, "a") as f:
                f.write(f"{x_value}, {y_value}, {z_value}\n")

            await asyncio.sleep(1)

loop = asyncio.get_event_loop()
loop.run_until_complete(run())
