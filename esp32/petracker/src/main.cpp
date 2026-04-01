#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "esp_camera.h"

namespace {

constexpr char kWifiSsid[] = "Tangyi";
constexpr char kWifiPassword[] = "hty88888888";

WebServer server(80);

constexpr char kIndexHtml[] PROGMEM = R"HTML(
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32-CAM Live View</title>
  <style>
    :root {
      color-scheme: dark;
      --bg: #101418;
      --panel: #1b2229;
      --text: #ecf2f8;
      --muted: #91a0b0;
      --accent: #58d68d;
    }
    * { box-sizing: border-box; }
    body {
      margin: 0;
      min-height: 100vh;
      display: grid;
      place-items: center;
      background: radial-gradient(circle at top, #263545 0%, var(--bg) 55%);
      color: var(--text);
      font-family: ui-sans-serif, system-ui, sans-serif;
    }
    main {
      width: min(92vw, 960px);
      background: rgba(27, 34, 41, 0.92);
      border: 1px solid rgba(255,255,255,0.08);
      border-radius: 20px;
      padding: 20px;
      box-shadow: 0 20px 60px rgba(0, 0, 0, 0.35);
    }
    h1 { margin: 0 0 8px; font-size: 1.4rem; }
    p { margin: 0 0 16px; color: var(--muted); }
    img {
      width: 100%;
      border-radius: 14px;
      background: #000;
      display: block;
      aspect-ratio: 4 / 3;
      object-fit: cover;
    }
    .actions {
      margin-top: 14px;
      display: flex;
      gap: 12px;
      flex-wrap: wrap;
    }
    a {
      color: var(--accent);
      text-decoration: none;
      font-weight: 600;
    }
  </style>
</head>
<body>
  <main>
    <h1>ESP32-CAM Live View</h1>
    <p>If the image stays blank, check the serial monitor for camera capture errors.</p>
    <img src="/stream" alt="Live camera stream">
    <div class="actions">
      <a href="/capture" target="_blank" rel="noreferrer">Open single snapshot</a>
      <a href="/health" target="_blank" rel="noreferrer">Health check</a>
    </div>
  </main>
</body>
</html>
)HTML";

// AI Thinker ESP32-CAM pin map.
constexpr int PWDN_GPIO_NUM = 32;
constexpr int RESET_GPIO_NUM = -1;
constexpr int XCLK_GPIO_NUM = 0;
constexpr int SIOD_GPIO_NUM = 26;
constexpr int SIOC_GPIO_NUM = 27;

constexpr int Y9_GPIO_NUM = 35;
constexpr int Y8_GPIO_NUM = 34;
constexpr int Y7_GPIO_NUM = 39;
constexpr int Y6_GPIO_NUM = 36;
constexpr int Y5_GPIO_NUM = 21;
constexpr int Y4_GPIO_NUM = 19;
constexpr int Y3_GPIO_NUM = 18;
constexpr int Y2_GPIO_NUM = 5;
constexpr int VSYNC_GPIO_NUM = 25;
constexpr int HREF_GPIO_NUM = 23;
constexpr int PCLK_GPIO_NUM = 22;

bool initCamera() {
  camera_config_t config = {};
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  // Use conservative settings first. This board is sensitive to ribbon seating
  // and power quality, and lower capture pressure is much more reliable.
  config.xclk_freq_hz = 10000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 15;
  config.fb_count = 1;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_DRAM;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    return false;
  }

  sensor_t *sensor = esp_camera_sensor_get();
  sensor->set_vflip(sensor, 1);
  sensor->set_brightness(sensor, 1);
  sensor->set_saturation(sensor, -2);
  sensor->set_framesize(sensor, FRAMESIZE_QVGA);
  return true;
}

void handleHealth() {
  server.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleIndex() {
  server.send_P(200, "text/html; charset=utf-8", kIndexHtml);
}

void handleCapture() {
  Serial.println("Capture requested");
  camera_fb_t *fb = esp_camera_fb_get();
  if (fb == nullptr) {
    Serial.println("Capture failed: esp_camera_fb_get() returned null");
    server.send(500, "application/json", "{\"error\":\"capture_failed\"}");
    return;
  }

  Serial.printf("Capture OK: %u bytes\n", fb->len);

  server.sendHeader("Content-Type", "image/jpeg");
  server.sendHeader("Content-Length", String(fb->len));
  server.send(200);

  WiFiClient client = server.client();
  client.write(fb->buf, fb->len);
  esp_camera_fb_return(fb);
}

void handleStream() {
  Serial.println("Stream requested");

  WiFiClient client = server.client();
  client.printf(
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n"
      "Cache-Control: no-cache\r\n"
      "Pragma: no-cache\r\n"
      "Connection: close\r\n\r\n");

  while (client.connected()) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (fb == nullptr) {
      Serial.println("Stream frame failed: esp_camera_fb_get() returned null");
      delay(100);
      continue;
    }

    client.printf(
        "--frame\r\n"
        "Content-Type: image/jpeg\r\n"
        "Content-Length: %u\r\n\r\n",
        fb->len);
    client.write(fb->buf, fb->len);
    client.print("\r\n");
    esp_camera_fb_return(fb);

    if (!client.connected()) {
      break;
    }
    delay(60);
  }

  Serial.println("Stream ended");
}

void handleNotFound() {
  String message;
  message.reserve(128);
  message += "ESP32-CAM is running.\n";
  message += "GET /\n";
  message += "GET /health\n";
  message += "GET /capture\n";
  message += "GET /stream\n";
  server.send(200, "text/plain", message);
}

void connectWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(kWifiSsid, kWifiPassword);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
}

void startServer() {
  server.on("/", HTTP_GET, handleIndex);
  server.on("/health", HTTP_GET, handleHealth);
  server.on("/capture", HTTP_GET, handleCapture);
  server.on("/stream", HTTP_GET, handleStream);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(1500);

  if (!initCamera()) {
    Serial.println("Restarting in 5 seconds...");
    delay(5000);
    ESP.restart();
  }

  connectWifi();
  startServer();
}

void loop() {
  server.handleClient();
}
