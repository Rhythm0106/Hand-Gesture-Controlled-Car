#include <WiFi.h>

// Wi-Fi credentials
const char* ssid = "";           // Replace with your Wi-Fi SSID
const char* password = "";   // Replace with your Wi-Fi Password
const uint16_t serverPort = 1234;

WiFiServer server(serverPort);
WiFiClient client;

// Motor control pins for L298N (adjust as per your wiring)
// First L298N Motor (Left Wheels)
const int L298N1_IN1 = 32;  // Left Wheel 1 - IN1 (GPIO32)
const int L298N1_IN2 = 33;  // Left Wheel 1 - IN2 (GPIO33)
const int L298N1_IN3 = 25;  // Left Wheel 2 - IN3 (GPIO25)
const int L298N1_IN4 = 26;  // Left Wheel 2 - IN4 (GPIO26)
const int L298N1_EN1 = 27;  // Enable pin for Left Wheel 1 (GPIO27)
const int L298N1_EN2 = 14;  // Enable pin for Left Wheel 2 (GPIO14)

// Second L298N Motor (Right Wheels)
const int L298N2_IN1 = 12;  // Right Wheel 1 - IN1 (GPIO12)
const int L298N2_IN2 = 13;  // Right Wheel 1 - IN2 (GPIO13)
const int L298N2_IN3 = 2;   // Right Wheel 2 - IN3 (GPIO2)
const int L298N2_IN4 = 15;  // Right Wheel 2 - IN4 (GPIO15)
const int L298N2_EN1 = 4;   // Enable pin for Right Wheel 1 (GPIO4)
const int L298N2_EN2 = 5;   // Enable pin for Right Wheel 2 (GPIO5)

void setup() {
  Serial.begin(115200);

  // Configure motor pins as output
  pinMode(L298N1_IN1, OUTPUT);
  pinMode(L298N1_IN2, OUTPUT);
  pinMode(L298N1_IN3, OUTPUT);
  pinMode(L298N1_IN4, OUTPUT);
  pinMode(L298N1_EN1, OUTPUT);
  pinMode(L298N1_EN2, OUTPUT);

  pinMode(L298N2_IN1, OUTPUT);
  pinMode(L298N2_IN2, OUTPUT);
  pinMode(L298N2_IN3, OUTPUT);
  pinMode(L298N2_IN4, OUTPUT);
  pinMode(L298N2_EN1, OUTPUT);
  pinMode(L298N2_EN2, OUTPUT);

  // Initialize all motor pins to LOW
  stopMotors();

  // Start Wi-Fi in AP mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP Address: ");
  Serial.println(IP);

  server.begin();
  Serial.println("Server started");
}

void loop() {
  if (!client || !client.connected()) {
    client = server.available(); // Wait for a new client
    if (client) {
      Serial.println("Client connected");
    }
    return;
  }

  // Check if data is available from the client
  if (client.available()) {
    String command = client.readStringUntil('\n');
    command.trim(); // Remove extra whitespace
    Serial.println("Received command: " + command);

    // Control the car based on the command
    if (command == "forward") {
      moveForward();
    } else if (command == "backward") {
      moveBackward();
    } else if (command == "left") {
      turnLeft();
    } else if (command == "right") {
      turnRight();
    } else if (command == "stop") {
      stopMotors();
    }
  }
}

// Function to move forward
void moveForward() {
  // Left Wheels
  digitalWrite(L298N1_IN1, HIGH);
  digitalWrite(L298N1_IN2, LOW);
  digitalWrite(L298N1_IN3, LOW);
  digitalWrite(L298N1_IN4, HIGH);
  // Right Wheels
  digitalWrite(L298N2_IN1, HIGH);
  digitalWrite(L298N2_IN2, LOW);
  digitalWrite(L298N2_IN3, LOW);
  digitalWrite(L298N2_IN4, HIGH);

  // Enable motors for speed control (PWM)
  analogWrite(L298N1_EN1, 128); // Max speed for Left Wheel 1
  analogWrite(L298N1_EN2, 128); // Max speed for Left Wheel 2
  analogWrite(L298N2_EN1, 128); // Max speed for Right Wheel 1
  analogWrite(L298N2_EN2, 128); // Max speed for Right Wheel 2
}

// Function to move backward
void moveBackward() {
  
   // Left Wheels
  digitalWrite(L298N1_IN1, LOW);
  digitalWrite(L298N1_IN2, HIGH);
  digitalWrite(L298N1_IN3, HIGH);
  digitalWrite(L298N1_IN4, LOW);
  // Right Wheels
  digitalWrite(L298N2_IN1, LOW);
  digitalWrite(L298N2_IN2, HIGH);
  digitalWrite(L298N2_IN3, HIGH);
  digitalWrite(L298N2_IN4, LOW);

  // Enable motors for speed control (PWM)
  analogWrite(L298N1_EN1, 128); // Max speed for Left Wheel 1
  analogWrite(L298N1_EN2, 128); // Max speed for Left Wheel 2
  analogWrite(L298N2_EN1, 128); // Max speed for Right Wheel 1
  analogWrite(L298N2_EN2, 128); // Max speed for Right Wheel 2
}

// Function to turn left
void turnLeft() {
  // Left Wheels (Stop)
  digitalWrite(L298N1_IN1, LOW);
  digitalWrite(L298N1_IN2, LOW);
  digitalWrite(L298N1_IN3, LOW);
  digitalWrite(L298N1_IN4, LOW);
  // Right Wheels (Move forward)
  digitalWrite(L298N2_IN1, HIGH);
  digitalWrite(L298N2_IN2, LOW);
  digitalWrite(L298N2_IN3, LOW);
  digitalWrite(L298N2_IN4, HIGH);

  // Enable motors for speed control (PWM)
  analogWrite(L298N2_EN1, 60); // Max speed for Right Wheel 1
  analogWrite(L298N2_EN2, 60); // Max speed for Right Wheel 2
}

// Function to turn right
void turnRight() {
  // Left Wheels (Move forward)
  digitalWrite(L298N1_IN1, HIGH);
  digitalWrite(L298N1_IN2, LOW);
  digitalWrite(L298N1_IN3, LOW);
  digitalWrite(L298N1_IN4, HIGH);
  // Right Wheels (Stop)
  digitalWrite(L298N2_IN1, LOW);
  digitalWrite(L298N2_IN2, LOW);
  digitalWrite(L298N2_IN3, LOW);
  digitalWrite(L298N2_IN4, LOW);

  // Enable motors for speed control (PWM)
  analogWrite(L298N1_EN1, 60); // Max speed for Left Wheel 1
  analogWrite(L298N1_EN2, 60); // Max speed for Left Wheel 2
}

// Function to stop all motors
void stopMotors() {
  // Left Wheels
  digitalWrite(L298N1_IN1, LOW);
  digitalWrite(L298N1_IN2, LOW);
  digitalWrite(L298N1_IN3, LOW);
  digitalWrite(L298N1_IN4, LOW);
  // Right Wheels
  digitalWrite(L298N2_IN1, LOW);
  digitalWrite(L298N2_IN2, LOW);
  digitalWrite(L298N2_IN3, LOW);
  digitalWrite(L298N2_IN4, LOW);

  // Disable motors (PWM)
  analogWrite(L298N1_EN1, 0);  // Stop Left Wheel 1
  analogWrite(L298N1_EN2, 0);  // Stop Left Wheel 2
  analogWrite(L298N2_EN1, 0);  // Stop Right Wheel 1
  analogWrite(L298N2_EN2, 0);  // Stop Right Wheel 2
}
