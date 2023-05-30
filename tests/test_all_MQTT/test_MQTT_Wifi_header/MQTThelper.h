//gleicher Code wie in der Header Datei der main
#ifndef MQTT_HELPER_H
#define MQTT_HELPER_H

#include <WiFi.h>
#include <PubSubClient.h>
#include "secure.h"

extern void callback(char* topic, byte* payload, unsigned int length); // Deklariere die Callback-Funktion

class MQTTHelper {
public:
  MQTTHelper() : client(espClient) {}
  void setup() {
    Serial.begin(115200);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // ab hier hat der Rest nichts mehr mit Wlan sondern nur noch mit MQTT zu tun

    // Set up MQTT server and callback function
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

    // Connect to MQTT broker
    reconnect();
  }

  void loop() {
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
    //String message = "vorwärts";
    //client.publish(mqttTopic, message.c_str());
    //client.callback("RoboTUM/steuerung")
    //Serial.println("Message published");
    //delay(5000);  // Wait 5 seconds before publishing the next message
  }

  static void callback(char* topic, byte* payload, unsigned int length) {
    // Handle incoming messages
    String message = "";
    for (int i = 0; i < length; i++) {
      message += (char)payload[i];
    }
    Serial.print("Received message: ");
    Serial.println(message);
  }

  void publishMessage(const char* topic, const char* message) {
    if (client.connected()) {
      client.publish(topic, message);
    }
  }

private:
  const char* mqttServer = "test.mosquitto.org";
  const int mqttPort = 1883;
  const char* mqttClientId = "ESP32Client";

  WiFiClient espClient;
  PubSubClient client;
  void reconnect() {
    while (!client.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (client.connect(mqttClientId)) {
      Serial.println("Connected to MQTT broker");
      client.subscribe("RoboTUM/steuerung");  // Subscribe to the topic (optional)
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
    }
  }
};

#endif
