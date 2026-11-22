#!/bin/bash
# Compile and flash the Screech firmware to a LOLIN S2 Mini.
# 1. Run this script.
# 2. Put the board in bootloader mode: hold 0, tap RST, keep holding 0 for 2 seconds, release.
set -e
cd "$(dirname "$0")"

CLI="/Applications/Arduino IDE.app/Contents/Resources/app/lib/backend/resources/arduino-cli"
ET=$(ls -d "$HOME"/Library/Arduino15/packages/esp32/tools/esptool_py/*/ | head -n 1)
BUILD=$(mktemp -d)

"$CLI" compile --fqbn esp32:esp32:lolin_s2_mini --libraries "$HOME/Documents/Arduino/libraries" --build-path "$BUILD" .

echo "Compiled. Now put the S2 Mini in bootloader mode (hold 0, tap RST, hold 0 for 2 seconds, release)."
for i in $(seq 1 300); do
  for P in /dev/cu.usbmodem*; do
    [ -e "$P" ] || continue
    if "$ET/esptool" --chip esp32s2 --port "$P" --before no-reset --after hard-reset --connect-attempts 2 write-flash 0x0 "$BUILD/screech.ino.merged.bin"; then
      echo "Done. Look for the WiFi network: elya_screech  (password: AP_PASS from secrets.h)"
      exit 0
    fi
  done
  sleep 1
done
echo "Timed out waiting for the board. Replug it and try again."
exit 1
