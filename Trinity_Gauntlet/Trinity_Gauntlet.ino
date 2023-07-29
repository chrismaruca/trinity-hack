#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Trinity_HackPublisher.h"
#include "secrets.h"

// Ultrasonic sensor
#define ECHO_PIN 27
#define TRIG_PIN 12

int distance; // micrometers

// OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// 'Trinity_logo', 32x32px
const unsigned char Trinity_logo [] PROGMEM = {
  0x00, 0x03, 0xc0, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x7f, 0xff, 0x00, 
  0x00, 0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0x80, 
  0x01, 0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0x80, 
  0x01, 0xff, 0xff, 0x80, 0x03, 0xff, 0xff, 0x80, 0x07, 0xff, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xf8, 
  0x7f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x7f, 0xff, 0x7f, 0xfe, 0x1f, 0xfc, 0x1f, 0xf8, 0x07, 0xf0, 0x0f, 0xe0, 0x03, 0xc0, 0x03, 0x80
};


// Wifi
const char* ssid = SECRET_SSID;
const char* password = SECRET_PW;

HackPublisher publisher("trinity");


// Gas sensor
int gaslvl = 0;


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

// Displays relevant data onto the OLED screen
void displayData(double dist, double temp, double hum, int gas) {
  display.clearDisplay();
  // Temperature & Humidity
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("T: " + String(temp, 1) + "C, H: " + String(hum, 1));
  display.drawLine(0, 15, 128, 15, WHITE);

  // Distance
  display.setTextSize(2);
  display.setCursor(32, 16);
  display.println("Distance:");
  display.setCursor(32, 32);
  display.println(" " + String(dist, 2) + " cm");
  display.setCursor(32, 48);
  display.println(" " + String(dist / 2.54, 2) + " in");
  
  // Display Trinity logo
  display.drawBitmap(0, 32, Trinity_logo, 32, 32, WHITE);
  display.display(); 
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) continue;
  Serial.println("test");
  Serial.println(ssid);
    
  // Set up OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(1000);
  display.setTextColor(WHITE);
    

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
  publisher.store("gas", gaslvl);
  gaslvl++;

  displayData(distance / 100.0, gaslvl, gaslvl, gaslvl); 
  
  if (gaslvl > 1000) {
    gaslvl = 0;
  }
  
  publisher.send();

  delay(250);
}
