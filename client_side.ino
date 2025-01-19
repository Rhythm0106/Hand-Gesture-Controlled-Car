#include <ESP8266WiFi.h>

// Wi-Fi credentials
const char* ssid = "";
const char* password = "";
const uint16_t serverPort = 1234;

WiFiServer server(serverPort);
WiFiClient client;

// Motor control pins (adjust as per your wiring)
const int IN1 = D1;
const int IN2 = D2;
const int IN3 = D3;
const int IN4 = D4;

void setup() {
  Serial.begin(115200);

  // Configure motor pins as output
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Initialize motor pins to LOW
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

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
    client = server.available();  // Wait for a new client
    if (client) {
      Serial.println("Client connected");
    }
    return;
  }

  // Check if data is available from the client
  if (client.available()) {
    String command = client.readStringUntil('\n');
    command.trim();  // Remove extra whitespace
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
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Function to move backward
void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Function to turn left
void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Function to turn right
void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Function to stop the motors
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}