#include <Wire.h>
#include <ESP8266WiFi.h>

// Wi-Fi credentials
const char* ssid = "Your_Receiver_SSID";
const char* password = "Your_Receiver_Password";
const char* serverIP = "192.168.4.1";  // Receiver ESP IP (AP mode)
const uint16_t serverPort = 1234;

WiFiClient client;

// MPU6050 I2C address
const int MPU_ADDR = 0x68;

void setup() {
  Serial.begin(115200);
  Wire.begin(D2, D1);  // SDA, SCL pins for ESP8266

  // Initialize MPU6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);  // Power management register
  Wire.write(0);     // Wake up the sensor
  Wire.endTransmission();

  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
}

void loop() {
  // Connect to server if not already connected
  if (!client.connected()) {
    Serial.println("Connecting to server...");
    if (client.connect(serverIP, serverPort)) {
      Serial.println("Connected to server");
    } else {
      Serial.println("Failed to connect to server");
      delay(5000);
      return;
    }
  }

  // Read accelerometer data from MPU6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);  // Starting register for accelerometer data
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);  // Request 6 bytes (accel X, Y, Z)

  int16_t accelX = Wire.read() << 8 | Wire.read();
  int16_t accelY = Wire.read() << 8 | Wire.read();
  int16_t accelZ = Wire.read() << 8 | Wire.read();

  // Determine car command based on tilt
  String command = "stop";  // Default command

  if (accelY > 15000) {
    command = "forward";
  } else if (accelY < -15000) {
    command = "backward";
  } else if (accelX > 15000) {
    command = "right";
  } else if (accelX < -15000) {
    command = "left";
  }

  // Send command to the receiver
  client.println(command);
  Serial.println("Sent command: " + command);

  delay(100);  // Adjust delay for responsiveness
}
