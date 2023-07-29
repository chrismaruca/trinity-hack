#include "Trinity_HackPublisher.h"
#include "secrets.h"

#define ECHO_PIN 27
#define TRIG_PIN 12

int distance; // micrometers
int temp = 0;

const char* ssid = SECRET_SSID;
const char* password = SECRET_PW;

HackPublisher publisher("trinity");

// Returns distance from ultrasonic sensor in micrometers
int readDistance() {
  long duration;
  // Clears the trig pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  // Sets the trig pin to HIGH for 10 ms
  digitalWrite(TRIG_PIN, HIGH);
  delay(10);
  digitalWrite(TRIG_PIN, LOW);

  // Reads the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculates and returns the distance
  return duration * 3.43 / 2;
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) continue;
  Serial.println("test");
  Serial.println(ssid);

  // Connect to wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Initialize publisher
  publisher.begin();

  // Set pin modes for ultrasonic sensor
  pinMode(ECHO_PIN, INPUT); 
  pinMode(TRIG_PIN, OUTPUT);
}

void loop() {
  distance = readDistance(); // Read ultrasonic sensor data into distance
  Serial.println(distance / 100.0); // Print in cm
  publisher.store("pos", distance / 100.0); // Send over wifi in cm
  publisher.store("gas", temp);
  temp++;
  
  if (temp > 1000) {
    temp = 0;
  }
  
  publisher.send();

  delay(250);
}
