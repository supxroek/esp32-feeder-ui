import { useEffect, useState } from "react";
import { handler } from "../netlify/functions/proxy";

export default function App() {
  //const [status, setStatus] = useState("");
  const [isCameraVisible, setCameraVisible] = useState(false);
  const [ipEsp32, setIpEsp32] = useState("192.168.32.16"); //Set Ip Esp32 Default
  const [ipCamera, setipCamera] = useState("192.168.32.236"); //Set Ip Camera Default
  const [portCamera, setPortCamera] = useState("8080"); //Set Port Default

  useEffect(() => {
    setipCamera(ipCamera);
    setPortCamera(portCamera);
    setIpEsp32(ipEsp32);
  }, [ipCamera, portCamera, ipEsp32]);

  // const handleFeed = async () => {
  //   setStatus("🍖 กำลังให้อาหาร...");
  //   try {
  //     //await fetch(`http://${ipEsp32}/feed`);
  //     await fetch("../netlify/functions/proxy");
  //   } catch {
  //     return setStatus("✅ ให้อาหารเรียบร้อยแล้ว!");
  //   }
  // };

  return (
    <div className="flex flex-col items-center min-h-screen bg-gradient-to-b from-green-100 to-green-300 p-6 gap-y-6">
      <h1 className="text-3xl font-bold text-green-700 drop-shadow-md">
        🐾 ควบคุมเครื่องให้อาหาร
      </h1>
      <div className="flex flex-col items-center justify-center">
        <h1 className="text-2xl font-bold text-gray-700 mb-4">
          เชื่อมต่อกับ IP Camera
        </h1>
        <div>
          <input
            type="text"
            placeholder="กรอก ESP32 IP เช่น 192.168.1.100"
            value={ipCamera}
            onChange={(e) => setipCamera(e.target.value)}
            className="px-4 py-2 border rounded-lg w-auto"
          />
          <span className="text-2xl font-bold"> : </span>
          <input
            type="text"
            placeholder="กรอก Port เช่น 8080"
            value={portCamera}
            onChange={(e) => setPortCamera(e.target.value)}
            className="px-4 py-2 border rounded-lg w-auto"
          />
        </div>
      </div>

      <div className="flex flex-col items-center justify-center">
        <h1 className="text-2xl font-bold text-gray-700 mb-4">
          เชื่อมต่อกับ IP ESP32
        </h1>
        <div>
          <input
            type="text"
            placeholder="กรอก ESP32 IP เช่น 192.168.1.100"
            value={ipEsp32}
            onChange={(e) => setIpEsp32(e.target.value)}
            className="px-4 py-2 border rounded-lg w-110"
          />
        </div>
      </div>
      <div className="flex flex-col items-center justify-center">
        {/* ปุ่มดูกล้อง */}
        <button
          onClick={() => setCameraVisible(!isCameraVisible)}
          className="mt-6 bg-gray-700 text-white px-8 py-3 rounded-lg shadow-lg hover:bg-gray-800 transition-transform transform active:scale-95"
        >
          {isCameraVisible ? "📷 ปิดกล้อง" : "🎥 ดูสัตว์เลี้ยง"}
        </button>

        {/* กล้องจาก ESP32 */}
        {isCameraVisible && (
          <div className="mt-4 w-full max-w-md rounded-lg overflow-hidden shadow-lg border border-gray-300 bg-white">
            <img
              className="w-full"
              src={`http://${ipCamera}:${portCamera}/video`} // IP ของ ESP32 ที่ใช้กับการดูวิดีโอ
              alt="Camera Stream"
            />
          </div>
        )}

        {/* ปุ่มให้อาหาร */}
        <button
          className="bg-blue-500 text-white px-8 py-3 rounded-lg shadow-lg hover:bg-blue-600 transition-transform transform active:scale-95 mt-6"
          //onClick={handleFeed}
          onClick={handler}
        >
          🍽️ ให้อาหาร
        </button>

        {/* สถานะ */}
        <p className="mt-4 text-lg font-semibold text-gray-800 bg-white px-4 py-2 rounded-lg shadow">
          {status}
        </p>
      </div>
    </div>
  );
}
