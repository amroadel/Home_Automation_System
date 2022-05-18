/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLdKrHeCfC"
#define BLYNK_DEVICE_NAME "AM2320"
#define BLYNK_AUTH_TOKEN "u3YwEPiWZV0F2ViyxENYZVXvNPmla36r"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"

#define LED            D4

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Amro's Wifi";
char pass[] = "overkill1234";

BlynkTimer timer;
Adafruit_AM2320 am2320 = Adafruit_AM2320();

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = am2320.readHumidity();
  float t = am2320.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
//  Blynk.begin(auth, ssid, pass, "blynk.cloud", 8080);
//  Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  am2320.begin();
  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
