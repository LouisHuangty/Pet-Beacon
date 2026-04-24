# Pet Beacon

Pet Beacon is a Flutter-based mobile app for lightweight pet tracking experiments. The project combines Bluetooth Low Energy scanning, RSSI-based proximity feedback, and a local M5Stack TimerCAM camera service to explore how a pet owner might detect when a tag moves farther away and quickly retrieve a recent visual update.

This repository was developed as part of CASA0015 Mobile Systems & Interactions coursework.

## App Link

- GitHub repository: <https://github.com/LouisHuangty/casa0015-mobile-assessment>

## Preview

Image placeholders are kept here for now and can be replaced later with screenshots, diagrams, or demo stills.

```text
[Placeholder] Hero image / app overview screenshot
```

```text
[Placeholder] Home page screenshot
```

```text
[Placeholder] BLE page screenshot
```

```text
[Placeholder] Device page screenshot
```

## Project Overview

Pet Beacon is built around a simple idea: use a BLE tag as a proximity signal for a pet, and combine it with a local camera board so that the owner can react when the pet moves farther away.

The current version supports:

- Firebase email and password authentication
- Firestore-backed user profile storage
- Pet profile setup with pet name and pet type
- BLE scanning and nearby device selection
- RSSI-based state mapping for `Very Close`, `Nearby`, and `Far`
- Automatic image capture when the selected BLE device enters `Far`
- Manual camera test capture from the app
- Camera service health detection on the local network
- History and last-seen style review screens

## Problem Statement

Common consumer pet trackers often depend on external networks, closed platforms, or more complex infrastructure than is necessary for local testing and prototyping. This project explores a more direct setup:

- a mobile app scans for a nearby BLE device
- signal strength is interpreted as a rough proximity estimate
- a local camera board provides a recent image when distance increases

The aim is not to build a production-ready pet tracker, but to prototype a practical interaction flow that links sensing, notification, and visual confirmation.

## Core Workflow

1. The user signs in and sets up a pet profile.
2. The app scans nearby BLE devices on the `BLE` page.
3. The user selects one BLE device as the tracking source.
4. The app maps RSSI values into proximity states.
5. When the selected device moves into `Far`, the app triggers a local camera capture.
6. The latest image and event details appear in the app history and last-seen views.

## Main Features

### 1. Authentication and Profile

- Firebase Authentication for registration and login
- Firestore-based profile storage
- Pet name and pet type configuration

### 2. BLE Tracking

- BLE discovery using `flutter_blue_plus`
- Device selection from nearby scan results
- Stable list ordering to reduce scan-list jumping
- RSSI threshold control for proximity classification
- Smoothed `Far` detection based on recent BLE samples

### 3. Camera Integration

- Local TimerCAM service detection
- Health check endpoint support
- Manual test capture from the `Device` page
- Automatic capture when BLE state enters `Far`
- Immediate local UI update, with Firestore used only as background backup

### 4. Multi-Page App Structure

- `Home`: current pet state and last automatic capture
- `History`: last seen information and capture events
- `Device`: camera endpoint, manual capture, and RSSI threshold controls
- `BLE`: scan, select, and debug nearby BLE devices

## Screens and Interaction Notes

### Home

- Displays the current pet status
- Shows the latest automatic capture when available
- Supports prototype state simulation when no live BLE device is selected

### History

- Displays recent capture events
- Shows the latest image and environment summary

### Device

- Displays the current camera endpoint
- Provides `Detect Camera Service`
- Provides `Test Camera Capture`
- Lets the user tune the RSSI threshold

### BLE

- Scans nearby BLE devices
- Lets the user choose one active tracking device
- Provides a `Clear Device` action to return to prototype mode

## Hardware and Firmware

The current active camera firmware is in:

- `esp32/imercam_x/`

Other earlier experiments are kept in:

- `esp32/petracker/`
- `raspberry_pi/petcam/`

The active TimerCAM firmware currently supports:

- local HTTP health checks
- single-frame JPEG capture
- MJPEG stream preview
- BLE advertising name configuration
- local network integration for the Flutter app

## Repository Structure

- `lib/main.dart` - main Flutter UI, BLE workflow, and camera logic
- `lib/firebase_options.dart` - Firebase platform configuration
- `pubspec.yaml` - Flutter dependencies and app version
- `esp32/imercam_x/` - active M5Stack TimerCAM firmware
- `esp32/petracker/` - earlier ESP32-CAM experiment
- `raspberry_pi/petcam/` - earlier Raspberry Pi camera service experiment

## Tech Stack

- Flutter
- Dart
- Firebase Authentication
- Cloud Firestore
- FlutterBluePlus
- HTTP
- PlatformIO
- ESP32 / M5Stack TimerCAM firmware

## Installation

### Flutter App

1. Clone the repository.
2. Run:

```bash
flutter pub get
```

3. Ensure Firebase config files are present for your target platform.
4. Connect a physical mobile device with Bluetooth enabled.
5. Start the app:

```bash
flutter run
```

### TimerCAM Firmware

1. Open `esp32/imercam_x/` in PlatformIO.
2. Build and upload the firmware to the TimerCAM board.
3. Connect the board to the same Wi-Fi network as the phone.
4. Confirm the following endpoints are reachable:

```text
http://<camera-ip>/health
http://<camera-ip>/capture-meta
http://<camera-ip>/stream
```

5. In the Flutter app, open the `Device` page and run `Detect Camera Service`.

## Current Limitations

- RSSI is only a rough proxy for physical distance
- BLE detection can still vary with environment and device orientation
- Camera capture reliability depends on the local board and Wi-Fi stability
- The system is still a prototype rather than a deployable pet tracking product

## Future Improvements

- clearer in-app distinction between automatic and manual capture states
- stronger BLE device identification and pairing flow
- more robust camera retry and recovery logic
- richer event history and export options
- better visual documentation and final project media

## Media Placeholders

You can replace these later with final assets:

```text
[Placeholder] System architecture diagram
```

```text
[Placeholder] BLE workflow diagram
```

```text
[Placeholder] Hardware setup photo
```

```text
[Placeholder] Demo GIF or QR code link
```
