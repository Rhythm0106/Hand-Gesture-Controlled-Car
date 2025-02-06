#include <WiFi.h>
#include <Wire.h>
#include <MPU6050.h>

const char* ssid = "10";           // Same SSID as car ESP32
const char* password = "miaomiao*"; // Same password as car ESP32
const char* carIP = "192.168.4.1";  // Default IP of ESP32 AP mode
const uint16_t serverPort = 1234;

WiFiClient client;
MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Initialize MPU6050
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }
  Serial.println("MPU6050 connected!");

  // Connect to the car's ESP32 Wi-Fi (AP Mode)
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Car...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to Car!");
}

void loop() {
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  String command = "stop";  // Default to stop

  // Movement logic (adjust thresholds as needed)
  if (ay > 10000) {
    command = "forward";
  } else if (ay < -10000) {
    command = "backward";
  } else if (ax > 5000) {
    command = "right";
  } else if (ax < -5000) {
    command = "left";
  }

  // Send command only if Wi-Fi is connected
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      client.connect(carIP, serverPort);
    }

    if (client.connected()) {
      client.println(command);
      Serial.println("Sent: " + command);
    }
  }

  delay(200);  // Adjust responsiveness
}
