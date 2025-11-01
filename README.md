Intall udev rules

```bash
# 1) Install PlatformIO udev rules
curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/master/scripts/99-platformio-udev.rules | sudo tee /etc/udev/rules.d/99-platformio-udev.rules >/dev/null

# 2) Reload and apply
sudo udevadm control --reload-rules
sudo udevadm trigger

# 3) Optional: add your user to dialout/plugdev (log out/in after)
sudo usermod -aG dialout $USER || true
sudo usermod -aG plugdev $USER  || true
```

replug the board and check visibility

```bash
lsusb | grep -i 0d28          # should show Arm mbed (0d28:0204)
ls /dev/serial/by-id/         # should show a USB CDC device
```
