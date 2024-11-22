const express = require("express");
const bodyParser = require("body-parser");
const os = require("os");

const app = express();
const port = 3000; 

app.use(bodyParser.json()); //Parsear JSON


function getIPAddress() {
    const interfaces = os.networkInterfaces();
    for (const interfaceName in interfaces) {
        for (const iface of interfaces[interfaceName]) {
        if (iface.family === "IPv4" && !iface.internal) {
            return iface.address;
        }
        }
    }
    return "127.0.0.1"; // Fallback address
    }



app.get("/", (req, res) => {
    res.send("<h1>IoT ESP32 Test</h1><p>Usa el siguiente endpoint para mandar información:</p><ul><li><b>POST:</b> /api/sensors</li></ul>");
  });

  app.get("/api/sensores", (req, res) => {
    res.send("<h1>IoT ESP32 Test</h1><p>Revisa la consola</p>");
  });
// POST: Add new sensor data
app.post("/api/sensores", (req, res) => {
  const  values  = req.body;
  console.log(values);
  // Validate input
  if (values === undefined) {
    return res.status(400).json({
      success: false,
      message: "no se recibieron valores",
    });
  }

  res.status(201).json({
    success: true,
  });

  
});


//Iniciar el servidor
app.listen(port, () => {
    const ipAddress = getIPAddress();
    console.log(`Listening...`);
    console.log(`- Localhost: http://localhost:${port}`);
    console.log(`- Network: http://${ipAddress}:${port}`);
  });
