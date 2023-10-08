/*
 * Project argon-lora-test
 * Description:
 * Author:
 * Date:
 */

#include <TinyLoRa.h>

#define LoRa_IRQ D6   // pin connected to LoRa's interrupt pin
#define LoRa_NSS D1   // pin connected to LoRa's slave select pin
#define LoRa_RST D2   // pin connected to LoRa's reset pin

#define ONBOARD_LED D7

// TO DO: 
//    Read on LoRa Protocol, watch Youtube
//    Read on Network Session Key, Application Session Key and Device Address
//    Include AnalogRead() to sensors, and write that as loraData
//    Change Pinout for TinyLoRa or Particle Argon
//    Hook to MQTT (?) --> Read about LoRa to MQTT
//    Make 3 variations:
//        Receivers : Receive LoRa, push to MQTT broker
//        Repeaters : Receive Lora, push LoRa (other channel)
//        Sensors   : Receive Sensors, push LoRa
//        1 code applies to all 3? --> Security issues (?) & device storage (?)

// Network Session Key (MSB)
uint8_t NwkSkey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// Application Session Key (MSB)
uint8_t AppSkey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// Device Address (MSB)
uint8_t DevAddr[4] = { 0x00, 0x00, 0x00, 0x00 };

// Data Packet to Send to TTN
unsigned char loraData[11] = {"hello LoRa"};

// How many times data transfer should occur, in seconds
const unsigned int sendInterval = 30;

// Pinout for Adafruit Feather 32u4 LoRa
TinyLoRa lora = TinyLoRa(7, 8, 4);


void setup()
{
  delay(2000);
  Serial.begin(9600);
  while (!Serial);
  
  // Initialize pin LED_BUILTIN as an output
  pinMode(ONBOARD_LED, OUTPUT);
  
  // Initialize LoRa
  Serial.print("Starting LoRa...");
  // define multi-channel sending
  lora.setChannel(MULTI);
  // set datarate
  lora.setDatarate(SF7BW125);
  if(!lora.begin())
  {
    Serial.println("Failed");
    Serial.println("Check your radio");
    while(true);
  }

  // Optional set transmit power. If not set default is +17 dBm.
  // Valid options are: -80, 1 to 17, 20 (dBm).
  // For safe operation in 20dBm: your antenna must be 3:1 VWSR or better
  // and respect the 1% duty cycle.

  // lora.setPower(17);

  Serial.println("OK");
}

void loop()
{
  Serial.println("Sending LoRa Data...");
  lora.sendData(loraData, sizeof(loraData), lora.frameCounter);
  // Optionally set the Frame Port (1 to 255)
  // uint8_t framePort = 1;
  // lora.sendData(loraData, sizeof(loraData), lora.frameCounter, framePort);
  Serial.print("Frame Counter: ");
  Serial.println(lora.frameCounter);
  lora.frameCounter++;

  // blink LED to indicate packet sent
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  
  Serial.println("delaying...");
  delay(sendInterval * 1000);
}
