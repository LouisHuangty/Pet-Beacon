# Submission Guide

You will need to edit this file, create a PDF using the instructions below, sign it digitally, and upload it to Moodle.

## How to Create a PDF from Markdown

When finished, export this file to PDF using VS Code and the Markdown PDF extension. Make sure headers and footers are disabled in the extension preferences before exporting.

Upload the PDF to Moodle together with a copy of your presentation slides.

## Link to GitHub Repository

Flutter Application Name - Pet Beacon  
GitHub Repository - [https://github.com/LouisHuangty/Pet-Beacon](https://github.com/LouisHuangty/Pet-Beacon)

## Introduction to Application

Pet Beacon is a Flutter mobile application designed as a prototype pet tracking system that combines Bluetooth Low Energy sensing with a local camera service. The main idea is to use a BLE device as a pet tag and interpret its RSSI signal strength as a rough estimate of proximity. The app allows the user to scan nearby BLE devices, select one as the tracked tag, and monitor its state through simple categories such as Very Close, Nearby, and Far.

To make the tracking flow more practical, the app is also connected to an M5Stack TimerCAM board on the local network. When the tracked BLE signal moves into the Far range, the app can trigger a camera capture and display a recent image in the interface. The application also includes Firebase Authentication for sign-in and registration, Firestore-based profile storage, a device page for camera health checks and manual capture testing, a BLE page for device discovery and selection, and a history page for reviewing recent capture events.


## Bibliography

1. Flutter. (2026). *Flutter documentation*. Available at: <https://docs.flutter.dev/> (Accessed: 25 April 2026).
2. Firebase. (2026). *Firebase documentation*. Available at: <https://firebase.google.com/docs> (Accessed: 25 April 2026).
3. FlutterBluePlus. (2026). *FlutterBluePlus*. Available at: <https://pub.dev/packages/flutter_blue_plus> (Accessed: 25 April 2026).
4. M5Stack. (2026). *TimerCamera-X documentation*. Available at: <https://docs.m5stack.com/en/unit/timercam_x> (Accessed: 25 April 2026).
5. M5Stack. (2026). *TimerCam Arduino repository*. Available at: <https://github.com/m5stack/TimerCam-arduino/tree/master> (Accessed: 25 April 2026).
6. Espressif Systems. (2026). *ESP32 documentation*. Available at: <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/> (Accessed: 25 April 2026).
7. PlatformIO. (2026). *PlatformIO documentation*. Available at: <https://docs.platformio.org/> (Accessed: 25 April 2026).

----

## Declaration of Authorship

I, Tangyi Huang, confirm that the work presented in this assessment is my own. Where information has been derived from other sources, I confirm that this has been indicated in the work.

Tangyi Huang

25 April 2026
