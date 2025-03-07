// #include <WiFi.h>
// #include <ESPAsyncWebServer.h>
// #include <ESP32Servo.h>
// #include <HCSR04.h>

// #define BLYNK_TEMPLATE_ID "TMPL69YZEIpQg"
// #define BLYNK_TEMPLATE_NAME "Quickstart Template"
// #define BLYNK_AUTH_TOKEN "5rsXENIy4LgykP02JQB5ZppAo7Cm-4aT"

// #define TRIG_PIN 5  // ขา Trigger ของ Ultrasonic Sensor
// #define ECHO_PIN 18 // ขา Echo ของ Ultrasonic Sensor
// #define SERVO_PIN 4 // ขาเชื่อมต่อ Servo

// const char* ssid = "Galaxy S24";     // ชื่อ WiFi
// const char* password = "oaffan20";   // รหัสผ่าน WiFi

// Servo foodServo;            // อ็อบเจ็กต์ Servo
// AsyncWebServer server(80);  // Web Server ที่พอร์ต 80

// bool objectDetected = false; // ตัวแปรเช็ควัตถุ (false = ไม่มี, true = มี)

// // 📌 ฟังก์ชันเชื่อมต่อ WiFi
// void connectWiFi() {
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.println("กำลังเชื่อมต่อ WiFi...");
//   }
//   Serial.print("✅ เชื่อมต่อ WiFi สำเร็จ! IP: ");
//   Serial.println(WiFi.localIP());
// }

// // 📌 ฟังก์ชันให้อาหาร (หมุน Servo)
// void feed() {
//   Serial.println("📢 รับคำสั่งให้อาหาร!");
//   foodServo.write(90);  // หมุนไป 90°
//   delay(1000);          // รอ 1 วินาที
//   foodServo.write(0);   // กลับไปตำแหน่งเดิม (0°)
//   Serial.println("✅ ให้อาหารเสร็จแล้ว!");
// }

// void setup() {
//   Serial.begin(115200);

//   pinMode(TRIG_PIN, OUTPUT);
//   pinMode(ECHO_PIN, INPUT);

//   foodServo.attach(SERVO_PIN, 400, 2000); // ตั้งค่า Servo
//   foodServo.write(0);
//   connectWiFi(); // เชื่อมต่อ WiFi

//   //ตั้งค่า API "/feed" สำหรับให้อาหารผ่านเว็บ
//   server.on("/feed", HTTP_GET, [](AsyncWebServerRequest* request) {
//     feed(); // เรียกใช้ฟังก์ชันให้อาหาร
//     request->send(200, "text/plain", "ให้อาหารแล้ว!");
//   });

//   server.begin();
// }

// void loop() {
//   digitalWrite(TRIG_PIN, LOW);
//   delayMicroseconds(2);
//   digitalWrite(TRIG_PIN, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(TRIG_PIN, LOW);

//   long duration = pulseIn(ECHO_PIN, HIGH);
//   float distance = duration * 0.0343 / 2; // แปลงเป็น cm

//   Serial.print("📏 ระยะวัตถุ: ");
//   Serial.print(distance);
//   Serial.println(" cm");

//   delay(1000);

//   if (distance > 0 && distance <= 20) {  // ถ้าอยู่ใกล้ ≤ 10 ซม.
//     if (!objectDetected) {  // ถ้าเป็นการเข้ามาครั้งแรก
//       Serial.println("🔴 วัตถุเข้าใกล้! กำลังให้อาหาร...");
//       feed(); // เรียกใช้ฟังก์ชันให้อาหาร
//       objectDetected = true;  // เปลี่ยนสถานะเป็น "มีวัตถุ"
//     }
//   } else {  
//     objectDetected = false;  // ถ้าวัตถุออกไป รีเซ็ตให้พร้อมให้อาหารรอบต่อไป
//   }


// }


#define BLYNK_PRINT Serial  // เปิดโหมด Debug ของ Blynk
#define BLYNK_TEMPLATE_ID "TMPL69YZEIpQg"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include <HCSR04.h>

#define BLYNK_AUTH_TOKEN "oqBzHLcrd4JxAipMbyaxO3kSBtsbhe8A"

#define TRIG_PIN 5  // ขา Trigger ของ Ultrasonic Sensor
#define ECHO_PIN 18 // ขา Echo ของ Ultrasonic Sensor
#define SERVO_PIN 4 // ขาเชื่อมต่อ Servo

const char* ssid = "Galaxy S24";     // ชื่อ WiFi
const char* password = "oaffan20";   // รหัสผ่าน WiFi

Servo foodServo;            // อ็อบเจ็กต์ Servo
BlynkTimer timer;           // Timer สำหรับ Blynk

bool objectDetected = false; // ตัวแปรเช็ควัตถุ (false = ไม่มี, true = มี)

// 📌 ฟังก์ชันเชื่อมต่อ WiFi
void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("กำลังเชื่อมต่อ WiFi...");
  }
  Serial.print("✅ เชื่อมต่อ WiFi สำเร็จ! IP: ");
  Serial.println(WiFi.localIP());
}

// 📌 ฟังก์ชันให้อาหาร (หมุน Servo)
void feed() {
  Serial.println("📢 รับคำสั่งให้อาหาร!");
  foodServo.write(90);  // หมุนไป 90°
  delay(1000);          // รอ 1 วินาที
  foodServo.write(0);   // กลับไปตำแหน่งเดิม (0°)
  Serial.println("✅ ให้อาหารเสร็จแล้ว!");
}

// 📌 ฟังก์ชันส่งระยะทางไปยัง Blynk
void sendDistanceToBlynk(float distance) {
  Blynk.virtualWrite(V0, distance);  // ส่งข้อมูลระยะทางไปยัง Virtual Pin V1
}

// 📌 ฟังก์ชันควบคุมการหมุน Servo ผ่าน Blynk
BLYNK_WRITE(V2) {  
  int servoPos = param.asInt();  // รับค่าจาก Blynk (ค่าของ slider หรือปุ่ม)
  foodServo.write(servoPos);     // หมุน Servo ตามค่าที่ได้รับ
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  foodServo.attach(SERVO_PIN, 400, 2000); // ตั้งค่า Servo
  foodServo.write(0);
  
  connectWiFi();  // เชื่อมต่อ WiFi
  Serial.print("WiFi Status: ");
  Serial.println(WiFi.status());
  delay(2000);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password, "blynk.cloud", 80);  // เชื่อมต่อกับ Blynk
  Blynk.config(BLYNK_AUTH_TOKEN, "188.166.206.43", 80);
  Blynk.connect();
  Serial.println("✅ เชื่อมต่อ Blynk สำเร็จ!");

}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.0343 / 2; // แปลงเป็น cm

  Serial.print("📏 ระยะวัตถุ: ");
  Serial.print(distance);
  Serial.println(" cm");

  sendDistanceToBlynk(distance);
  delay(1000);

  if (distance > 0 && distance <= 20) {  // ถ้าอยู่ใกล้ ≤ 10 ซม.
    if (!objectDetected) {  // ถ้าเป็นการเข้ามาครั้งแรก
      Serial.println("🔴 วัตถุเข้าใกล้! กำลังให้อาหาร...");
      feed(); // เรียกใช้ฟังก์ชันให้อาหาร
      objectDetected = true;  // เปลี่ยนสถานะเป็น "มีวัตถุ"
    }
  } else {  
    objectDetected = false;  // ถ้าวัตถุออกไป รีเซ็ตให้พร้อมให้อาหารรอบต่อไป
  }

}

