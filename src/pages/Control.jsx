import { useState } from "react";
import { useNavigate } from "react-router-dom";

export default function Control({ esp32Ip, esp32Port }) {
  const [status, setStatus] = useState("รอคำสั่ง...");
  const [isCameraVisible, setCameraVisible] = useState(false);
  const navigate = useNavigate();

  const handleFeed = async () => {
    setStatus("กำลังให้อาหาร...");
    try {
      const response = await fetch(`http://${esp32Ip}/feed`);
      const data = await response.text();
      setStatus(data);
    } catch {
      setStatus("เกิดข้อผิดพลาด! กรุณาลองใหม่");
    }
  };

  return (
    <div className="flex flex-col items-center min-h-screen bg-gray-100 p-6">
      <h1 className="text-2xl font-bold text-green-600">
        ควบคุมเครื่องให้อาหาร
      </h1>

      {/* ปุ่มดูกล้อง */}
      <button
        onClick={() => setCameraVisible(!isCameraVisible)}
        className="mt-4 bg-gray-500 text-white px-6 py-2 rounded-lg hover:bg-gray-600 transition"
      >
        {isCameraVisible ? "ปิดกล้อง" : "ดูสัตว์เลี้ยง"}
      </button>

      {/* กล้องจาก ESP32 */}
      {isCameraVisible && (
        <img
          className="w-full max-w-md my-4 rounded-lg shadow-lg"
          src={`http://${esp32Ip}${esp32Port ? `:${esp32Port}` : ""}/stream`} // สำหรับการเข้าถึงกล้องด้วย ESP32 ที่เชื่อมต่อกับกล้อง
          //src={`http://${esp32Ip}${esp32Port ? `:${esp32Port}` : ""}/video`} // ทดสอบการเข้าถึงกล้องด้วยมือถือโดยใช้ IP และ Port
          //src={`http://10.1.1.92:8080/video`} // ทดสอบการเข้าถึงกล้องด้วยมือถือโดยใช้ IP และ Port
          alt="Camera Stream"
        />
      )}

      {/* ปุ่มให้อาหาร */}
      <button
        className="bg-blue-500 text-white px-6 py-2 rounded-lg hover:bg-blue-600 transition mt-4"
        onClick={handleFeed}
      >
        ให้อาหาร
      </button>

      {/* สถานะ */}
      <p className="mt-4 text-lg font-semibold">{status}</p>

      {/* ปุ่มกลับไปตั้งค่า IP */}
      <button
        className="mt-6 bg-red-500 text-white px-6 py-2 rounded-lg hover:bg-red-600 transition"
        onClick={() => navigate("/")}
      >
        กลับไปหน้าแรก
      </button>
    </div>
  );
}
