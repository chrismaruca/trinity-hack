#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "AM232X.h"
#include "Trinity_HackPublisher.h"
#include "secrets.h"
//#include <Adafruit_NeoPixel.h>

// Ultrasonic sensor
#define ECHO_PIN 27
#define TRIG_PIN 12
#define SOUND_SPEED 0.0343
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
HackPublisher publisher("trinity", true);


// Gas sensor
#define GAS_PIN A2
#define BUZZER_PIN 13
const double DANGER_THRESHOLD = 75.0;
const double GAS_THRESHOLD = 50.0;
double gas = 0;
int buzz = 0;


// Temperature sensor
AM232X AM2320;
double temperature = 0;
double humidity = 0;

// NeoPixels
#define PIN            15
#define NUMPIXELS      12
//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//Misc
int testval = 0;

// Initialize the Buzzer
void initBuzzer() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

// Play the buzzer sound in 500 ms intervals when gas is detected
void playBuzzer(int g) {
  if (buzz > 0) {
    buzz--;
  }
  
  if (buzz == 0 && g >= GAS_THRESHOLD)
  {
    digitalWrite(BUZZER_PIN, HIGH);
    buzz = 4;
  }
  else if (buzz <= 2) {
    digitalWrite(BUZZER_PIN, LOW);
  }
}

// Returns distance from ultrasonic sensor in micrometers
int readDistance() {
  long duration;
  // Clears the trig pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  // Sets the trig pin to HIGH for 10 ms
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Reads the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculates and returns the distance in micrometers
  return duration * SOUND_SPEED * 100 / 2;
}

// Returns analog gas reading from sensor (12 bit, 0-4095)
int readGas() {
  int g = analogRead(GAS_PIN);
  Serial.println(g);
  
  return g;
}

// Displays relevant data onto the OLED screen
void displayData(double dist, double temp, double hum, double gas) {
  display.clearDisplay();
    
  // Temperature & Humidity
  display.setTextSize(1);
  display.setCursor(0, 4);
  //display.println("T: " + String(temp, 1) + " C,"); // Celsius
  display.println("T: " + String(temp * 9 / 5 + 32, 1) + " F,"); // Fahrenheit
  display.setCursor(72, 4);
  display.println("H: " + String(hum, 1) + "%");
  display.drawLine(0, 15, 128, 15, WHITE);

  // Distance
  display.setCursor(33, 16);
  display.println("Distance:");
  
  display.setTextSize(2);
  display.setCursor(33, 24);
  display.println(String(dist, 0) + " cm");
  display.setCursor(33, 39);
  display.println(String(dist / 2.54, 0) + " in");
  
  // Gas
  String rating; // Store gas safety rating
  if (gas >= DANGER_THRESHOLD) {
    rating = "Danger!";
  } else if (gas >= GAS_THRESHOLD) {
    rating = "Unsafe";
  } else {
    rating = "Safe";
  }
  display.setTextSize(1);
  display.drawLine(0, 54, 128, 54, WHITE);
  display.setCursor(0, 56);
  display.println("Gas: " + String(gas, 1) + "%,");
  display.setCursor(72, 56);
  display.println(rating);
  
  // Display Trinity logo
  display.drawBitmap(0, 20, Trinity_logo, 32, 32, WHITE);
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

  // Set up AM2320
  if (!AM2320.begin()) {
    Serial.println("Sensor not found");
    while (1);
  }
  AM2320.wakeUp();
  delay(1000);
    
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

  // NeoPixel setup
  //pixels.begin(); // This initializes the NeoPixel library.

  // Initialize the Buzzer
  initBuzzer();
}

void loop() {
  distance = readDistance(); // Read ultrasonic sensor data into distance (micrometers)
  
  AM2320.read(); // Read AM2320 data
  temperature = int(AM2320.getTemperature() * 10) / 10.0; // Get temperature and round to one decimal
  humidity = int(AM2320.getHumidity() * 10) / 10.0; // Get humidity and round to one decimal
  
  gas = map(readGas(), 0, 4095, 0, 1000) / 10.0; // Get the gas level and scale it from 0 - 100.0
  playBuzzer(gas);
  
  //Serial.println(distance / 100.0); // Print in cm
  publisher.store("dist", distance / 100.0); // Send over wifi in cm
  publisher.store("gas", gas);
  publisher.store("temp", temperature);
  publisher.store("hum", humidity);
  publisher.send();
  
  // Display readings on OLED screen
  displayData(distance / 100.0, temperature, humidity, gas); 
  
  // Dummy data
  testval += 5;
  if (testval > 1000) {
    testval = 0;
  }
  
  delay(250);
}
