#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>

const char* ssid = "Galaxy S24";    // ชื่อ WiFi
const char* password = "oaffan20";  // รหัสผ่าน WiFi

Servo foodServo;            // สร้างอ็อบเจ็กต์ Servo
AsyncWebServer server(80);  // Web Server ที่พอร์ต 80

void setup() {
  Serial.begin(115200);

  // เชื่อมต่อ WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("กำลังเชื่อมต่อ WiFi...");
  }

  Serial.print("เชื่อมต่อ WiFi สำเร็จ! IP: ");
  Serial.println(WiFi.localIP());

  // ตั้งค่า Servo (ต่อที่ขา GPIO4)
  foodServo.attach(4, 400, 1500);  // กำหนดช่วงสัญญาณ PWM (500 - 2500 µs)

  // เสิร์ฟ API ที่ "/feed"
  server.on("/feed", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("📢 รับคำสั่งให้อาหาร!");

    // หมุน Servo ให้จ่ายอาหาร (90°)
    foodServo.write(90);
    Serial.println("🔄 กำลังให้อาหาร... (หมุนไป 90°)");
    delay(1000);  // รอ 1 วินาที

    // กลับไปตำแหน่งเดิม (0°)
    foodServo.write(0);
    Serial.println("✅ ให้อาหารเสร็จแล้ว (กลับไป 0°)");

    // ให้อาหารสำเร็จ!
    Serial.println("✅ ให้อาหารสำเร็จ!");
  });

  server.begin();
}

void loop() {
}
