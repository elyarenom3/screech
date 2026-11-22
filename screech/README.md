# Screech firmware

Robot name: `screech`
WiFi network: `elya_screech`
Password: the `AP_PASS` value in `secrets.h`
Control page: `http://192.168.4.1` (opens automatically when you join the network)

## Flash to the S2 Mini

Option 1, one command:

```
cd screech
./flash.sh
```

Option 2, Arduino IDE:

1. Open `screech/screech.ino`.
2. Tools > Board > esp32 > **LOLIN S2 Mini**.
3. Tools > Port > pick the `usbmodem` port.
4. Click Upload.

The upload needs bootloader mode: hold button **0**, tap **RST**, keep holding **0** for 2 seconds, release.
`./flash.sh` starts waiting after it compiles, so enter bootloader mode when it asks.
After the upload, tap **RST** once if the board does not restart by itself.

## Settings in the code

Before the first build, copy `secrets.example.h` to `secrets.h` and set the robot's access-point password (`AP_PASS`) and your home WiFi name and password (or leave the home WiFi blank for access-point-only mode). `secrets.h` is ignored by git.

In `screech.ino`:

- `AP_SSID`: the WiFi network name. The password `AP_PASS` lives in `secrets.h`.
- `deviceHostname`: the name used on your home network (`screech.local`).
- Pins are already set for the S2 Mini (I2C 33/35, servos 1, 2, 4, 6, 8, 10, 13, 14).

Libraries (already installed): Adafruit GFX, Adafruit SSD1306, Adafruit BusIO, ESP32Servo.
