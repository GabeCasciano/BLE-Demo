#!/bin/bash 
openocd -f interface/cmsis-dap.cfg -f target/nrf51.cfg \
  -c "init; halt; nrf51 mass_erase; program softdevice/s110_nrf51_8.0.0.hex verify reset; exit"
