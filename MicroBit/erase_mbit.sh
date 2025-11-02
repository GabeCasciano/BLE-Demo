#!/bin/bash 
openocd -f interface/cmsis-dap.cfg -f target/nrf51.cfg \
  -c "init; halt; nrf51 mass_erase; program .pio/build/microbit_v1/firmware.hex verify reset; exit"
