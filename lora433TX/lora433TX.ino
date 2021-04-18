// Mylena Diniz - 04/2021
// (TX) LoRa SX1278 communication @ 433 MHz 
// based on https://www.filipeflop.com/ code

#include "DHT.h"   // sensor library
#include <SPI.h>   // module communication            
#include <LoRa.h>
#define soil_humid_pin A0   // soil humidity
#define dht_pin A1   // air humidity/temperature
#define dht_type DHT11   // DHT 11

DHT dht(dht_pin, dht_type);

int soil_humid;
int soil_humid_pct;

const int csPin = 10;   // Chip Select (Slave Select do protocolo SPI) of LoRa module
const int resetPin = 0;   // LoRa module reset
const int irqPin = 4;   // DI0 pin
 
String outgoing;   // outgoing message
 
byte localAddress = 0xFF;   // THIS LoRa module address
byte msgCount = 0;   // sent message counter
byte destination = 0xBB;   // Receiver module address (0xFF sends to all)
long lastSendTime = 0;   // last sent messagem TimeStamp
int interval = 4000;   // gap between sent messages (5s or more) // I put 4
 
void setup(){
  Serial.begin(9600);
  while (!Serial);
  LoRa.setPins(csPin, resetPin, irqPin);
 
  // checking LoRa radio (it can be 433MHz or 915MHz)
  if (!LoRa.begin(433E6)){             
    Serial.println("Erro ao iniciar modulo LoRa.");
    while (true);                      
  }
  Serial.println("Modulo LoRa iniciado");

  // starting DHT sensor
  dht.begin(); 
  pinMode(soil_humid_pin, INPUT);
}
 
void loop(){  
  String soil;
  soil_humid = analogRead(soil_humid_pin); // reads A0 for soil humidity
  soil_humid_pct = (soil_humid*100)/1024;
  //Serial.print("Umidade do solo em %: ");
  //Serial.print(soil_humid_pct);
 
  
  if (soil_humid > 0 && soil_humid < 400){  // humid soil
    soil = "Solo umido"; 
    Serial.println("Status: Solo umido");
  }
  else if (soil_humid > 400 && soil_humid < 800)  { // partly humid soil
    soil = "Solo moderadamente umido"; 
    Serial.println("Status: Solo moderadamente umido");
  }
  else if (soil_humid > 800 && soil_humid < 1024)  { // dry soil
    soil = "Solo seco";   
    Serial.println(" Status: Solo seco");
  }
  else {
    soil = "Erro de leitura (solo)";
  }
  
  // DHT info reading can take 250ms to 2s
  float air_humid = dht.readHumidity();
  float air_temp = dht.readTemperature();
  
  if (isnan(air_temp) || isnan(air_humid)){
    Serial.println("Erro de leitura (DHT)");
  } 
  else{
    Serial.print(" Umidade: ");
    Serial.print(air_humid);
    Serial.print(" % \n");
    Serial.print(" Temperatura: ");
    Serial.print(air_temp);
    Serial.println(" *C");
  }
  delay(200);
  
  String msg_air_humid = String(air_humid);
  String msg_air_temp = String(air_temp);
  //String msg_soil_humid_pct = String(soil_humid_pct);

  // message to be sent by LoRa
  String message = "Status: " + soil + "\nUmidade: " + msg_air_humid + " % \nTemperatura: " + msg_air_temp + " *C";
  
  // checks time gap to send message
  if (millis() - lastSendTime > interval){ 
    sendMsg(message);
    Serial.println("Enviando: " + message );
    lastSendTime = millis();   // last message Timestamp
  } 
}

// send message function
void sendMsg(String outgoing){
  LoRa.beginPacket();                  
  LoRa.write(destination);   // adds destination address to packet
  LoRa.write(localAddress);   // adds sender address to packet
  LoRa.write(msgCount);                 
  LoRa.write(outgoing.length());   // adds message length (bytes) to packet 
  LoRa.print(outgoing);   // adds message to packet
  LoRa.endPacket();                     
  msgCount++;   // increments counter
}
