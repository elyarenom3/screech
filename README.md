# Screech

Screech is a small four-legged walking robot with an animated OLED face, controlled from a phone over WiFi. It is an adaptation of the the open-source [Sesame Robot Project](https://github.com/dorianborian/sesame-robot) by Dorian Todd, with my own firmware changes, my own face artwork, my own web controller styling, and a body I designed and made by hand.

The body is hand-made: I used some of the Sesame STL shapes as reference and built my own parts out of paper, reinforced with wooden dowels and glue. The electronics are based on the hand-wired LOLIN S2 Mini.

This README documents everything in the repository: the hardware, the paper build, the wiring, the firmware, the web controller, the HTTP and serial interfaces, the face system, and the reference material it was all built from.

---

## Contents

1. [What is in this repository](#what-is-in-this-repository)
2. [The robot at a glance](#the-robot-at-a-glance)
3. [Body: paper parts from the Sesame shapes](#body-paper-parts-from-the-sesame-shapes)
4. [Electronics and parts list](#electronics-and-parts-list)
5. [Wiring](#wiring)
6. [Firmware](#firmware)
   - [What I changed compared with Sesame](#what-i-changed-compared-with-sesame)
   - [Files](#firmware-files)
   - [Build and flash](#build-and-flash)
   - [First connection](#first-connection)
   - [Home network mode](#home-network-mode)
7. [Web controller](#web-controller)
8. [Gamepad control](#gamepad-control)
9. [HTTP API](#http-api)
10. [Serial command line](#serial-command-line)
11. [Faces](#faces)
12. [Movement and poses](#movement-and-poses)
13. [Calibration](#calibration)
14. [Reference material (ref/)](#reference-material-ref)
15. [Troubleshooting](#troubleshooting)
16. [Credits and license](#credits-and-license)

---

## What is in this repository

| Path | What it is |
| --- | --- |
| `screech/` | The firmware for the LOLIN S2 Mini. Open this folder in the Arduino IDE, or run `flash.sh`. |
| `screech/screech.ino` | Main sketch: WiFi, captive portal, HTTP API, servo control, face animation, idle behaviour, serial command line. |
| `screech/captive-portal.h` | The web controller page (HTML, CSS and JavaScript) served by the robot. |
| `screech/face-bitmaps.h` | The "arcade" face set: 68 frames of 128x64 pixel bitmaps for 37 named faces. |
| `screech/movement-sequences.h` | Servo naming, face animation modes, and every pose and walking sequence. |
| `screech/flash.sh` | One-command compile and flash for macOS using the Arduino IDE's bundled `arduino-cli`. |
| `screech/secrets.example.h` | Template for `secrets.h`, which holds the home WiFi name and password. `secrets.h` is ignored by git. |
| `screech/README.md` | Short quick-start for flashing. |
| `shapes/` | The three Sesame STL files I used as physical reference for the paper parts: `R1-v117.stl`, `L1-v117.stl`, `L3-v117.stl`. |
| `face-bitmaps.h` | An identical copy of `screech/face-bitmaps.h` kept at the top level. |
| `ref/sesame-robot-main/` | A full copy of the upstream Sesame repository: CAD, STLs, PCB files, build guide, wiring guide, firmware, photos. |
| `ref/sesame-firmware-main/` | The upstream firmware exactly as I started from it, for diffing. |
| `ref/sesame-parts-drawings.pdf` | A 65-sheet drawing pack of every Sesame printed part, with millimetre grids and actual-size templates. |
| `LICENSE`, `NOTICE` | Apache License 2.0, inherited from Sesame, plus a notice listing the changes made here. |

`ref/sesame-robot-main.zip` (84 MB) exists on disk but is excluded from git because its contents are already extracted next to it.

---

## The robot at a glance

| | |
| --- | --- |
| Form | Quadruped, two servos per leg, eight servos in total |
| Controller | LOLIN (WeMos) ESP32-S2 Mini |
| Display | 0.96" SSD1306 OLED, 128x64, I2C address 0x3C |
| Servos | 8x MG90S, 180 degree version, 50 Hz PWM, pulse range 732 to 2929 µs |
| Power | 5 V at 3 A: USB-C, or battery through a rocker switch and a 5 V buck converter |
| Body | Paper parts reinforced with wooden dowels and glue, shaped from the Sesame STLs |
| Control | Phone or laptop over WiFi, gamepad through the browser, HTTP API, USB serial |
| WiFi | Creates its own network `elya_screech`. Optionally joins a home network as `screech.local`. |
| Firmware size | 1,162,306 bytes of flash (88% of 1,310,720), 79,960 bytes of RAM at start (24%) |

### Joint and servo names

The Sesame naming is used everywhere: on the drawings, in the firmware, on the web page and on the wiring. R means the robot's right and L its left, with the robot facing away from you.

| Servo index | Joint | Where it is | What it moves | S2 Mini GPIO |
| --- | --- | --- | --- | --- |
| S0 | R1 | front right, in the body | swings the front right leg forward and back | 1 |
| S1 | R2 | back right, in the body | swings the back right leg | 2 |
| S2 | L1 | front left, in the body | swings the front left leg | 4 |
| S3 | L2 | back left, in the body | swings the back left leg | 6 |
| S4 | R4 | inside the back right leg | raises and lowers that leg | 8 |
| S5 | R3 | inside the front right leg | raises and lowers that leg | 10 |
| S6 | L3 | inside the front left leg | raises and lowers that leg | 13 |
| S7 | L4 | inside the back left leg | raises and lowers that leg | 14 |

Note that S4 is R4 and S5 is R3. This ordering comes from upstream and is kept so the poses and the wiring guide match.

---

## Body: paper parts from the Sesame shapes

Sesame is designed as 11 3D-printed parts: four hip joints (R1, R2, L1, L2), four legs (R3, R4, L3, L4), an internal frame, a top cover and a bottom cover. They come in 7 distinct shapes, because the right and left hip joints and the right and left legs are mirror images of each other.

I did not print them. I used the STL shapes as a physical reference and made my own versions out of paper, then reinforced them with wooden dowels and glue so they are stiff enough to carry the servos and hold the robot's weight when it stands and walks.

What I used as reference:

- `shapes/R1-v117.stl`, `shapes/L1-v117.stl` and `shapes/L3-v117.stl`: one hip joint of each hand and one leg. The other joints are mirror images, so these three cover the geometry of all eight.
- The full STL set under `ref/sesame-robot-main/hardware/printing/stl/`, including the internal frame, bottom cover and the top cover variants.
- `ref/sesame-parts-drawings.pdf`: a drawing pack with six views of each shape on a 1 mm grid, enlarged views, section cuts, tables of hole positions and diameters, and an actual-size 1:1 template for each shape. Sheet 1 has a 50 mm check bar. Print at 100% scale and measure that bar; if it reads 50 mm, every template marked 1:1 is the real size of the part.

Reference sizes of the three STLs, measured from the files (bounding box, millimetres):

| Part | X | Y | Z |
| --- | --- | --- | --- |
| R1 hip joint | 20.3 | 30.5 | 49.5 |
| L1 hip joint | 20.3 | 30.5 | 49.5 |
| L3 leg | 9.5 | 19.1 | 58.4 |

Things that matter when the parts are paper:

- The servo pockets are the critical dimensions. Cut the hip joint and leg openings from the 1:1 templates and test the fit on a real servo before gluing.
- The hip joints take a one-sided servo horn pressed in flush, then screwed. In paper, the dowel reinforcement should sit under the horn so the screw bites into wood.
- The legs carry a servo inside them with the shaft at the very top. The dowel runs the length of the leg so the leg does not fold at the pocket.
- The internal frame holds the four body servos with their shafts facing outward, the controller, and the battery. It needs the most stiffness, since all eight servos pull on it.

Sesame's own print notes, orientation images and support guidance are in `ref/sesame-robot-main/hardware/printing/README.md` if you prefer to print.

---

## Electronics and parts list

This is the hand-wired LOLIN S2 Mini option from the Sesame bill of materials. The full list with purchase links, the Distro Board options and the power notes is in `ref/sesame-robot-main/hardware/bom/README.md`.

| Item | Qty | Notes |
| --- | --- | --- |
| LOLIN / WeMos ESP32-S2 Mini | 1 | Native USB-C. Fits a small perfboard. |
| MG90S all-metal micro servo, 180 degree | 8 | Buy spares. Must be the 180 degree version. |
| 0.96" SSD1306 I2C OLED, 128x64 | 1 | Four wires: VCC, GND, SDA, SCL. |
| Small protoboard, about 5 x 7 cm | 1 | Hosts the 3-pin servo headers and the rails. |
| 3-pin male header | 8 | One per servo plug. |
| 22 AWG silicone wire | 1 kit | 5 V rail and ground rail. |
| 30 AWG silicone wire | 1 kit | The eight servo signal wires and the OLED wires. |
| Rocker switch, KCD1 panel mount | 1 | Battery on/off. |
| Buck converter, 5 to 12 V in, 5 V 3 A out | 1 | Only for battery power. Set to 5.1 V. |
| Battery | 1 | Sesame recommends the Bambu Lab 14500 7.4 V 800 mAh pack with an XH2.54 pigtail. |
| USB-C data cable | 1 | Flashing, and tethered 5 V 3 A power. |
| Heat-shrink, small zip ties | | Insulation and bundling. |
| M2 x 5 mm self-threading screws | about 40 | Joints, servo mounts, covers. |
| M2.5 x 5 mm machine screws | 10 | Servo horn to servo shaft. The screws that come with servos are usually too short. |

Tools: soldering iron, 0.6 to 0.8 mm solder, flux, solder wick, flush cutters, precision screwdrivers, a multimeter.

Power rule from upstream: the rails need 5 V at 3 A. Route a battery through the switch and the buck converter before it reaches the rails. Never cut the factory connector off a battery pack; make an adapter pigtail instead.

---

## Wiring

The S2 Mini wiring diagram is at `ref/sesame-robot-main/docs/wiring-guide/s2-mini-wiring-guide-new.png` (credit to captianeverypowersr on the Sesame Discord). The step-by-step hand-wiring guide is `ref/sesame-robot-main/docs/wiring-guide/README.md`.

Pin assignments used by this firmware (`screech/screech.ino`):

| Signal | S2 Mini GPIO |
| --- | --- |
| Servo S0 (R1) | 1 |
| Servo S1 (R2) | 2 |
| Servo S2 (L1) | 4 |
| Servo S3 (L2) | 6 |
| Servo S4 (R4) | 8 |
| Servo S5 (R3) | 10 |
| Servo S6 (L3) | 13 |
| Servo S7 (L4) | 14 |
| OLED SDA | 33 |
| OLED SCL | 35 |

Summary of the harness:

1. Solder eight 3-pin headers onto the protoboard in a row, in S0 to S7 order.
2. Run one bare 22 AWG wire along all the middle pins: that is the 5 V rail. Run another along all the outer ground pins: that is the ground rail. Servo plugs go on with the brown wire on the ground rail.
3. Run eight 30 AWG signal wires from the remaining header pins to the GPIOs in the table.
4. Wire the OLED: VCC to 3.3 V or 5 V as the module requires, GND to ground, SDA to GPIO 33, SCL to GPIO 35.
5. Connect the rails to the S2 Mini's 5 V pin and GND. With a battery, the switch and buck converter feed the rails.
6. Keep the OLED and switch leads un-soldered until the electronics are seated, so the lengths can be set exactly.

The firmware staggers servo moves by `motorCurrentDelay` (20 ms by default) so that eight servos do not start at once and pull the rail down. Raise it in Settings if the board resets during movement.

---

## Firmware

The firmware is a fork of the upstream Sesame firmware (`ref/sesame-firmware-main/`), written for the Arduino-ESP32 core. One event loop runs the DNS server for the captive portal, the HTTP server, the WiFi state machine, the face animation, the idle blink timer, the OLED info scroll and the current movement command.

### What I changed compared with Sesame

| Area | Sesame | Screech |
| --- | --- | --- |
| Robot name | Sesame | Screech. Boot splash shows `SCREECH` for 2 seconds. |
| Access point | `Sesame-Controller` / `12345678` | `elya_screech` / `hotwh33ls` |
| mDNS hostname | `sesame-robot.local` | `screech.local` |
| Home WiFi credentials | Hard-coded in the sketch | In `secrets.h`, ignored by git. `secrets.example.h` is the template. |
| `frameDelay` | 100 ms | 50 ms (faster walking frames) |
| `walkCycles` | 10, max 50 in the settings panel | 75, max 200 in the settings panel |
| Face frame rates | Mostly 1 fps | Per-face rates tuned to the new artwork: walk 4, rest 1, swim 3, dance 3, wave 2, point 4, stand 1, cute 2, pushup 3, freaky 2, bow 1, worm 4, shake 4, shrug 2, dead 1, crab 3 |
| Face animation modes in poses | wave, swim, pushup, worm, shake, shrug, crab play once | They loop while the pose runs. cute plays as a boomerang. |
| Face bitmaps | Upstream kaomoji-style faces | New "arcade" set: chunky pixel eyes with scanlines, a status line in the top 16 rows, level bars as the talking mouth. Same 37 names. |
| Web controller | Dark theme, orange accent | Retro appliance theme: cream panels with corner screws, cassette-deck keys, LCD-style status readouts, scan lines, an animated robot head in the header, Cyrillic subtitles on every section, electric blue accent |
| Flashing | Arduino IDE only | `flash.sh` compiles and flashes from the terminal |

Everything else, including the HTTP API, the WiFi provisioning state machine, the serial command line, the idle system and the pose sequences, is unchanged from upstream.

### Firmware files

**`screech.ino`**

- Configuration block: AP name and password, `#include "secrets.h"` for the home network, OLED size and address, I2C pins, the `servoPins` array for the S2 Mini (with the Distro Board V1, V2 and V3 pinouts kept as comments), the `servoSubtrim` offsets, and the animation constants.
- `faceEntries[]` is generated from the `FACE_LIST` macro and maps a face name to its frames. `faceFpsEntries[]` sets the frame rate per face.
- HTTP handlers: `handleRoot`, `handleCommandWeb`, `handleGetSettings`, `handleSetSettings`, `handleGetStatus`, `handleApiCommand`, `handleWifiScan`, `handleWifiConnect`, `handleWifiStatus`, `handleNotFound`.
- WiFi: `connectToWifi` (blocking, used at boot), `updateWifiSetup` (non-blocking state machine for connects started from the web page, plus a 5 second link watchdog), `startMdns`, `announceNetwork`.
- `setup()`: serial at 115200, I2C, OLED, boot splash, WiFi (station first if enabled, then the access point), DNS server on port 53 answering every name with the robot's IP, HTTP routes, four PWM timers, eight servos attached, rest face.
- `loop()`: services DNS, HTTP, WiFi setup, face animation, idle blink and info scroll, then runs `currentCommand`, then reads the serial command line.
- Face engine: `setFace`, `setFaceWithMode`, `updateAnimatedFace`, `delayWithFace` (a delay that keeps the face and the web server alive).
- Idle engine: `enterIdle`, `exitIdle`, `updateIdleBlink`, `scheduleNextIdleBlink`.
- Helpers: `setServoAngle` (applies subtrim, clamps to 0 to 180, waits `motorCurrentDelay`), `pressingCheck` (waits while keeping the server alive and returns false if the command changed), `recordInput`, `updateWifiInfoScroll`.

**`movement-sequences.h`**

- `enum ServoName` with the S0 to S7 ordering, `servoNameToIndex`, `enum FaceAnimMode` (LOOP, ONCE, BOOMERANG).
- Every pose as an inline function: rest, stand, wave, dance, swim, point, pushup, bow, cute, freaky, worm, shake, shrug, dead, crab, and the four continuous moves: walk forward, walk backward, turn left, turn right.

**`face-bitmaps.h`**

- `FACE_LIST` X-macro naming the 37 faces.
- One `PROGMEM` array per frame, 1,024 bytes each, named `epd_bitmap_<face>` for frame 0 and `epd_bitmap_<face>_1` to `_5` for extra frames.

**`captive-portal.h`**

- The whole controller page as one `PROGMEM` string literal (`index_html`), about 1,300 lines.

**`flash.sh`**

- Compiles with the Arduino IDE's bundled `arduino-cli` for `esp32:esp32:lolin_s2_mini`, then waits up to five minutes for a `/dev/cu.usbmodem*` port and writes the merged binary at address 0 with `esptool`.

### Build and flash

Prerequisites:

1. Arduino IDE 2.x.
2. The ESP32 board package by Espressif. Add `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json` under File > Preferences > Additional Board Manager URLs, then install "esp32" from the Boards Manager.
3. Libraries, from the Library Manager: `ESP32Servo` (upstream pins v3.0.9 because newer releases had a bug where writing one servo moved several), `Adafruit SSD1306`, `Adafruit GFX Library`, `Adafruit BusIO`.
4. Create `screech/secrets.h`:

```sh
cp screech/secrets.example.h screech/secrets.h
```

Then edit it. Leave the SSID empty or `ENABLE_NETWORK_MODE` false for access-point-only use.

```cpp
#define NETWORK_SSID "YourHomeNetwork"   // 2.4 GHz only
#define NETWORK_PASS "YourPassword"
#define ENABLE_NETWORK_MODE true
```

Option 1, one command (macOS, Arduino IDE installed in /Applications):

```sh
cd screech
./flash.sh
```

The script compiles first, then prints a prompt. At that point put the S2 Mini in bootloader mode: hold button **0**, tap **RST**, keep holding **0** for two seconds, release. The script finds the port and flashes. Tap **RST** afterwards if the board does not restart on its own.

Option 2, Arduino IDE:

1. Open `screech/screech.ino`.
2. Tools > Board > esp32 > **LOLIN S2 Mini**.
3. Tools > USB CDC On Boot > Enabled. Upload Speed 921600. Partition scheme "Default 4MB with spiffs".
4. Tools > Port > the `usbmodem` port.
5. Put the board in bootloader mode as above, then click Upload.

Compile result on the current code: 1,162,306 bytes of program storage (88%) and 79,960 bytes of global RAM (24%).

### First connection

1. Power the robot. The OLED shows `SCREECH`, then "Setting up WiFi...", then the rest face.
2. On a phone, join the WiFi network `elya_screech` with password `hotwh33ls`.
3. The captive portal opens the controller page by itself. If it does not, open `http://192.168.4.1` in a browser.
4. For the first 30 seconds without input, a line scrolls across the top of the face with the network name, password and IP. It stops once you press anything.

### Home network mode

Two ways to join a home network, both 2.4 GHz only:

- **At compile time** through `secrets.h`. The robot connects at boot, keeps its own access point running at the same time, and advertises `http://screech.local` through mDNS. The OLED scroll then shows both addresses. If the connection fails it falls back to access-point-only mode.
- **At runtime** from the web page: Settings > WiFi / Network > Scan for Networks, pick a network, enter the password, Connect. The attempt runs in the background and the page polls for the result for about 15 seconds. Your phone may drop off the robot's network for a moment while the radio changes channel. Credentials entered this way are held in RAM only (`WiFi.persistent(false)`), so after a reboot the robot is back to access-point mode.

Security note from upstream: there is no authentication and no HTTPS. Anyone on the same network can drive the robot. Keep it on networks you trust.

---

## Web controller

The page served at `/` is a single HTML file with no external dependencies, so it works with no internet connection. The theme is a late-1980s home appliance: cream plastic panels with corner screws, chunky keys, a small green LCD readout for the command queue and the gamepad state, red and mustard accents, faint scan lines, an animated robot head with glowing lenses in the header, and a Cyrillic subtitle on each section (СКРИЧ for Screech, ДВИЖЕНИЕ movement, ПОЗЫ poses, СИСТЕМА system, НАСТРОЙКИ settings, СТОП stop). It is laid out for a phone held upright and also works on a laptop.

Sections:

- **Movement control.** A D-pad: forward, backward, left, right. The robot moves while a key is held and stops on release. A STOP ALL key clears the current command.
- **Poses and animations.** Fifteen keys: Stand, Rest, Wave, Dance, Swim, Point, Push-up, Bow, Cute, Freaky, Worm, Shake, Shrug, Dead, Crab. Each runs once and returns to the stand pose with the idle face.
- **System.** Opens Settings, and shows the gamepad readout.
- **Settings panel.**
  - `frameDelay` (ms between walking frames, default 50)
  - `walkCycles` (1 to 200, default 75)
  - `motorCurrentDelay` (ms between servo moves, default 20)
  - Accent colour: Electric Blue (default), Flame Orange, Cyan, Light Blue, Green, Red, Purple, Yellow, Pink, or a custom colour. Saved in the browser.
  - Individual motor sliders S0 R1 to S7 L4, 0 to 180 degrees, for testing and calibration.
  - WiFi / Network: status, scan, manual SSID entry for hidden networks, password, connect, and the resulting `screech.local` link and IP.

Settings are read from `/getSettings` when the panel opens and written with `/setSettings` on save.

---

## Gamepad control

If the browser supports the Gamepad API, a connected controller drives the robot directly. The readout under System changes from "Gamepad disconnected" to "Gamepad connected". The page polls the controller every 80 ms.

| Control | Action |
| --- | --- |
| Left stick or D-pad up / down / left / right | Walk forward / backward, turn left / right. Releasing stops. |
| A (Cross) | Stand |
| B (Circle) | Wave |
| X (Square) | Dance |
| Y (Triangle) | Swim |
| LB (L1) | Point |
| RB (R1) | Push-up |
| LT (L2) | Bow |
| RT (R2) | Shake |
| Back (Share) | Stop |
| Start (Options) | Rest |
| L3 (stick click) | Cute |
| R3 (stick click) | Freaky |
| Home (PS) | Stop |
| Touchpad / extra button | Worm |

The stick has a 0.5 dead zone and picks whichever axis is larger.

---

## HTTP API

All endpoints are on port 80 at `http://192.168.4.1` (access point) or `http://screech.local` (home network). Unknown `/api/*` paths return a JSON 404. Any other unknown path returns the controller page, which is what makes the captive portal work.

### Query-string endpoints (used by the web page)

```http
GET /cmd?go=forward          # also backward, left, right. Runs until stop.
GET /cmd?stop=1
GET /cmd?pose=wave           # any pose name from the list below
GET /cmd?motor=3&value=120   # motor 1 to 8 (S0 to S7), angle 0 to 180
GET /cmd?motor=L3&value=120  # servo names also work
GET /getSettings             # {"frameDelay":50,"walkCycles":75,"motorCurrentDelay":20,"faceFps":8}
GET /setSettings?frameDelay=50&walkCycles=75&motorCurrentDelay=20&faceFps=8
```

### JSON API

```http
GET /api/status
```

```json
{"currentCommand":"forward","currentFace":"walk","networkConnected":true,"apIP":"192.168.4.1","networkIP":"192.168.1.42"}
```

```http
POST /api/command
Content-Type: application/json

{"command":"wave","face":"happy"}     # movement plus face
{"face":"thinking"}                   # face only, no movement
{"command":"stop"}                    # stop
```

Responses are `{"status":"ok","message":"..."}` on success or `{"error":"..."}` with a 4xx status.

### WiFi provisioning

```http
GET  /api/wifi/scan      # {"scanning":true} while scanning, then [{"ssid":"...","rssi":-60,"secure":true}, ...]
POST /api/wifi/connect   # form fields ssid, password -> {"success":true,"pending":true}
GET  /api/wifi/status    # {"connected":true,"connecting":false,"ssid":"...","ip":"...","host":"screech.local","mdns":true,"rssi":-58}
```

### Examples

```sh
curl http://screech.local/api/status
curl -X POST http://screech.local/api/command -H "Content-Type: application/json" -d '{"command":"dance","face":"dance"}'
curl -X POST http://screech.local/api/command -H "Content-Type: application/json" -d '{"face":"love"}'
```

```python
import requests
robot = "http://screech.local"
requests.post(f"{robot}/api/command", json={"command": "wave", "face": "happy"})
requests.post(f"{robot}/api/command", json={"face": "talk_happy"})
print(requests.get(f"{robot}/api/status").json())
```

Commands: `forward`, `backward`, `left`, `right`, `stop`, `rest`, `stand`, `wave`, `dance`, `swim`, `point`, `pushup`, `bow`, `cute`, `freaky`, `worm`, `shake`, `shrug`, `dead`, `crab`.

---

## Serial command line

Open the Serial Monitor at 115200 baud (Tools > Serial Monitor) or any terminal on the USB port. Commands end with a newline. Useful for wiring checks and calibration with the body open.

| Command | Effect |
| --- | --- |
| `<motor> <angle>` | Move one servo, index 0 to 7, angle 0 to 180. Example: `5 90` |
| `all <angle>` | Move all eight servos. Example: `all 90` |
| `run walk` or `rn wf` | Walk forward one cycle |
| `rn wb`, `rn tl`, `rn tr` | Walk backward, turn left, turn right |
| `run rest` or `rn rs` | Rest pose (all servos to 90) |
| `run stand` or `rn st` | Stand pose |
| `rn wv`, `rn dn`, `rn sw`, `rn pt`, `rn pu`, `rn bw`, `rn ct`, `rn fk`, `rn wm`, `rn sk`, `rn sg`, `rn dd`, `rn cb` | Wave, dance, swim, point, push-up, bow, cute, freaky, worm, shake, shrug, dead, crab |
| `face <name>` or `fc <name>` | Show a face without moving. Example: `fc surprised` |
| `subtrim` or `st` | Print the current subtrim offsets |
| `st <motor> <offset>` | Set an offset in degrees, -90 to +90, for one servo. Example: `st 2 -4` |
| `st save` | Print a `servoSubtrim[8] = {...};` line to paste into the sketch |
| `st reset` | Clear all offsets |

Subtrim offsets set over serial live in RAM. Paste the `st save` output into `screech.ino` to make them permanent.

---

## Faces

Faces are 128x64 one-bit bitmaps drawn on the OLED. The "arcade" set replaces the upstream artwork while keeping every face name, so the poses, the API and the serial commands are unchanged. The style: large pixel eyes with scan lines, a status line in the top 16 rows (which shows yellow on the common two-colour OLEDs), and solid level bars for the mouth on the talking faces.

| Face | Frames | FPS | Used by |
| --- | --- | --- | --- |
| walk | 2 | 4 | forward, backward, left, right |
| rest | 3 | 1 | rest |
| swim | 3 | 3 | swim |
| dance | 2 | 3 | dance |
| wave | 3 | 2 | wave |
| point | 3 | 4 | point |
| stand | 1 | 1 | stand |
| cute | 3 | 2 | cute |
| pushup | 3 | 3 | pushup |
| freaky | 3 | 2 | freaky |
| bow | 2 | 1 | bow |
| worm | 3 | 4 | worm |
| shake | 3 | 4 | shake |
| shrug | 2 | 2 | shrug |
| dead | 2 | 1 | dead |
| crab | 3 | 3 | crab |
| defualt | 1 | 1 | fallback (the upstream spelling is kept in the code) |
| idle | 4 | 1 | idle breathing, boomerang |
| idle_blink | 4 | 7 | random blinks while idle |
| happy, sad, angry, surprised, sleepy, love, excited, confused, thinking | 1 each | 1 | conversational faces for the API |
| talk_happy, talk_sad, talk_angry, talk_surprised, talk_sleepy, talk_love, talk_excited, talk_confused, talk_thinking | 1 each | 1 | open-mouth variants for speech |

Animation modes (`FaceAnimMode`): `FACE_ANIM_LOOP` repeats, `FACE_ANIM_ONCE` stops on the last frame, `FACE_ANIM_BOOMERANG` plays forward then backward.

Idle behaviour: after a pose finishes, the robot enters idle. It shows the `idle` face as a boomerang and blinks at a random interval between 3 and 7 seconds, with a 30% chance of a double blink. Any command exits idle.

### Adding or replacing a face

1. Draw a 128x64 black and white image. Upstream suggests [jsPaint](https://jspaint.app/) or Sesame Studio.
2. Convert it with [image2cpp](https://javl.github.io/image2cpp/): horizontal byte order, 128x64, Arduino code output.
3. In `face-bitmaps.h`, add `X(myface)` to `FACE_LIST` and paste the array as `epd_bitmap_myface`. Extra frames are `epd_bitmap_myface_1` to `_5` (six frames maximum).
4. In `screech.ino`, add `{ "myface", fps }` to `faceFpsEntries[]` if it is animated.
5. Trigger it with `fc myface` over serial, `{"face":"myface"}` over the API, or `setFaceWithMode("myface", FACE_ANIM_LOOP)` in a pose.

---

## Movement and poses

All poses are in `movement-sequences.h`. Each one sets a face, moves servos through `setServoAngle` with `delayWithFace` pauses, then returns to `runStandPose(1)`, which also enters idle.

Reference angles:

| Pose | R1 | R2 | L1 | L2 | R4 | R3 | L3 | L4 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Rest | 90 | 90 | 90 | 90 | 90 | 90 | 90 | 90 |
| Stand | 135 | 45 | 45 | 135 | 0 | 180 | 0 | 180 |

In Rest the hip joints are parallel to the body and the legs are level. In Stand the hips turn 45 degrees to form an X and the legs push down.

The continuous moves (forward, backward, left, right) loop while `currentCommand` matches. Each servo step is followed by `pressingCheck(cmd, frameDelay)`, which keeps the web server alive and returns the robot to Stand the moment the command changes, which is how release-to-stop works. `frameDelay` is 50 ms here, half the upstream value, so the gait runs faster. The walk gait moves the front legs with a slightly longer stride than the rear legs and exchanges support legs at the same time as it swings.

One-shot poses, in brief: wave (one front leg lifts and waves four times), dance (hips level, legs alternate), swim (hips sweep), point (one leg extends forward for two seconds), pushup (four push-ups on the front legs), bow (front down, hold three seconds), cute, freaky, worm, shake, shrug, dead, crab.

---

## Calibration

Follow the upstream order: calibrate with the joints detached from the servo shafts, so no servo can stall against the body.

1. Flash the firmware. Open the web page or the serial monitor.
2. Plug servos in one at a time starting at S0, brown wire on the ground rail. Each one should move to its position and hold.
3. Send Rest (`rn rs`). All hips go to 90 degrees. Fit each hip joint so it sits parallel to the body. Do not screw it yet.
4. Send Stand (`rn st`). The four hips should form an X at 45 degrees. Send Rest again and check they return parallel. Then screw each hip joint to its shaft with an M2.5 machine screw.
5. Repeat for the legs: in Stand, fit each leg so it points down. Check in Rest that nothing collides. Screw them on.
6. If a joint sits a few degrees off, use `st <motor> <offset>` over serial to correct it, test with Rest and Stand, then `st save` and paste the line into `screech.ino`.

If a servo moves the wrong way or jitters, upstream's note applies: in most cases the plug is in the wrong header.

---

## Reference material (ref/)

Everything the build was derived from is kept in `ref/` so the repository is self-contained.

- `ref/sesame-robot-main/`: the Sesame repository. Key documents:
  - `README.md`: project overview and feature list
  - `hardware/bom/README.md`: full bill of materials with purchase links
  - `hardware/printing/README.md` and `hardware/printing/stl/`: print settings and all 11 STLs, including three top cover variants
  - `hardware/cad/`: Fusion 360 (`.f3z`, 40 MB) and STEP (30 MB) source models
  - `hardware/pcb/`: Distro Board V1, V2 and V3.1 schematics and Gerbers (unused here)
  - `docs/build-guide/README.md` and `assets/`: the photographed assembly guide
  - `docs/wiring-guide/README.md` and the S2 Mini wiring diagram
  - `docs/images/sesamefaces/`: the upstream face artwork as PNGs
  - `firmware/`: upstream firmware and `debugging-firmware/sesame-motor-tester.ino`
  - `motor-testing/README.md`: the servo test and hip alignment procedure
- `ref/sesame-firmware-main/`: the exact upstream firmware snapshot this fork started from. `diff -w` against `screech/` shows every change.
- `ref/sesame-parts-drawings.pdf`: 65 sheets, "Sesame robot: drawings of every printed part". Sheet 1 explains the pack and has the 50 mm check bar. Sheet 3 has the sizes that matter. Each of the 7 shapes then gets an overview, six views on a 1 mm grid, enlarged views, section cuts, hole and outline tables, and an actual-size template. All sizes were measured by computer from the official model files and are given to 0.01 mm.

---

## Troubleshooting

| Symptom | What to check |
| --- | --- |
| Upload fails or no port | Enter bootloader mode: hold 0, tap RST, hold 0 for two seconds, release. Use a data cable. Enable "USB CDC On Boot". |
| Board resets when servos move | Power supply under 3 A, or too many servos starting together. Use a stronger supply, raise `motorCurrentDelay` in Settings, check the rails. |
| A servo moves the wrong way or twitches | Plug in the wrong header, or brown wire on the wrong side. Check against the pin table. |
| Blank OLED | SDA and SCL swapped (33 and 35), wrong I2C address (firmware expects 0x3C), or the display power pin. The firmware halts in `setup()` if the display does not answer. |
| Cannot join home WiFi | Only 2.4 GHz networks work. Check the name and password in `secrets.h` letter by letter. Watch the serial monitor at 115200. |
| `screech.local` does not resolve | mDNS needs Bonjour on Windows or Avahi on Linux. Use the IP from the serial monitor or the OLED scroll. |
| Captive portal page does not appear | Open `http://192.168.4.1` by hand. |
| API returns 404 | `/api/command` needs POST with `Content-Type: application/json`. |
| Compile error: `secrets.h` not found | Copy `secrets.example.h` to `secrets.h`. |

---

## Credits and license

- Sesame Robot Project by [Dorian Todd](https://www.doriantodd.com/): design, firmware, guides and CAD. [github.com/dorianborian/sesame-robot](https://github.com/dorianborian/sesame-robot).
- S2 Mini wiring diagram by captianeverypowersr on the Sesame Discord.
- Screech firmware changes, arcade face set, web controller theme, paper body and drawing pack: Elya Renom.

Licensed under the Apache License, Version 2.0, the same license as Sesame. See `LICENSE` and `NOTICE`.
