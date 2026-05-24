#include <WiFi.h>              // Include the WiFi library
#include <PubSubClient.h>      // Include the MQTT library
#include "HX711.h"
#include "Ultrasonic.h"

// WiFi credentials
const char* ssid = "Bulk";            // Replace with your WiFi SSID
const char* password = "Bulking7";    // Replace with your WiFi password

// MQTT broker
const char* mqtt_server = "test.mosquitto.org"; // Use your broker's address
const char* topic_send = "trash/full";         // Topic to notify the trash is full
const char* topic_receive = "trash/collector"; // Topic to receive collector's response

WiFiClient espClient;
PubSubClient client(espClient);

// Define the pins for the HX711
const int LOADCELL_DOUT_PIN = 7;
const int LOADCELL_SCK_PIN = 6;

HX711 scale;

// Define the ultrasonic sensor
Ultrasonic ultrasonic(3);

void setup_wifi() {
    delay(10);
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected.");
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.println(topic);

    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println("Message: " + message);

    if (String(topic) == topic_receive && message == "coming") {
        Serial.println("Trash collector is on the way!");
    }
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("TrashBinClient")) {
            Serial.println("Connected to MQTT broker.");
            client.subscribe(topic_receive);
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" Retrying in 5 seconds...");
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(9600);

    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);

    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.set_scale(350000.f);
    scale.tare();
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    delay(5000);

    long RangeInCentimeters = ultrasonic.MeasureInCentimeters();
    float weight = scale.get_units(10);

    Serial.print("Measured Distance: ");
    Serial.print(RangeInCentimeters);
    Serial.println(" cm");

    Serial.print("Measured Weight: ");
    Serial.print(weight);
    Serial.println(" kg");

    if (RangeInCentimeters < 7 || weight > 1.0) {
        Serial.println("Trash bin is full. Sending notification...");
        client.publish(topic_send, "Trash bin is full!");
    } else {
        Serial.println("Trash bin is not full yet.");
    }
}