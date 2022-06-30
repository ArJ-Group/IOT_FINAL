#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include <SimpleTimer.h>
#include <SimpleDHT.h>

// LiquidCrystal_I2C lcd(0x27, 16, 2);

// Soil Moisture 
const int sensor_pin = A0; 
float moisture_percentage;
const int AirValue = 616;   
const int WaterValue = 335;
int soilMoistureValue = 0;
int soilmoisturepercent=0;
SimpleTimer timer;

// Connection 
WiFiClient espClient;
PubSubClient client(espClient);
String macAddr = "";

//LED
#define BLUE_LED D3
#define GREEN_LED D2 
#define RED_LED D1 

// DHT
SimpleDHT11 dht11(D7);

// MQTT
const char *ssid = "OPPO A9 2020";
const char *password = "punyaaika";
const char *mqtt_server = "broker.hivemq.com"; 

void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
  macAddr = WiFi.macAddress();
  Serial.println(macAddr);
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(macAddr.c_str()))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void MainFunction() { 
  soilMoistureValue = analogRead(A0);
  Serial.println(soilMoistureValue); 
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100); 
  Serial.println(soilmoisturepercent); 
  static char soilMoisture[7];
  dtostrf(soilmoisturepercent, 4, 2, soilMoisture);
  Serial.print("Soil Moisture Percent: ");
  Serial.println(soilMoisture);
  
  client.publish("1941720088/room/soil", soilMoisture); 
  if(soilmoisturepercent < 10) 
  { 
  Serial.println("Nearly dry, Pump turning on"); 
  digitalWrite(RED_LED, LOW); 
  digitalWrite(GREEN_LED, LOW); 
  digitalWrite(BLUE_LED, HIGH);
  digitalWrite(D5, LOW); 
  } 
  else if(soilmoisturepercent > 70) 
  { 
  Serial.println("Nearly wet, Pump turning off"); 
  digitalWrite(RED_LED, LOW); 
  digitalWrite(GREEN_LED, LOW); 
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(D5, HIGH); 
  } else {
    Serial.println("Still Need Water"); 
    digitalWrite(RED_LED, LOW); 
    digitalWrite(GREEN_LED, LOW); 
    digitalWrite(BLUE_LED, HIGH);
    digitalWrite(D5, LOW); 
  }

  int err = SimpleDHTErrSuccess;

  byte temperature = 0;
  byte humidity = 0;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess)
  {
    Serial.print("Pembacaan DHT11 gagal, err=");
    Serial.println(err);
    delay(1000);
    return;
  }

  static char temperatureTemp[7];
  dtostrf(temperature, 4, 2, temperatureTemp);
  Serial.print("Temp: ");
  Serial.println(temperatureTemp);

  static char humidityHum[7];
  dtostrf(humidity, 4, 2, humidityHum);
  Serial.print("Humidity: ");
  Serial.println(humidityHum);

  
  client.publish("1941720088/room/temperature", temperatureTemp); 
  client.publish("1941720088/room/humidity", humidityHum);
  
}

void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageInfo;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageInfo += (char)message[i];
  }
  Serial.println();

  if(topic=="1941720088/room/pump"){
      Serial.print("Water pump turning ");
      if(messageInfo == "on"){
        digitalWrite(D5, HIGH);
        Serial.print("On");
      }
      else if(messageInfo == "off"){
        digitalWrite(D5, LOW);
        Serial.print("Off");
      }
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  
  pinMode(D5,OUTPUT);
  pinMode(RED_LED,OUTPUT);
  pinMode(GREEN_LED,OUTPUT);
  pinMode(BLUE_LED,OUTPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.subscribe("1941720088/room/pump");
  timer.setInterval(1000L,MainFunction);
}
void loop() {
  if (!client.connected())
  {
    reconnect();
  }
  if (!client.loop())
  {
    client.connect(macAddr.c_str());
  }

  timer.run();

}
