#include <ArduinoBLE.h>
BLEService ledService("180A"); // BLE LED Service
BLEByteCharacteristic switchCharacteristic("2A57", BLERead | BLE
void setup() {
Serial.begin(9600);
// while (!Serial);
pinMode(LED_BUILTIN, OUTPUT); // Set the built-in LED pin to o
pinMode(2, OUTPUT); // Set the built-in LED pin to output mode

2024GEARS_Soft_Robotics Manual 12

if (!BLE.begin()) {
Serial.println("Starting Bluetooth® Low Energy failed!");
while (1);
}
BLE.setLocalName("Nano 33 IoT");
BLE.setAdvertisedService(ledService);
ledService.addCharacteristic(switchCharacteristic);
BLE.addService(ledService);
switchCharacteristic.writeValue(0);
BLE.advertise(); // Start advertising
Serial.println("BLE LED Peripheral mode");
}
void loop() {
BLEDevice central = BLE.central(); // Listen for BLE central d
if (central) {
Serial.print("Connected to central device: ");
Serial.println(central.address());
while (central.connected()) {
if (switchCharacteristic.written()) {
switch (switchCharacteristic.value()) {
case 0x01:
controlLED(HIGH, 0); // LED on
break;
default:
controlLED(LOW, 0); // LED off
break;
}
}
}
Serial.print("Disconnected from central device: ");
Serial.println(central.address());

2024GEARS_Soft_Robotics Manual 13

}
}
void controlLED(int state, unsigned long blinkInterval) {
static unsigned long previousMillis = 0;
unsigned long currentMillis = millis();
if (state == LOW) {
digitalWrite(LED_BUILTIN, LOW);
digitalWrite(2, LOW);
} else if (blinkInterval > 0) {
if (currentMillis - previousMillis >= blinkInterval) {
previousMillis = currentMillis;
if (digitalRead(LED_BUILTIN) == LOW) {
digitalWrite(LED_BUILTIN, HIGH);
digitalWrite(2, HIGH);
} else {
digitalWrite(LED_BUILTIN, LOW);
digitalWrite(2, LOW);
}
}
} else {
digitalWrite(LED_BUILTIN, state);
digitalWrite(2, state);
}
}




//--------------------------------------------------------------
// TinyCircuits ST BLE TinyShield UART Example Sketch
// Last Updated 2 March 2016
//
// This demo sets up the BlueNRG-MS chipset of the ST BLE modul
// with Nordic's virtual UART connection, and can pass data bet
// serial monitor and Nordic nRF UART V2.0 app or another compa
// terminal. This example is written specifically to be fairly
// with the Nordic NRF8001 example, with a replacement UART.ino
// 'aci_loop' and 'BLEsetup' functions to allow easy replacemen
//
// Written by Ben Rose, TinyCircuits http://tinycircuits.com
//
//--------------------------------------------------------------
#include <SPI.h>
#include <STBLE.h>
//Debug output adds extra flash and memory requirements!

#ifndef BLE_DEBUG
#define BLE_DEBUG true
#endif

#if defined (ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif

uint8_t ble_rx_buffer[21];
uint8_t ble_rx_buffer_len = 0;
uint8_t ble_connection_state = false;
#define PIPE_UART_OVER_BTLE_UART_TX_TX 0
const int ledPin = LED_BUILTIN; // Pin connected to the onboard

void setup() {
  SerialMonitorInterface.begin(9600);
  while (!SerialMonitorInterface); //This line will block until
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  BLEsetup();
}
void loop() {
aci_loop();//Process any ACI commands or events from the NRF80
//SerialMonitorInterface.print(ble_rx_buffer_len);
if (ble_rx_buffer_len) {//Check if data is available
SerialMonitorInterface.print(ble_rx_buffer_len);
SerialMonitorInterface.print(" : ");



SerialMonitorInterface.println((char*)ble_rx_buffer);
if (strcmp((char*)ble_rx_buffer, "on") == 0) {
digitalWrite(ledPin, HIGH);
SerialMonitorInterface.print(ble_rx_buffer_len);
}
if (strcmp((char*)ble_rx_buffer, "off") == 0) {
digitalWrite(ledPin, LOW);
SerialMonitorInterface.print(ble_rx_buffer_len);
}
ble_rx_buffer_len = 0;//clear afer reading
}

if (SerialMonitorInterface.available()) {//Check if serial inp
delay(10);//should catch input
uint8_t sendBuffer[21];
uint8_t sendLength = 0;
while (SerialMonitorInterface.available() && sendLength < 19
sendBuffer[sendLength] = SerialMonitorInterface.read();
sendLength++;
}
if (SerialMonitorInterface.available()) {
SerialMonitorInterface.print(F("Input truncated, dropped:
if (SerialMonitorInterface.available()) {
SerialMonitorInterface.write(SerialMonitorInterface.read
}
}
sendBuffer[sendLength] = '\0'; //Terminate string
sendLength++;
if (!lib_aci_send_data(PIPE_UART_OVER_BTLE_UART_TX_TX, (uint
{
SerialMonitorInterface.println(F("TX dropped!"));

}
}
}



}