#!/bin/bash 
platformio run -t clean 
platformio run -e espdev
platformio run -e espdev --target compiledb
