#!/usr/bin/env python3

import argparse
import signal
import sys

import paho.mqtt.client as mqtt

import asyncio

from paho.mqtt.enums import CallbackAPIVersion
from bleak import BleakScanner, BleakClient

UUID_PHOTO_CHAR = "00000000-0919-1997-2025-000000000001"
UUID_LED_CHAR = "00000000-0919-1997-2025-000000000002"
UUID_AIN_CHAR = "00000000-0919-1997-2025-000000000003"
UUID_BTN_CHAR = "00000000-0919-1997-2025-000000000004"

ROUTER_CLIENT_ID = "GABES-MQTT-BLE-CLIENT"

BASE_TOPIC = "GABESDEMO"
READ_TOPIC = "READ"
WRITE_TOPIC = "WRITE"

TOPIC_LED = f"{BASE_TOPIC}/{WRITE_TOPIC}/LED_VALUE"
TOPIC_PHOTO = f"{BASE_TOPIC}/{READ_TOPIC}/PHOTO_VALUE"
TOPIC_AIN = f"{BASE_TOPIC}/{READ_TOPIC}/AIN_VALUE"
TOPIC_BTN = f"{BASE_TOPIC}/{READ_TOPIC}/BTN_VALUE"


class MqttClient:

    def __init__(self, hostname: str, port: int, client_id: str):
        self.hostname = hostname
        self.port = port
        self.client_id = client_id

        self._led_value = 0

        self.client = mqtt.Client(
            CallbackAPIVersion.VERSION2,
            client_id=self.client_id,
            clean_session=True,
            protocol=mqtt.MQTTv311,
        )

        self.client.on_connect = self.on_connect 
        self.client.on_disconnect = self.on_disconnect

    @property
    def led_value_bytes(self):
        return self._led_value.to_bytes(byteorder="little", signed=False)

    @property
    def is_connected(self):
        return self.client.is_connected()

    def on_connect(self, client: mqtt.Client, userdata, flags, rc, props=None):
        if rc != 0:
            print(f"Connection failed, rc={rc}")
            return

        print(f"{client._client_id} connected")

        self.client.subscribe(TOPIC_LED)
        self.client.message_callback_add(TOPIC_LED, self.on_led_callback)

    def on_disconnect(self, client: mqtt.Client, userdata, flags, rc, props=None):
        print(f"{client._client_id} disconnected reason: {rc}")

    def on_led_callback(self, client: mqtt.Client, userdata, msg: mqtt.MQTTMessage):
        val = int(msg.payload.decode())
        print(f"On LED Value change callback: {val}")
        if 0 > val > 255: 
            val = 0
        self._led_value = val

    def Start(self):
        self.client.connect(host=self.hostname, port=self.port)

        self.client.loop_start()
        print("Starting MQTT")

    def Stop(self):
        self.client.loop_stop()
        self.client.disconnect()
        print("MQTT Stopped")

    def PubValue(self, topic:str, payload:str):
        self.client.publish(topic, payload)

def parse_args():
    ap = argparse.ArgumentParser(description="Route BLE traffic to MQTT")
    ap.add_argument(
        "--hostname", default="test.mosquitto.org", help="Broker hostname (default: https://test.mosquitto.org/)"
    )
    ap.add_argument(
        "--port",
        type=int,
        default=1883,
        help="Broker port (default: 1883)",
    )
    ap.add_argument(
        "--client-id", default="ble-demo", help="Broker client id (default: ble-demo)"
    )
    ap.add_argument(
        "--mac",
        default="0C:8B:95:34:6F:2E",
        help="BLE device to connect to (default: 0C:8B:95:34:6F:2E)",
    )

    return ap.parse_args()


stop_event = asyncio.Event() # finish setting this up

async def main():


    args = parse_args()

    print("Starting BLE to MQTT Service")

    mclient = MqttClient(args.hostname, args.port, args.client_id)

    def handle_sig(signum, frame):
        print(f"Caught signal {signum}, stopping")
        mclient.Stop()
        stop_event.set()

    signal.signal(signal.SIGINT, handle_sig)
    signal.signal(signal.SIGTERM, handle_sig)
    signal.signal(signal.SIGHUP, handle_sig)

    mclient.Start()

    async with BleakClient(args.mac) as bclient:
        print(f"Connected: {bclient.is_connected}")

        while bclient.is_connected and not stop_event.is_set():
            try:
                photo = int.from_bytes(
                    await bclient.read_gatt_char(UUID_PHOTO_CHAR), "little", signed=False
                )
                mclient.PubValue(TOPIC_PHOTO, str(photo))

                ain = int.from_bytes(
                    await bclient.read_gatt_char(UUID_AIN_CHAR), "little", signed=False
                )
                mclient.PubValue(TOPIC_AIN, str(ain))

                btn = int.from_bytes(
                    await bclient.read_gatt_char(UUID_BTN_CHAR), "little", signed=False
                )
                mclient.PubValue(TOPIC_BTN, str(btn))

                await bclient.write_gatt_char(UUID_LED_CHAR, mclient.led_value_bytes, True)

            except Exception as e:
                print(f"Failed to read characteristic: {e}")

    print("Disconnected.")
    mclient.Stop()

if __name__ == "__main__":
    asyncio.run(main())
