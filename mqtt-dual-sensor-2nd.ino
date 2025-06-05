#include <ESP8266WiFi.h>     // Untuk ESP8266
#include <PubSubClient.h>    // Library MQTT

// Wi-Fi credentials
const char* ssid = "yourSSID";           // Ganti dengan nama WiFi kamu
const char* password = "yourPassword";   // Ganti dengan password WiFi kamu

// MQTT HiveMQ broker (public)
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttClientID = "SensorClient123"; // Pastikan unik di antara device lain

WiFiClient espClient;
PubSubClient client(espClient);

// Pin sensor & buzzer
#define triggerPin1 D1
#define echoPin1    D2
#define triggerPin2 D3
#define echoPin2    D4
#define buzzer      D0

// Ambang batas (dalam cm)
#define THRESHOLD   50

void setup() {
  Serial.begin(9600);

  pinMode(triggerPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(triggerPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  connectToWiFi();

  client.setServer(mqttServer, mqttPort);
  client.setCallback(mqttCallback);

  Serial.println("Sensor ultrasonik siap!");
}

void connectToWiFi() {
  Serial.print("Menghubungkan ke WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println(" Terhubung!");
  Serial.print("Alamat IP: ");
  Serial.println(WiFi.localIP());
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT...");

    // HiveMQ public broker tidak butuh username/password
    if (client.connect(mqttClientID)) {
      Serial.println(" Terhubung ke MQTT");
    } else {
      Serial.print("Gagal, kode kesalahan=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

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

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // Tidak digunakan, karena hanya publish
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  long distance1 = measureDistance(triggerPin1, echoPin1);
  long distance2 = measureDistance(triggerPin2, echoPin2);

  Serial.print("Jarak Sensor 1: ");
  Serial.print(distance1);
  Serial.println(" cm");

  Serial.print("Jarak Sensor 2: ");
  Serial.print(distance2);
  Serial.println(" cm");

  if (distance1 <= THRESHOLD || distance2 <= THRESHOLD) {
    Serial.println("Objek terdeteksi!");
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);

    String message = "Sensor1: " + String(distance1) + " cm, Sensor2: " + String(distance2) + " cm";
    client.publish("home/sensor/distance", message.c_str());
  } else {
    digitalWrite(buzzer, LOW);
  }

  delay(200);
}
