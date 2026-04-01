# PetCam Stage 1

Minimal Raspberry Pi camera service for phase 1 validation:

- live preview in browser
- single image capture
- saved capture metadata endpoint
- simple health check

## Prerequisites

- Raspberry Pi OS with a working camera
- `libcamera-still` already working
- Python 3 available
- `picamera2` installed on the Pi

Install runtime dependencies on the Pi:

```bash
sudo apt update
sudo apt install -y python3-picamera2 python3-flask
```

## Run

From the Raspberry Pi:

```bash
cd ~/petcam
python3 app.py
```

The service listens on port `8000`.

## Endpoints

- `/`: browser UI with live preview and a capture button
- `/health`: returns `{"status":"ok"}`
- `/stream`: MJPEG stream
- `/capture`: returns a fresh JPEG capture
- `/capture-meta`: captures and returns metadata JSON
- `/captures/<filename>`: serves saved images

## Copy From Your Mac

If this directory lives in your repo on the Mac, copy it to the Pi with:

```bash
scp -r raspberry_pi/petcam ziyangucliot@ZiyangRaspi.local:~
```

Or use the Pi IP instead of `.local`.
