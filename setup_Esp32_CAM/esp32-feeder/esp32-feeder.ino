#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include <NewPing.h>
#include "esp_camera.h"

#define CAMERA_MODEL_AI_THINKER  // Has PSRAM

// WiFi Credentials
const char *ssid = "Galaxy S24";
const char *password = "oaffan20";

// Server Setup
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Servo Setup
Servo feederServo;
const int servoPin = 13;

// Ultrasonic Sensor Setup
#define TRIGGER_PIN 14
#define ECHO_PIN 15
#define MAX_DISTANCE 200
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Camera Configuration
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22



bool foodDispensed = false;

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Ready!");

  // เชื่อมต่อ WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected to WiFi");
  Serial.println(WiFi.localIP());

  // ตั้งค่าเซอร์โวมอเตอร์
  feederServo.attach(servoPin);
  feederServo.write(0);

  // ตั้งค่ากล้อง
  camera_config_t config;
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
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.println("Camera Init Failed");
    return;
  }

  // ตั้งค่า WebSocket
  ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_DATA) {
      String msg = (char *)data;
      if (msg == "FEED") {
        dispenseFood();
      }
    }
  });
  server.addHandler(&ws);

  // เริ่มต้น Web Server
  Serial.println("Starting Web Server...");
  server.begin();
  Serial.println("Web Server Started!");

  //Test Web Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hello from ESP32!");
  });

  //Camera Stream
  server.on("/stream", HTTP_GET, [](AsyncWebServerRequest *request) {
    WiFiClient client = request->beginResponseStream("image/jpeg");
    esp_camera_fb_get();  // ถ่ายภาพจากกล้อง
    client.write(camera_fb->buf, camera_fb->len);
    esp_camera_fb_return(camera_fb);  // คืนค่า buffer หลังการใช้งาน
  });
}

void dispenseFood() {
  int distance = sonar.ping_cm();

  Serial.print("Measured Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // ตรวจสอบว่ามีอาหารค้างอยู่หรือไม่
  if (distance > 10 || distance == 0) {
    Serial.println("Dispensing Food...");
    feederServo.write(90);
    delay(2000);
    feederServo.write(0);
    foodDispensed = true;
  } else {
    Serial.println("Food still present, skipping dispense.");
  }
}

void loop() {
  ws.cleanupClients();
  delay(1000);
}
