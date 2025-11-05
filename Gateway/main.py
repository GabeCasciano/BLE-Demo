#!/usr/bin/env python3

import asyncio
from bleak import BleakScanner, BleakClient

UUID_PHOTO_CHAR = "dbbb06b1-1b2a-4a42-ac4e-6c64a9d88961"
UUID_LED_CHAR = "dbbb06b1-1b2a-4a42-ac4e-6c64a9d88965"
UUID_AIN_CHAR = "dbbb06b1-1b2a-4a42-ac4e-6c64a9d88963"
UUID_BTN_CHAR = "dbbb06b1-1b2a-4a42-ac4e-6c64a9d88964"

async def main():
    print("Scanning for BLE devices...")
    devices = await BleakScanner.discover(timeout=5)

    for i, d in enumerate(devices):
        print(f"{i}: {d.name} ({d.address})")

    target = input("\nEnter the number of the device to connect: ")
    target_device = devices[int(target)]

    async with BleakClient(target_device.address) as client:
        print(f"Connected: {client.is_connected}")

        while client.is_connected:
            try:
                value = await client.read_gatt_char(UUID_PHOTO_CHAR)
                print(f"Value from {UUID_PHOTO_CHAR}: {int(value[0])}")

                value = await client.read_gatt_char(UUID_AIN_CHAR)
                print(f"Value from {UUID_AIN_CHAR}: {int(value[0])}")

                value = await client.read_gatt_char(UUID_BTN_CHAR)
                print(f"Value from {UUID_BTN_CHAR}: {bool(value[0])}")

            except Exception as e:
                print(f"Failed to read characteristic: {e}")

    print("Disconnected.")

if __name__ == "__main__":
    asyncio.run(main())
