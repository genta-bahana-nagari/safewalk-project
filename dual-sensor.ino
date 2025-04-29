#define triggerPin1 D1
#define echoPin1    D2

#define triggerPin2 D3
#define echoPin2    D4

#define buzzer      D6

// Distance threshold in centimeters
#define THRESHOLD   50

void setup() {
  Serial.begin(9600);
  
  // Sensor 1 pin modes
  pinMode(triggerPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  
  // Sensor 2 pin modes
  pinMode(triggerPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  
  // Buzzer pin
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  Serial.println("Dual ultrasonic sensors are ready!");
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

void loop() {
  long distance1 = measureDistance(triggerPin1, echoPin1);
  long distance2 = measureDistance(triggerPin2, echoPin2);
  
  Serial.print("Sensor 1 Jarak: ");
  Serial.print(distance1);
  Serial.println(" cm");

  Serial.print("Sensor 2 Jarak: ");
  Serial.print(distance2);
  Serial.println(" cm");

  // If either sensor detects object within threshold, activate buzzer
  if (distance1 <= THRESHOLD || distance2 <= THRESHOLD) {
    Serial.println("Objek terdeteksi oleh salah satu sensor!");
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
  } else {
    digitalWrite(buzzer, LOW);
  }

  delay(200);
}
