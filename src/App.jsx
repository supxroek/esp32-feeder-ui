import { BrowserRouter as Router, Routes, Route } from "react-router-dom";
import Home from "./pages/Home";
import Control from "./pages/Control";
import { useState } from "react";

export default function App() {
  const [esp32Ip, setEsp32Ip] = useState("");
  const [esp32Port, setEsp32Port] = useState("");

  return (
    <Router>
      <Routes>
        <Route
          path="/"
          element={<Home setEsp32Ip={setEsp32Ip} setEsp32Port={setEsp32Port} />}
        />
        <Route
          path="/control"
          element={<Control esp32Ip={esp32Ip} esp32Port={esp32Port} />}
        />
      </Routes>
    </Router>
  );
}
