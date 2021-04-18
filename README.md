# LoRa
Codes for Communication between LoRa modules (Cognitive Sensor Networks project)

- The .ino codes get the data info from sensors (soil humidity; air humidity; air temperature) and send from TX to RX via LoRa transmittion:  
  - the 'compact' code sends a simpler message;
  - the other lora433 sends more detail info;
  - the code can be used for 915 MHz modules that use the same communitcation method (SPI);
  - lora_TXRX is generic for sending and receiving messages.
- The .py code gets the data received (RX) through serial port connected to arduino and saves it in a (medicoesLoRa).xlsx file.
- The .m code gets info from .xlsx file and plot graphs with sensor data vs hour of the day or packet ID.
