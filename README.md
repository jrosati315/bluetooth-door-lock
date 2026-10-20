# Bluetooth Door Lock

An ESP32 Bluetooth Low Energy door-lock prototype with stepper motor control and an RGB status LED.

## Hardware

- ESP32-WROOM-32E; Arduino board selection: ESP32 Dev Module.
- Stepper driver inputs in constructor order: GPIO 27, 25, 26, 33.
- Common-cathode RGB LED: common to GND; red through its own 220-ohm resistor to GPIO 18; green through its own 220-ohm resistor to GPIO 19.
- Motor and driver model numbers, power wiring, and mechanism dimensions still need documentation. Motor coils must connect through a driver.

## Setup

Open `firmware/BluetoothLock/BluetoothLock.ino` in Arduino IDE. Install **esp32 by Espressif Systems** using Boards Manager and **Stepper by Arduino** using Library Manager. Select **ESP32 Dev Module**. BLE headers come with the ESP32 board package. The development computer has board package 3.3.12 installed.

The builder reports working phone connectivity and motor/LED operation. This saved baseline has not been independently compiled or tested.

## Operation

BLE name: `ESP32Lock`. Service UUID: `6E400001-B5A3-F393-E0A9-E50E24DCCA9E`. Writable characteristic: `6E400002-B5A3-F393-E0A9-E50E24DCCA9E`.

- Write text `123`: unlock, 512 steps, then green.
- Write text `000`: lock, -512 steps, then red.
- Yellow during movement: red brightness 255, green 190.
- LED off at startup: position unknown.
- Motor configuration: 2048 steps per revolution, 10 RPM.

## Prototype limitations

No phone authentication is configured. Repeated commands repeat motor movement. There is no position feedback; colors indicate completed commands rather than verified bolt position. Movement blocks inside the BLE callback. Reconnection still needs testing. This is an experimental prototype, not a validated security product.

## Planned work

Document the mechanism and power requirements, improve command handling and reconnection, add position feedback and authentication, design and test an enclosure, build an iPhone app with Shortcuts actions, and design a custom PCB. CAD, electronics, app files, photos, and test records will be added as developed.
