import os
from ultralytics import YOLO
import cv2
import requests
import torch
import time

# โหลดโมเดล YOLOv8 และตรวจสอบว่าใช้ GPU ได้หรือไม่
device = "cuda" if torch.cuda.is_available() else "cpu"
model = YOLO("yolov8s.pt").to(device)

# URL ของ IP Webcam (ปรับให้ดึงวิดีโอความละเอียดต่ำลง)
IP_WEBCAM_URL = "http://192.168.32.236:8080/video?640x480"

# ตั้งค่า Line Notify API
LINE_TOKEN = "yf34O2iOaX96TznK2CmCC0xGJdDn3rDfZray6sdWBUt"
LINE_API_URL = "https://notify-api.line.me/api/notify"
LINE_HEADERS = {"Authorization": f"Bearer {LINE_TOKEN}"}

def send_line_notify(message, image_path):
    """ ส่งข้อความและภาพแจ้งเตือนผ่าน Line Notify """
    with open(image_path, 'rb') as img_file:
        files = {'imageFile': img_file}
        data = {'message': message}
        response = requests.post(LINE_API_URL, headers=LINE_HEADERS, files=files, data=data)
        if response.status_code == 200:
            print("แจ้งเตือนสำเร็จ!")
        else:
            print(f"การแจ้งเตือนล้มเหลว: {response.status_code}")

# สร้างโฟลเดอร์สำหรับเก็บภาพถ้ายังไม่มี
output_folder = os.path.join(os.path.dirname(__file__), 'detected_images')
os.makedirs(output_folder, exist_ok=True)

# อ่านภาพจาก IP Webcam
cap = cv2.VideoCapture(IP_WEBCAM_URL)
cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)  # ลด buffer เพื่อลดดีเลย์

frame_skip = 3  # ข้ามเฟรมทุก 3 เฟรม
frame_count = 0
last_notification_time = 0  # เวลาที่ส่งการแจ้งเตือนครั้งล่าสุด
notification_interval = 10 * 60  # 10 นาทีในหน่วยวินาที

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break

    frame_count += 1
    if frame_count % frame_skip != 0:
        continue  # ข้ามบางเฟรมเพื่อเพิ่มความเร็ว

    # ลดขนาดภาพก่อนประมวลผล (ลดภาระของ YOLO)
    frame_resized = cv2.resize(frame, (640, 480))

    # ตรวจจับวัตถุด้วย YOLOv8
    results = model(frame_resized)

    for r in results:
        for box in r.boxes:
            class_id = int(box.cls[0])  # หมายเลขคลาส
            conf = box.conf[0]  # ความมั่นใจ
            x1, y1, x2, y2 = map(int, box.xyxy[0])  # พิกัดกรอบ

            # ตรวจจับเฉพาะแมว (คลาส 15 ใน COCO dataset)
            if class_id == 15 and conf > 0.5:
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                cv2.putText(frame, "Cat Detected!", (x1, y1 - 10),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

                # ตรวจสอบเวลาที่ผ่านไปก่อนส่งการแจ้งเตือนครั้งใหม่
                current_time = time.time()
                if current_time - last_notification_time >= notification_interval:
                    # บันทึกภาพที่มีการตรวจพบแมว
                    file_name = os.path.join(output_folder, 'detected_cat.png')
                    cv2.imwrite(file_name, frame)

                    # ส่งภาพไปยัง Line Notify
                    send_line_notify("🐱 พบแมวในกล้อง!", file_name)

                    # อัปเดตเวลาที่ส่งการแจ้งเตือนครั้งล่าสุด
                    last_notification_time = current_time

    # แสดงผลภาพ
    cv2.imshow("YOLOv8 Cat Detection", frame)

    # ปิดโปรแกรมเมื่อกด ESC
    if cv2.waitKey(1) == 27:
        break

cap.release()
cv2.destroyAllWindows()
