// Mylena Diniz - 04/2021
// Lucas Fernandes
// (RX) LoRa SX1278 communication @ 433 MHz 

#include <SPI.h>   // module communication
#include <LoRa.h>

bool header = true;

// creating header
String dataLabel1 = "Pacote";
String dataLabel2 = "Umid_solo";
String dataLabel3 = "Umid_ar";
String dataLabel4 = "Temp_ar";
String dataLabel5 = "Rssi";
String dataLabel6 = "Snr";
String dataLabel7 = "Data";
String dataLabel8 = "Hora";

const int csPin = 10;   // Chip Select (Slave Select do protocolo SPI) do modulo Lora
const int resetPin = 0;   // LoRa module reset
const int irqPin = 4;   // DI0 pin

void setup(){
  Serial.begin(9600);
  while (!Serial);
  LoRa.setPins(csPin, resetPin, irqPin);

  // checking LoRa radio (it can be 433MHz or 915MHz)
  if (!LoRa.begin(433E6)){
    Serial.println("Erro ao iniciar modulo LoRa.");
    while (1);
  }
}

void loop(){
  while(header){
        Serial.print(dataLabel1);
        Serial.print("/");
        Serial.print(dataLabel2);
        Serial.print("/");
        Serial.print(dataLabel3);
        Serial.print("/");
        Serial.print(dataLabel4);
        Serial.print("/");
        Serial.print(dataLabel5);
        Serial.print("/");
        Serial.print(dataLabel6);
        Serial.print("/");
        Serial.print(dataLabel7);
        Serial.print("/");
        Serial.println(dataLabel8);
        header = false;
  }
  
  if (LoRa.parsePacket()){   // parse for a packet
    while (LoRa.available()){   // read packet
      Serial.print((char)LoRa.read());
    }
    Serial.print("/");
    Serial.print(String(LoRa.packetRssi())+"/");   // adds RSSI to data
    Serial.println(String(LoRa.packetSnr()));   // adds SNR to data
  }
}
