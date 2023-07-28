#include "Trinity_HackPublisher.h"
#include "secrets.h"

const char* ssid = SECRET_SSID;
const char* password = SECRET_PW;

HackPublisher publisher("trinity");

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
}

void loop() {
  for (int i = 0; i < 100; i++)
  {
    publisher.store("pos", i);
    publisher.store("gas", i*10);
    publisher.send();

    delay(50);
  }
}
