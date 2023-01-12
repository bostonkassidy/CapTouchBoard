#include <CapacitiveSensor.h>

/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */
#include <elapsedMillis.h>


#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

IPAddress ip(192, 168, 43, 177);
IPAddress pc(192, 168, 43, 43);

unsigned int localPort = 8888;      // local port to listen on

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

int sensorCount = 8;
int touchFlag[] = {0,0,0,0,0,0,0,0};
long total[] = {0,0,0,0,0,0,0,0};
int symbolNum[] = {49,50,51,52,53,54,55,56};

long threshhold = 2000;
int led = 13;
const int channel = 1;
unsigned int interval = 500; 


// Define the number of samples to keep track of. The higher the number, the
// more the readings will be smoothed, but the slower the output will respond to
// the input. Using a constant rather than a normal variable lets us use this
// value to determine the size of the readings array.
const int numReadings = 3;

int readings1[numReadings];      // the readings from the analog input
int readings2[numReadings];
int readings3[numReadings];
int readings4[numReadings];
int readings5[numReadings];
int readings6[numReadings];
int readings7[numReadings];
int readings8[numReadings];

int readIndex = 0;              // the index of the current reading
//int total1 = 0;                  // the running total
int average[numReadings];                // the average


#include "FastLED.h"
#define NUM_LEDS 8 
CRGB leds[NUM_LEDS];
#define PIN 10

#define sensitivityKnob A8

CapacitiveSensor   sensor1 = CapacitiveSensor(2,3);        // 1M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
CapacitiveSensor   sensor2 = CapacitiveSensor(4,5);        // 1M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire and or foil
CapacitiveSensor   sensor3 = CapacitiveSensor(6,7);        // 1M resistor between pins 4 & 8, pin 8 is sensor pin, add a wire and or foil
CapacitiveSensor   sensor4 = CapacitiveSensor(8,9);
CapacitiveSensor   sensor5 = CapacitiveSensor(14,15);
CapacitiveSensor   sensor6 = CapacitiveSensor(16,17);
CapacitiveSensor   sensor7 = CapacitiveSensor(18,19);
CapacitiveSensor   sensor8 = CapacitiveSensor(20,21);

void setup()                    
{
   Serial.begin(9600);
   pinMode(led, OUTPUT);
   
   Ethernet.begin(mac, ip);
   
   /*
  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }
*/
   
  // start UDP
  Udp.begin(localPort);   

   ////initialize
   for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings1[thisReading] = 0;
    readings2[thisReading] = 0;
    readings3[thisReading] = 0;
    readings4[thisReading] = 0;
    readings5[thisReading] = 0;
    readings6[thisReading] = 0;
    readings7[thisReading] = 0;
    readings8[thisReading] = 0;
  }
}
      
void loop()                    
{
    /// threshhold knob
    long sensitivity = analogRead(sensitivityKnob);
    //threshhold = map(sensitivity, 4, 1023, 999, 3000); ///enable knob

    readings1[readIndex] = sensor1.capacitiveSensor(30);
    total[0] = total[0] + readings1[readIndex];
    
    readings2[readIndex] = sensor2.capacitiveSensor(30);
    total[1] = total[1] + readings2[readIndex];

    readings3[readIndex] = sensor3.capacitiveSensor(30);
    total[2] = total[2] + readings3[readIndex];
    
    readings4[readIndex] = sensor4.capacitiveSensor(30);
    total[3] = total[3] + readings4[readIndex];
/*
    readings5[readIndex] = sensor5.capacitiveSensor(30);
    total[4] = total[4] + readings5[readIndex];

    readings6[readIndex] = sensor6.capacitiveSensor(30);
    total[5] = total[5] + readings6[readIndex];

    readings7[readIndex] = sensor7.capacitiveSensor(30);
    total[6] = total[6] + readings7[readIndex];

    readings8[readIndex] = sensor8.capacitiveSensor(30);
    total[7] = total[7] + readings8[readIndex];
  */  
    readIndex = readIndex + 1;

    // if we're at the end of the array...
    if (readIndex >= numReadings) {
      // ...wrap around to the beginning:
      readIndex = 0;
    }

    // calculate the average:
    for(int i; i<8; i++){
    average[i] = total[i] / numReadings;
    }

//// Uncomment for setup

for(int i = 0; i <= sensorCount-1; i++){
  Serial.print(average[i]);
  Serial.print("\t");
  if(i == sensorCount-1){
     Serial.print("threshhold: " + String(threshhold));
     Serial.println("\t");
     delay(100);
    }
  }
 
  readSensor(1, 60);
  readSensor(2, 61);
  readSensor(3, 62);
  readSensor(4, 63);
  //readSensor(5, 64);
  //readSensor(6, 65);
  //readSensor(7, 66);
  //readSensor(8, 67);
  delay(50);
    
  while (usbMIDI.read()) {
    // ignore incoming messages
  }
}

void readSensor(int sensorNumber, int midiNote){
   if (average[sensorNumber-1] > threshhold && touchFlag[sensorNumber-1] == 0){
      
      Serial.print("touch" + String(sensorNumber) + ": ");
      Serial.println(average[sensorNumber-1]);
     
      
      for(int i; i<8; i++){
         touchFlag[i] = 0;
        }
      //touchFlag[sensorNumber-1] = 1; //// Toggle momentary button
      
      usbMIDI.sendNoteOn(midiNote, 127, channel);  // 60 = C4
      
      Udp.beginPacket(pc, localPort);
      Udp.write(symbolNum[sensorNumber-1]);
      Udp.endPacket();
   
      elapsedMillis timeElapsed; //// lights on for 1 second
      while(timeElapsed < interval){
      //setPixel(sensorNumber ,0, 0, 255);
      digitalWrite(led, HIGH);
      }      
      
      usbMIDI.sendNoteOff(midiNote, 0, channel);  // 60 = C4
      
      digitalWrite(led, LOW);
      //setAll(0,0,0);
    }
  }
