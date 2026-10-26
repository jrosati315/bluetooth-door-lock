# Bluetooth Door Lock

I’m building a Bluetooth-controlled door lock that I can operate from my iPhone. The goal is a compact assembly with a custom enclosure, dedicated PCB, and an iPhone app that exposes lock and unlock actions through Shortcuts.

The current prototype uses an ESP32 and a stepper motor. I can send commands from my phone to move the mechanism in either direction, and an RGB LED shows when it is moving and which command has finished. The next major task is designing the enclosure and mounting system in SolidWorks.

## Working prototype

The firmware runs on an ESP32-WROOM-32E and receives commands over Bluetooth Low Energy. An unlock command advances the stepper by 512 steps; a lock command moves it back by the same amount. The LED turns yellow during movement, then green after an unlock command or red after a lock command. I adjusted the red and green brightness separately to get a readable yellow.

I have tested phone connectivity, motor movement, and the LED indication on the prototype. It currently uses an existing Bluetooth app, which requires opening the app to send commands. That proves the basic control path, but it is not the interaction I want for the finished lock.

[View the Arduino firmware](firmware/BluetoothLock/BluetoothLock.ino).

## Where I’m taking the design

### Mechanical assembly

I want the lock to withstand force through its mechanism, mounts, and fasteners. The SolidWorks work will define how the motor drives the lock, how the assembly attaches to the door, and how the electronics fit inside the enclosure. A key design requirement is to keep the force holding the door closed out of the motor’s gears.

Before settling on materials or wall thicknesses, I need to document the dimensions, mounting constraints, and required holding force. Fit checks and physical load testing will determine whether the design meets those requirements; strength has not been validated yet. Manual operation and access during a power failure also need to be addressed.

### Custom electronics

The planned PCB will replace the development-board wiring with a dedicated controller that supports Bluetooth communication with an iPhone and can be programmed with the project firmware. It will bring together the controller, motor-driver connections, power regulation, programming access, status LED, and connections for position feedback.

The final controller and power source are still open decisions. The ESP32 already provides working BLE communication, so changing processors is not a prerequisite for improving the phone interface. Board size and component selection will follow the mechanical and power requirements.

### iPhone control

The intended app will provide separate Lock and Unlock actions, with Shortcuts support so I can operate the lock with one tap instead of navigating a general-purpose Bluetooth app. This stage still needs implementation and testing, including reconnection, operation while the app is backgrounded, and confirmation that the lock reached its intended position.

### Feedback and reliability

The current firmware tracks commands rather than measuring bolt position. Adding position feedback will let the system distinguish a completed movement from a jam or missed steps. I also plan to improve repeated-command handling, move motor operation out of the BLE callback, and add authenticated access before treating the prototype as a usable door lock.

## Development status

| Area | Status |
| --- | --- |
| ESP32 BLE commands from a phone | Working on the prototype |
| Stepper movement and RGB status indication | Working on the prototype |
| SolidWorks enclosure and mounting design | Planned; next major task |
| Position feedback and authenticated access | Planned |
| Dedicated PCB and power system | Planned |
| iPhone app and Shortcuts integration | Planned |
| Mechanical load and durability testing | Not yet performed |

This repository will hold the firmware, CAD models, PCB design files, app source, and test results as the project develops. The current build is a proof of concept, not a validated security product.

<details>
<summary>Prototype setup, wiring, and BLE interface</summary>

### Arduino setup

Open `firmware/BluetoothLock/BluetoothLock.ino` in Arduino IDE. Install **esp32 by Espressif Systems** from Boards Manager and **Stepper by Arduino** from Library Manager. Select **ESP32 Dev Module**. The BLE headers are included with the ESP32 board package; no separate BLE library is required. The development computer had board package 3.3.12 installed when this baseline was saved.

### Wiring recorded in the firmware

| Connection | ESP32 GPIO |
| --- | --- |
| Stepper driver inputs, constructor order | 27, 25, 26, 33 |
| Red LED through its own 220-ohm resistor | 18 |
| Green LED through its own 220-ohm resistor | 19 |
| RGB common cathode | GND |

The blue LED channel is unused. Yellow uses red brightness 255 and green 190. The motor is configured for 2048 steps per revolution and 10 RPM. Motor and driver model numbers and power wiring still need to be documented. Motor coils connect through a driver, not directly to ESP32 GPIO.

### BLE interface

- Device name: `ESP32Lock`
- Service UUID: `6E400001-B5A3-F393-E0A9-E50E24DCCA9E`
- Writable characteristic UUID: `6E400002-B5A3-F393-E0A9-E50E24DCCA9E`
- Write text `123` to request unlock; write text `000` to request lock.

At startup the LED is off because the position is unknown. Repeated commands repeat the motor movement. There is no authentication configured in this firmware, and LED colors do not verify the physical lock position.

</details>
