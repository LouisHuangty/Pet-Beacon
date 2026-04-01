# Pet Beacon

Pet Beacon is a Flutter mobile app prototype for pet tracking. This version extends the earlier mock-data build with Bluetooth Low Energy scanning so the app can detect nearby BLE devices and use signal strength as part of the tracking experience.

## Current Version

This version includes:

- Firebase email/password login and registration
- Firestore-based user profile storage
- Pet profile setup with pet name and pet type
- Multi-page app navigation
- BLE device scanning
- Selection of a nearby BLE device
- RSSI-based proximity feedback for tracking states
- Prototype history, tracking, and device status screens

## Main Idea

The app is designed to help a pet owner monitor a pet tag and quickly check relevant information in one place. The current prototype combines account setup, profile information, and BLE scanning to support a simple pet-tracking workflow.

In this stage of development, some parts of the experience still use placeholder or prototype data, but the Bluetooth scanning flow is now integrated into the app structure.

## Bluetooth Feature

The BLE part of the app is used to:

- scan for nearby Bluetooth Low Energy devices
- display detected devices and identifiers
- show whether devices are connectable
- monitor RSSI signal strength
- map signal strength to simple pet proximity states

This version is intended as a prototype for testing BLE discovery and interaction rather than a final production-ready tracking system.

## Tech Stack

- Flutter
- Dart
- Firebase Authentication
- Cloud Firestore
- FlutterBluePlus

## Project Structure

- `lib/main.dart`: main application UI, BLE flow, and tracking logic
- `lib/firebase_options.dart`: Firebase platform configuration
- `ios/Runner/Info.plist`: iOS Bluetooth permission text
- `android/` and `ios/`: mobile platform configuration
- `esp32/petracker/`: earlier ESP32-CAM prototype firmware
- `raspberry_pi/petcam/`: phase 1 Raspberry Pi camera service

## Running the App

1. Install Flutter and platform dependencies.
2. Run `flutter pub get`.
3. Ensure Firebase configuration files are present for the target platform.
4. Enable Bluetooth permissions on the device.
5. Run the app with `flutter run`.

## Raspberry Pi Camera Flow

Phase 1 uses a Raspberry Pi camera service instead of the earlier ESP32-CAM hardware.

1. Copy `raspberry_pi/petcam/` to the Pi.
2. On the Pi, install the runtime packages:
   `sudo apt install -y python3-picamera2 python3-flask`
3. Start the service with `python3 app.py`
4. Confirm these endpoints work from the phone or laptop:
   `http://<pi-ip>:8000/health`
   `http://<pi-ip>:8000/capture-meta`
5. In the Flutter app, update `_raspberryPiBaseUrl` in [lib/main.dart](/Users/tangyihuang/mobile/myfirstapp_github/lib/main.dart) to your Pi IP.
6. Open the `Device` tab and use `Test Camera Capture` before validating automatic BLE lost capture.

## Notes

- This repository is part of the CASA0015 Mobile Systems & Interactions coursework development process.
- The app currently combines live BLE scanning with some prototype interface states.
- Future versions can extend the BLE workflow with stronger device pairing, more robust connection handling, and fuller real-world tracking logic.
