#define BLYNK_TEMPLATE_ID "TMPLKNWnfEtv"
#define BLYNK_DEVICE_NAME "Home Automation System"
#define BLYNK_AUTH_TOKEN "tBdS8ztkXPp9E-5fZXgRXLy-9VD-qwM_"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

/**************Includes************/
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"

/*************************Define the pins we are using************************/
#define LED            D4 
#define ldrPin         A0 
#define MAX_LDR        40
#define Light1         D5
#define Light2         D6
#define Fan            D7
#define Aux            D8
#define motionSensor   0

#define timeSeconds    5 //seconds

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME  "amroadel"
#define AIO_KEY       "e840a0afff0d4440b2fd404020bdefe4"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe L1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Light1"); // FeedName
void MQTT_connect();
/********************************************************************************/

char auth[] = BLYNK_AUTH_TOKEN;

BlynkTimer timer;
Adafruit_AM2320 am2320 = Adafruit_AM2320();

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Amro's Wifi";
char pass[] = "overkill1234";

/*****************Timer: Auxiliary variables (USED FOR PIR)***********************/
unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;

int pinValue1;
int pinValue2;
int pinValue3;
bool enable_sensors = true;
//Changing Light1 with the slider
BLYNK_WRITE(V3)
{
  pinValue1 = param.asInt(); // assigning incoming value from pin V3 to a variable
  digitalWrite(Light1, !pinValue1);
}

//Changing Light2 with the slider
BLYNK_WRITE(V6)
{
  pinValue2 = param.asInt(); // assigning incoming value from pin V6 to a variable
  if (!enable_sensors)
    digitalWrite(Light2, !pinValue2);
}

//Changing Fan with the slider
BLYNK_WRITE(V4)
{
  pinValue3 = param.asInt(); // assigning incoming value from pin V6 to a variable
  if (!enable_sensors) {
    digitalWrite(Fan, !pinValue3);
    Blynk.virtualWrite(V8, pinValue3);
  }
}

BLYNK_WRITE(V7)
{
  enable_sensors = param.asInt(); // assigning incoming value from pin V6 to a variable

}

// Checks if motion was detected, sets LED HIGH and starts a timer (ISR of the PIR sensor)
void IRAM_ATTR detectsMovement() {
  Blynk.virtualWrite(V5, HIGH);
  digitalWrite(LED, HIGH);
  startTimer = true;
  lastTrigger = millis();
}

// Interact with the blynk interface
void sendSensor()
{
  float h = am2320.readHumidity();
  float t = am2320.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  float ldrStatus = analogRead(ldrPin);

  //Handle Temp and Hmidity sensros
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }


  if (enable_sensors){
    if (t > 24.5) {
      Blynk.virtualWrite(V8, HIGH);
      digitalWrite(Fan, LOW);
    }
    else {
      Blynk.virtualWrite(V8, LOW);
      digitalWrite(Fan, HIGH);
    }
  }

 
  
  
  Blynk.virtualWrite(V1, h); //Write Humidity to the app
  Blynk.virtualWrite(V0, t); //Write Temp to the app

  //Handle LDR sensor
  float percentage = (ldrStatus/MAX_LDR)*100;
  Blynk.virtualWrite(V2, percentage); // To turn LightOFF in blynk

  if (enable_sensors) { 
    if (percentage > 20) {
    digitalWrite(Light2, HIGH);
    }
    else {
      digitalWrite(Light2, LOW);
    }
  }

  

  // PIR Sensor Handling 
  // Current time
  now = millis();
  // Turn off the LED after the number of seconds defined in the timeSeconds variable
  if(startTimer && (now - lastTrigger > (timeSeconds*1000))) {
    Blynk.virtualWrite(V5, LOW);
    digitalWrite(LED, LOW);
    startTimer = false;
  }


  // Google Assistant
  
  Adafruit_MQTT_Subscribe *subscription;
  subscription = mqtt.readSubscription(100);
  if (subscription == &L1) {
    int Light1_State = atoi((char *)L1.lastread);
    digitalWrite(Light1, !Light1_State);
    Blynk.virtualWrite(V3, Light1_State); 
    
  }


}
void setup() {
  
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(ldrPin, INPUT); // Here LDR sensor is determined as input.
  pinMode(Light1, OUTPUT); // Here Light1 is determined as output.
  pinMode(Light2, OUTPUT); // Here Light2 is determined as output.
  pinMode(Fan, OUTPUT); // Here Fan is determined as output.
  pinMode(motionSensor, INPUT_PULLUP); // PIR Motion Sensor mode INPUT_PULLUP

  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

  //Connect to the Blynk framework
  Blynk.begin(auth, ssid, pass);
  am2320.begin();
  timer.setInterval(1000L, sendSensor);

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&L1);
}

void loop() {
  Blynk.run();
  timer.run();

  MQTT_connect();
  /*
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &L1) {
      int Light1_State = atoi((char *)L1.lastread);
      digitalWrite(Light1, !(Light1_State));
      Blynk.virtualWrite(V3, !(Light1_State));
      
    }
  }*/
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  
}
