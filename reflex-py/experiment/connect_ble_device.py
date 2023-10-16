import argparse
import asyncio

from bleak import BleakScanner
from bleak import BleakClient

async def discover_ble_devices(use_bdaddr):
    print("scanning for 5 seconds, please wait...")
    devices = await BleakScanner.discover(
        return_adv=True, cb=dict(use_bdaddr=use_bdaddr)
    )
    for d, a in devices.values():
        print(f"\n{d}\n{'-' * len(str(d))}\n{a}")    
    
    return devices

async def main(args: argparse.Namespace):
    devices = await discover_ble_devices(args.macos_use_bdaddr)

    # Prompt the user to select a device
    device_str = None
    while not device_str:
        device_str = input("Select a device: ")
        if device_str not in devices:
            print("Device not found. Try again.")
            device_str = None
            # Scan for the devices again
            devices = await discover_ble_devices(args.macos_use_bdaddr)
    
    # Connect to the device
    print(f"Connecting to {device_str}...")
    CHARACTERISTIC_UUID = "0000ffe1-0000-1000-8000-00805f9b34fb"
    device = devices[device_str][0]
    async with BleakClient(device) as client:
        await client.connect()
        await client.pair()
        await client.write_gatt_char(CHARACTERISTIC_UUID, bytearray([0x01, 0x00]))
        await client.disconnect()

if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    parser.add_argument(
        "--macos-use-bdaddr",
        action="store_true",
        help="when true use Bluetooth address instead of UUID on macOS",
    )

    args = parser.parse_args()

    asyncio.run(main(args))