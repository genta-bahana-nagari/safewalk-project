#define triggerPin D1
#define echoPin D2
#define buzzer D5

// Distance threshold in centimeters
#define THRESHOLD 50

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Configure pin modes
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  
  // Initial state of buzzer - off
  digitalWrite(buzzer, LOW);
  
  Serial.println("Sensor jarak ultrasonic siap!");
}

void loop() {
  // Variables for duration and distance calculation
  long duration, jarak;
  
  // Clear the trigger pin
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  
  // Set the trigger pin high for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  // Read the echo pin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance in centimeters
  // Speed of sound is 343 m/s or 0.0343 cm/microsecond
  // Distance = (time × speed) / 2 (divided by 2 for round trip)
  jarak = duration * 0.0343 / 2;
  
  // Print the distance on the Serial Monitor
  Serial.print("Jarak: ");
  Serial.print(jarak);
  Serial.println(" cm");
  
  // Check if an object is detected within threshold distance
  if (jarak <= THRESHOLD) {
    // Object detected within threshold, turn on the buzzer
    Serial.println("Objek terdeteksi dalam jarak threshold!");
    digitalWrite(buzzer, HIGH);
  } else {
    // No object detected within threshold, turn off the buzzer
    digitalWrite(buzzer, LOW);
  }
  
  // Wait a bit before the next reading
  delay(2000);
}
