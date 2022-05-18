// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLdKrHeCfC"
#define BLYNK_DEVICE_NAME "AM2320"
#define BLYNK_AUTH_TOKEN "u3YwEPiWZV0F2ViyxENYZVXvNPmla36r"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
char auth[] = BLYNK_AUTH_TOKEN;

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Interfacing Arduino uno with LDR sensor
const int ledPin = D4; // digital pin 5 
const int ldrPin = A0; // analog pin 0
const int ANALOG_THRESHOLD = 600; // Threshold to compare the values of the light if it's dark or not
void setup() { // The setup() function will only run once, after each powerup or reset of the Arduino board.
Serial.begin(9600);
//pinMode(ledPin, OUTPUT); // Here LED is determined as an output or an indicator.
pinMode(ldrPin, INPUT); // Here LDR sensor is determined as input.
}
void sendSensor (){
  
  // if the read input is not a number print failure message
  if (isnan(ldrStatus)) {
    Serial.println("Failed to read from LDR sensor!");
    return;
  } 
  int ldrStatus = analogRead(ldrPin);
  // For RoomLight1
  if(ldrStatus < ANALOG_THRESHOLD)
    Blynk.virtualWrite(V3, LOW); // To turn LightOFF in blynk 
  else
    Blynk.virtualWrite(V3, HIGH); // To turn LightON in blynk 
  
  Serial.println(ldrStatus);
}
void loop() { // Void loop is ran again and again and contains main code.

  Blynk.run();
  timer.run();

//if (ldrStatus <= 200) {digitalWrite(ledPin, HIGH); // If LDR senses darkness led pin high that means led will glow.
//Serial.print("Darkness over here,turn on the LED :");
//Serial.println(ldrStatus);
//} else {
//digitalWrite(ledPin, LOW); // If LDR senses light led pin low that means led will stop glowing.
//Serial.print("There is sufficient light , turn off the LED : ");
//Serial.println(ldrStatus);
//}
}
