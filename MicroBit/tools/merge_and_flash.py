
import os
from os.path import join, exists
from SCons.Script import Import

Import("env")

PROJECT_DIR = env.subst("$PROJECT_DIR")
BUILD_DIR   = env.subst("$BUILD_DIR")
APP_HEX     = join(BUILD_DIR, "firmware.hex")
MERGED_HEX  = join(BUILD_DIR, "merged.hex")

# Put your S110 here (e.g., softdevice/s110_nrf51_8.0.0.hex)
SD_PATH     = join(PROJECT_DIR, "softdevice", "s110_nrf51_8.0.0_softdevice.hex")

# Locate bundled tools
def exe(name):
    return name + ".exe" if os.name == "nt" else name

OPENOCD_DIR = env.PioPlatform().get_package_dir("tool-openocd")
SREC_DIR    = env.PioPlatform().get_package_dir("tool-sreccat")

OPENOCD     = join(OPENOCD_DIR, "bin", exe("openocd"))
SREC_CAT    = join(SREC_DIR, exe("srec_cat"))

def merge_sd(source, target, env):
    if not exists(SD_PATH):
        print(f"*** SoftDevice not found: {SD_PATH}")
        env.Exit(1)
    # Merge SoftDevice + app -> merged.hex
    cmd = f'"{SREC_CAT}" "{SD_PATH}" -Intel "{APP_HEX}" -Intel -o "{MERGED_HEX}" -Intel'
    print(f"*** Merging SoftDevice + app -> {MERGED_HEX}")
    if env.Execute(cmd) != 0:
        print("*** srec_cat failed")
        env.Exit(1)

# After firmware.hex is produced, create merged.hex
env.AddPostAction(APP_HEX, merge_sd)

# Custom upload target. NOTE: dependencies **must** be provided.
# We depend on APP_HEX; the post-action above will generate MERGED_HEX.
env.AddCustomTarget(
    name="upload_merged",
    dependencies=[APP_HEX],
    actions=(
        f'"{OPENOCD}" -f interface/cmsis-dap.cfg -f target/nrf51.cfg '
        f'-c "init; halt; nrf51 mass_erase; program {MERGED_HEX} verify reset; exit"'
    ),
    title="Upload merged (SD + app)",
    description="Mass erase + program merged SoftDevice+app via CMSIS-DAP",
)
