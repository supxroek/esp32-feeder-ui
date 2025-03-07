from flask import Flask, jsonify
from flask_cors import CORS
import requests

app = Flask(__name__)
CORS(app)  # อนุญาตให้ React ใช้ API นี้

ESP32_IP = "http://192.168.32.16"  # เปลี่ยนเป็น IP ของ ESP32

@app.route("/feed", methods=["GET"])
def feed():
    try:
        response = requests.get(f"{ESP32_IP}/feed")
        return jsonify(response.text)
    except:
        return jsonify({"error": "ไม่สามารถเชื่อมต่อกับ ESP32 ได้"}), 500

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
