# Home_Automation_System
Embedded Systems Project

# Project Idea
In this project, we will be building a home automation system based on the NodeMCU board. We will be able to control several devices and appliances using voice commands to google assistant as well as a mobile app and a web interface using Blynk. To do so, our NodeMCU will connect to the Adafruit.io cloud in order to interface with IFTTT, which will interact with Google assistant. Also, the NodeMCU will connect to a Blynk cloud where we can control the home system using a mobile app or a web interface. Our system will include several sensors that will report some insights about the surrounding environment that will be used to control some actuators. For example, we will be using a temperature sensor to measure the temperature inside the room, and based on that, the NodeMCU will turn on/off a fan.   
# Motivation 
*	Smart cities are being built all over Egypt as part of the mega projects pushed by the government in an effort to create a more modernized urban environment.
*	A building block for any of those cities would be a smart house that tries to harness technology to consume energy efficiently as well as measure certain data that helps to make the house more secure, sustainable and efficient.


# Hardware
* ESP8266 WIFI Module (Or NodeMCU)
* Power Supply (12V and 5V)
* Four Channels Relay Module
* Motion Sensor (PIR Sensor)
* Temperature and Humidity Sensor (AM2302 sensor)
* Fan 
* 2 Light Bulbs
# Software and services
* Arduino IDE
* Blynk
* IFTTT
* Google Assistant
* Adafruit.io  
![Project Architecture](https://github.com/amroadel/Home_Automation_System/blob/main/imgs//softwares.PNG)
# System Architecture & Proposed Solution
* The NodeMCU board will be used as a development board as well as a gateway to the internet to communicate with different services.
* Blynk will offer a platform to develop a mobile and web interface for our smart home. 
* Adafruit.io will connect the NodeMCU to the IFTTT cloud, which will be responsible for relying the Google Assistant commands back to the NodeMCU.

![Project Architecture](https://github.com/amroadel/Home_Automation_System/blob/main/imgs/Diagram.png)
# Circuit Diagram 
![Project Architecture](https://github.com/amroadel/Home_Automation_System/blob/main/imgs/cicuit_diagram.png)


