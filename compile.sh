#!/bin/bash 
platformio run -t clean 
platformio run -e microbit_v1
platformio run -e microbit_v1 --target compiledb
