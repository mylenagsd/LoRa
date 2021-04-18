// Mylena Diniz - 04/2021
// Lucas Fernandes
// (TX) LoRa SX1278 communication @ 433 MHz 

#include <SPI.h>   // module communication
#include <LoRa.h>
#include "DHT.h"   // sensor library

#define soil_humid_pin A0   // soil humidity
#define dht_pin A1   // air humidity/temperature
#define dht_type DHT11    // DHT 11

DHT dht(dht_pin, dht_type);
int counter = 0;   // message counter

int soil_humid;
int soil_humid_pct;

const int csPin = 10;   // Chip Select (Slave Select do protocolo SPI) of LoRa module
const int resetPin = 0;   // LoRa module reset
const int irqPin = 4;   // DI0 pin

void setup(){
  Serial.begin(9600);
  while (!Serial);
  LoRa.setPins(csPin, resetPin, irqPin);

  // checking LoRa radio (it can be 433MHz or 915MHz)
  if (!LoRa.begin(433E6)) {
    Serial.println("Erro ao iniciar modulo LoRa.");
    while (1);
  }

  dht.begin();   // starting DHT sensor
  pinMode(soil_humid_pin, INPUT);
}

void loop(){
  soil_humid = analogRead(soil_humid_pin);
  soil_humid_pct = (soil_humid*100)/1024;

  // DHT info reading can take 250ms to 2s
  float air_humid = dht.readHumidity();
  float air_temp = dht.readTemperature();
  
  if (isnan(air_temp) || isnan(air_humid)){
    Serial.println("Erro de leitura (DHT)");
  } 
  
  Serial.print("Enviando pacote: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print(counter);
  LoRa.print("/");
  LoRa.print(soil_humid);
  LoRa.print("/");
  LoRa.print(air_humid);
  LoRa.print("/");
  LoRa.print(air_temp);
  LoRa.endPacket();

  counter++;

  delay(5000);
}
