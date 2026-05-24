# Smart Waste Management System using IoT

## Project Overview

This project is an IoT-based Smart Waste Management System designed to monitor trash bin status using sensors and automatically notify waste collectors when the bin becomes full.

The system combines:
- Weight detection using a Load Cell Sensor
- Distance measurement using an Ultrasonic Sensor
- Wireless communication using WiFi and MQTT protocol

When the trash bin exceeds predefined thresholds, the system automatically sends a notification to the MQTT broker indicating that the bin requires collection.

The project demonstrates practical applications of:
- Embedded Systems
- IoT Communication
- Sensor Integration
- Real-Time Monitoring
- MQTT Messaging

---

## Technologies & Components Used

### Hardware
- Arduino MKR WiFi 1010
- HX711 Load Cell Amplifier
- Load Cell Sensor
- Ultrasonic Sensor
- WiFi Module (Built into MKR1010)

### Software & Libraries
- Arduino IDE
- WiFi Library
- PubSubClient Library
- HX711 Library
- Ultrasonic Library

### Communication Protocol
- MQTT Protocol
- Mosquitto Public MQTT Broker

---

## System Features

- Measures trash bin weight using a load sensor
- Detects trash level using an ultrasonic sensor
- Sends automatic MQTT notifications when the bin becomes full
- Receives confirmation messages from waste collectors
- Displays real-time monitoring data through Serial Monitor

---

## How the System Works

1. The ultrasonic sensor measures the distance between the trash and the top of the bin.
2. The load cell measures the weight of the trash.
3. The Arduino continuously monitors both values.
4. If:
   - Distance becomes lower than a threshold, OR
   - Weight exceeds a threshold,
   
   the system considers the trash bin full.
5. A notification message is published to the MQTT broker.
6. The collector can respond through another MQTT topic.
7. The system receives and displays the response.

---

## MQTT Topics Used

### Publish Topic
```bash
trash/full
