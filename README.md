# Blind Stick Project - Safewalk

This project uses **two HC-SR04 ultrasonic sensors** to measure the distance of nearby objects. If **either sensor** detects an object closer than a defined threshold (50 cm by default), a **buzzer** is triggered.

## 🚀 Features

- Dual HC-SR04 sensor support
- Real-time distance measurement via Serial Monitor
- Buzzer alert when object detected within threshold distance
- Easy to customize and expand

## 🛠️ Hardware Required

- 1 × ESP8266 or NodeMCU (or any Arduino-compatible board)
- 2 × HC-SR04 ultrasonic sensors
- 1 × Buzzer (active or passive)
- Jumper wires
- Breadboard (optional)

## 🖥️ Pin Configuration

| Component       | Pin        |
|-----------------|------------|
| Sensor 1 TRIG   | D1         |
| Sensor 1 ECHO   | D2         |
| Sensor 2 TRIG   | D3         |
| Sensor 2 ECHO   | D4         |
| Buzzer          | D6         |

> ⚠️ Make sure your sensor's ECHO pins are 5V tolerant or use a voltage divider if needed.

## 📦 How to Use

1. Connect the components as described above.
2. Upload the sketch to your board using the Arduino IDE.
3. Open the **Serial Monitor** at 9600 baud.
4. Place objects near the sensors to test proximity detection.
5. When either sensor detects an object closer than the threshold, the buzzer will beep.

## 🧠 Code Overview

```cpp
#define triggerPin1 D1
#define echoPin1    D2
#define triggerPin2 D3
#define echoPin2    D4
#define buzzer      D6
#define THRESHOLD   50
