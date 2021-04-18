// Mylena Diniz - 04/2021
// (RX and TX) LoRa SX1278 communication @ 433 MHz 
// based on https://www.filipeflop.com/ code 

#include <SPI.h>  // module communication            
#include <LoRa.h>

const int csPin = 10;   // Chip Select (Slave Select do protocolo SPI) do modulo Lora
const int resetPin = 0;   // LoRa module reset
const int irqPin = 4;   // DI0 pin
 
String outgoing;   // outgoing message
 
byte localAddress = 0xBB;     // THIS LoRa module address
byte msgCount = 0;   // sent message counter
byte destination = 0xFF;   // Receiver module address (0xFF sends to all)
long lastSendTime = 0;   // last sent messagem TimeStamp
int interval = 5000;   // gap between sent messages (5s or more)

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
}

void loop(){
  // checks time gap to send message
  if (millis() - lastSendTime > interval){
    sendMsg(message);
    Serial.println("Enviando: " + message);
    lastSendTime = millis();   // last message Timestamp
  }
 
  // parse for a packet, and call onReceive
  receiveMsg(LoRa.parsePacket());
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
 
// receive message function
void receiveMsg(int packet){
  if (packet == 0){
    return;   // no message, no return
  }
 
  int receiver = LoRa.read();   // receiver address
  byte sender = LoRa.read();   // sender address
  byte incomingMsgId = LoRa.read();   // incoming message ID (counter)
  byte incomingLength = LoRa.read();   // message length
 
  String incoming;
 
  while (LoRa.available()){
    incoming += (char)LoRa.read();
  }
 
  if (incomingLength != incoming.length()){   
    Serial.println("Erro de comunicação! ");   // lengths don't match
    return;   // "breaks" code                         
  }
  if (receiver != localAddress && receiver != 0xBB){   // checks destination ID
    Serial.println("Esta mensagem não é para este nó.");
    return;   // "breaks" code
  }
 
  Serial.println(incoming);
  Serial.println("Recebido do dispositivo: 0x" + String(sender, HEX));
  Serial.println("Enviado para: 0x" + String(receiver, HEX));
  Serial.println("ID da mensagem: " + String(incomingMsgId));
  Serial.println("Tamanho da mensagem: " + String(incomingLength));
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();
}
