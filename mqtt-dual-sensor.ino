#include <ESP8266WiFi.h>     // Include this if you're using ESP8266. Use <WiFi.h> for ESP32.
#include <PubSubClient.h>     // Include MQTT client library

// Wi-Fi credentials
const char* ssid = "yourSSID";
const char* password = "yourPassword";

// MQTT Broker settings
const char* mqttServer = "mqtt.server.address"; // Enter the address of your MQTT broker
const int mqttPort = 1883;                      // Default port for MQTT
const char* mqttUser = "yourUsername";          // Optional: if your MQTT broker requires authentication
const char* mqttPassword = "yourPassword";      // Optional: if your MQTT broker requires authentication
const char* mqttClientID = "SensorClient";      // Unique client ID for this device

WiFiClient espClient;
PubSubClient client(espClient);

// Pin definitions
#define triggerPin1 D1
#define echoPin1    D2
#define triggerPin2 D3
#define echoPin2    D4
#define buzzer      D0

// Distance threshold in centimeters
#define THRESHOLD   50

void setup() {
  // Start serial monitor
  Serial.begin(9600);
  
  // Initialize pins
  pinMode(triggerPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(triggerPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  // Connect to Wi-Fi
  connectToWiFi();
  
  // Initialize MQTT
  client.setServer(mqttServer, mqttPort);
  client.setCallback(mqttCallback);

  Serial.println("Dual ultrasonic sensors are ready!");
}

// Wi-Fi connection function
void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println(" Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// MQTT reconnect function
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    if (client.connect(mqttClientID, mqttUser, mqttPassword)) {
      Serial.println(" connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

// Function to measure distance from a specific sensor
long measureDistance(int triggerPin, int echoPin) {
  long duration, distance;
  
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0343 / 2;
  
  return distance;
}

// MQTT callback (called when a message is received)
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // Placeholder for any action when an MQTT message is received
}

void loop() {
  // Reconnect to MQTT server if needed
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  // Measure distances
  long distance1 = measureDistance(triggerPin1, echoPin1);
  long distance2 = measureDistance(triggerPin2, echoPin2);

  // Print distances to serial monitor
  Serial.print("Sensor 1 Jarak: ");
  Serial.print(distance1);
  Serial.println(" cm");

  Serial.print("Sensor 2 Jarak: ");
  Serial.print(distance2);
  Serial.println(" cm");

  // If either sensor detects object within threshold, activate buzzer and send MQTT message
  if (distance1 <= THRESHOLD || distance2 <= THRESHOLD) {
    Serial.println("Objek terdeteksi oleh salah satu sensor!");
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
    
    // Send MQTT message (you can customize the topic and payload)
    String message = "Objek terdeteksi pada Sensor 1: " + String(distance1) + " cm, Sensor 2: " + String(distance2) + " cm";
    client.publish("home/sensor/distance", message.c_str());
  } else {
    digitalWrite(buzzer, LOW);
  }

  delay(200);
}
