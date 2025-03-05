import { useState } from "react";
import { useNavigate } from "react-router-dom";

export default function Home({ setEsp32Ip, setEsp32Port }) {
  const [ip, setIp] = useState("");
  const [port, setPort] = useState("");
  const navigate = useNavigate();

  const handleConnect = () => {
    if (ip) {
      setEsp32Ip(ip);
      setEsp32Port(port);
      navigate("/control");
    } else {
      alert("กรุณากรอก IP ของ ESP32");
    }
  };

  return (
    <div className="flex flex-col items-center justify-center min-h-screen bg-gray-100">
      <h1 className="text-2xl font-bold text-gray-700 mb-4">
        เชื่อมต่อกับ ESP32
      </h1>
      <div>
        <input
          type="text"
          placeholder="กรอก ESP32 IP เช่น 192.168.1.100"
          value={ip}
          onChange={(e) => setIp(e.target.value)}
          className="px-4 py-2 border rounded-lg w-80"
        />
        <span className="text-2xl font-bold"> : </span>
        <input
          type="text"
          placeholder="กรอก Port เช่น 8080"
          value={port}
          onChange={(e) => setPort(e.target.value)}
          className="px-4 py-2 border rounded-lg w-auto"
        />
      </div>
      <button
        onClick={handleConnect}
        className="mt-4 bg-blue-500 text-white px-6 py-2 rounded-lg hover:bg-blue-600 transition"
      >
        Connect
      </button>
    </div>
  );
}
