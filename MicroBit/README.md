## Setup micro:bit on Linux

### 1. Intall udev rules

a. Install PlatformIO udev rules

`curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/master/scripts/99-platformio-udev.rules | sudo tee /etc/udev/rules.d/99-platformio-udev.rules >/dev/null`

b. Reload and apply

```bash
sudo udevadm control --reload-rules
sudo udevadm trigger
```

c. add your user to dialout (log out/in after)

`sudo usermod -aG dialout $USER || true`

### 2. replug the board and check visibility

```bash
lsusb | grep -i 0d28          # should show Arm mbed (0d28:0204)
ls /dev/serial/by-id/         # should show a USB CDC device
```

## microbit specific UUIDs : [repo](https://github.com/NordicSemiconductor/bluetooth-numbers-database/blob/master/v1/service_uuids.json)

```json
[
    {
        "name": "micro:bit Accelerometer Service",
        "identifier": "org.microbit.service.accelerometer",
        "uuid": "E95D0753-251D-470A-A062-FA1922DFA9A8",
        "source": "microbit"
    },
    {
        "name": "micro:bit Magnetometer Service",
        "identifier": "org.microbit.service.magnetometer",
        "uuid": "E95DF2D8-251D-470A-A062-FA1922DFA9A8",
        "source": "microbit"
    },
    {
        "name": "micro:bit Button Service",
        "identifier": "org.microbit.service.button",
        "uuid": "E95D9882-251D-470A-A062-FA1922DFA9A8",
        "source": "microbit"
    },
    {
        "name": "micro:bit IO Pin Service",
        "identifier": "org.microbit.service.io_pin",
        "uuid": "E95D127B-251D-470A-A062-FA1922DFA9A8",
        "source": "microbit"
    },
    {
        "name": "micro:bit LED Service",
        "identifier": "org.microbit.service.led",
        "uuid": "E95DD91D-251D-470A-A062-FA1922DFA9A8",
        "source": "microbit"
    },
    {
        "name": "micro:bit Event Service",
        "identifier": "org.microbit.service.event",
        "uuid": "E95D93AF-251D-470A-A062-FA1922DFA9A8",
        "source": "microbit"
    },
    {
        "name": "micro:bit DFU Control Service",
        "identifier": "org.microbit.service.dfu_control",
        "uuid": "E95D93B0-251D-470A-A062-FA1922DFA9A8",
        "source": "microbit"
    },
    {
        "name": "micro:bit Temperature Service",
        "identifier": "org.microbit.service.temperature",
        "uuid": "E95D6100-251D-470A-A062-FA1922DFA9A8",
        "source": "microbit"
    }
]
```
