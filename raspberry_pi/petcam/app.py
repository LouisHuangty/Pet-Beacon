from __future__ import annotations

import io
import signal
import threading
import time
from pathlib import Path

from flask import Flask, Response, jsonify, render_template_string, send_file
from picamera2 import Picamera2
from picamera2.encoders import JpegEncoder
from picamera2.outputs import FileOutput


BASE_DIR = Path(__file__).resolve().parent
CAPTURE_DIR = BASE_DIR / "captures"
CAPTURE_DIR.mkdir(exist_ok=True)

app = Flask(__name__)
camera_lock = threading.Lock()
picam2 = Picamera2()
video_config = picam2.create_video_configuration(
    main={"size": (1280, 720), "format": "RGB888"}
)
still_config = picam2.create_still_configuration(main={"size": (1920, 1080)})
picam2.configure(video_config)
picam2.start()
time.sleep(2)


INDEX_HTML = """
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>PetCam Stage 1</title>
  <style>
    :root {
      color-scheme: dark;
      --bg: #0f151c;
      --panel: #17202b;
      --text: #eef3f8;
      --muted: #94a3b8;
      --accent: #6ee7b7;
    }
    * { box-sizing: border-box; }
    body {
      margin: 0;
      min-height: 100vh;
      display: grid;
      place-items: center;
      background: radial-gradient(circle at top, #223247 0%, var(--bg) 60%);
      color: var(--text);
      font-family: ui-sans-serif, system-ui, sans-serif;
    }
    main {
      width: min(92vw, 960px);
      background: rgba(23, 32, 43, 0.94);
      border: 1px solid rgba(255,255,255,0.08);
      border-radius: 20px;
      padding: 20px;
      box-shadow: 0 24px 64px rgba(0, 0, 0, 0.38);
    }
    h1 { margin: 0 0 8px; }
    p { margin: 0 0 16px; color: var(--muted); }
    img {
      width: 100%;
      display: block;
      border-radius: 14px;
      background: #000;
      aspect-ratio: 16 / 9;
      object-fit: cover;
    }
    .actions {
      margin-top: 16px;
      display: flex;
      gap: 12px;
      flex-wrap: wrap;
    }
    a, button {
      appearance: none;
      border: none;
      border-radius: 999px;
      padding: 10px 16px;
      background: #233043;
      color: var(--text);
      cursor: pointer;
      text-decoration: none;
      font: inherit;
    }
    a.primary, button.primary {
      background: var(--accent);
      color: #06281d;
      font-weight: 700;
    }
    #result {
      margin-top: 16px;
      min-height: 24px;
      color: var(--muted);
      word-break: break-all;
    }
  </style>
</head>
<body>
  <main>
    <h1>PetCam Stage 1</h1>
    <p>Phase 1 prototype for live preview, single capture, and later signal-lost trigger integration.</p>
    <img src="/stream" alt="Live camera stream">
    <div class="actions">
      <button class="primary" onclick="capture()">Capture Now</button>
      <a href="/health" target="_blank" rel="noreferrer">Health</a>
      <a href="/capture" target="_blank" rel="noreferrer">Raw Capture Endpoint</a>
    </div>
    <div id="result"></div>
  </main>
  <script>
    async function capture() {
      const result = document.getElementById("result");
      result.textContent = "Capturing...";
      try {
        const response = await fetch("/capture-meta");
        const data = await response.json();
        if (!response.ok) {
          throw new Error(data.error || "capture failed");
        }
        result.innerHTML = `Saved: <a href="${data.image_url}" target="_blank" rel="noreferrer">${data.filename}</a>`;
      } catch (error) {
        result.textContent = `Error: ${error.message}`;
      }
    }
  </script>
</body>
</html>
"""


class StreamingOutput(io.BufferedIOBase):
    def __init__(self) -> None:
        self.frame = None
        self.condition = threading.Condition()

    def write(self, buf: bytes) -> int:
        with self.condition:
            self.frame = bytes(buf)
            self.condition.notify_all()
        return len(buf)


stream_output = StreamingOutput()
stream_encoder = JpegEncoder(q=80)
stream_output_target = FileOutput(stream_output)


def start_streaming() -> None:
    picam2.start_recording(stream_encoder, stream_output_target)


start_streaming()


def capture_file() -> tuple[Path, str]:
    timestamp = time.strftime("%Y%m%d-%H%M%S")
    path = CAPTURE_DIR / f"capture-{timestamp}.jpg"
    with camera_lock:
        request = picam2.capture_request()
        try:
            request.save("main", str(path))
        finally:
            request.release()
    return path, path.name


@app.get("/")
def index() -> str:
    return render_template_string(INDEX_HTML)


@app.get("/health")
def health():
    return jsonify({"status": "ok"})


@app.get("/stream")
def stream():
    def generate():
        while True:
            with stream_output.condition:
                stream_output.condition.wait()
                frame = stream_output.frame
            if frame is None:
                continue
            yield (
                b"--frame\r\n"
                b"Content-Type: image/jpeg\r\n\r\n" + frame + b"\r\n"
            )

    return Response(
        generate(),
        mimetype="multipart/x-mixed-replace; boundary=frame",
    )


@app.get("/capture")
def capture():
    path, _ = capture_file()
    return send_file(path, mimetype="image/jpeg")


@app.get("/capture-meta")
def capture_meta():
    path, filename = capture_file()
    return jsonify(
        {
            "status": "ok",
            "filename": filename,
            "image_url": f"/captures/{filename}",
        }
    )


@app.get("/captures/<path:filename>")
def get_capture(filename: str):
    return send_file(CAPTURE_DIR / filename, mimetype="image/jpeg")


def shutdown_handler(*_: object) -> None:
    try:
        picam2.stop_recording()
    finally:
        picam2.stop()
    raise SystemExit(0)


signal.signal(signal.SIGINT, shutdown_handler)
signal.signal(signal.SIGTERM, shutdown_handler)


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8000, threaded=True)
