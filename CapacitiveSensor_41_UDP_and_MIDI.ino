/*
 * Uses a high value resistor e.g. 1M between send pin and receive pin
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */
#include <CapacitiveSensor.h>
#include <elapsedMillis.h>
#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

IPAddress ip(192, 168, 43, 177); /// teensy ip
IPAddress pc(192, 168, 43, 43); /// computer ip

unsigned int localPort = 8888;      // local port to listen on

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

const int sensorCount = 16;
int touchFlag[sensorCount];
long total[sensorCount];
int symbolNum[] = {49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64};

long threshhold = 2500;   ///// Set threshhold here!
int led = 13;
const int channel = 1;
unsigned int interval = 100; 

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
int readings9[numReadings];
int readings10[numReadings];
int readings11[numReadings];
int readings12[numReadings];
int readings13[numReadings];
int readings14[numReadings];
int readings15[numReadings];
int readings16[numReadings];

int readIndex = 0;              // the index of the current reading
//int total1 = 0;                  // the running total
int average[numReadings];                // the average

#define sensitivityKnob A17

CapacitiveSensor   sensor1 = CapacitiveSensor(2,3);        // 1M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
CapacitiveSensor   sensor2 = CapacitiveSensor(4,5);        // 1M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire and or foil
CapacitiveSensor   sensor3 = CapacitiveSensor(6,7);        // 1M resistor between pins 4 & 8, pin 8 is sensor pin, add a wire and or foil
CapacitiveSensor   sensor4 = CapacitiveSensor(8,9);
CapacitiveSensor   sensor5 = CapacitiveSensor(10,11);
CapacitiveSensor   sensor6 = CapacitiveSensor(12,32);
CapacitiveSensor   sensor7 = CapacitiveSensor(24,25);
CapacitiveSensor   sensor8 = CapacitiveSensor(26,27);
CapacitiveSensor   sensor9 = CapacitiveSensor(33,34);
CapacitiveSensor   sensor10 = CapacitiveSensor(35,36);
CapacitiveSensor   sensor11 = CapacitiveSensor(37,38);
CapacitiveSensor   sensor12 = CapacitiveSensor(39,40);
CapacitiveSensor   sensor13 = CapacitiveSensor(14,15);
CapacitiveSensor   sensor14 = CapacitiveSensor(16,17);
CapacitiveSensor   sensor15 = CapacitiveSensor(28,29);
CapacitiveSensor   sensor16 = CapacitiveSensor(30,31);

void setup()                    
{
   Serial.begin(9600);
   pinMode(led, OUTPUT);
   Ethernet.begin(mac, ip);

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

  // start UDP
  Udp.begin(localPort);   

   for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings1[thisReading] = 0;
    readings2[thisReading] = 0;
    readings3[thisReading] = 0;
    readings4[thisReading] = 0;
    readings5[thisReading] = 0;
    readings6[thisReading] = 0;
    readings7[thisReading] = 0;
    readings8[thisReading] = 0;
    readings9[thisReading] = 0;
    readings10[thisReading] = 0;
    readings11[thisReading] = 0;
    readings12[thisReading] = 0;
    readings13[thisReading] = 0;
    readings14[thisReading] = 0;
    readings15[thisReading] = 0;
    readings16[thisReading] = 0;
  }
}
      
void loop()                    
{
    /// threshhold knob
    long sensitivity = analogRead(sensitivityKnob);
    //threshhold = map(sensitivity, 4, 1023, 999, 3000);  ///enable knob

    // subtract the last reading:
    total[0] = total[0] - readings1[readIndex];
    readings1[readIndex] = sensor1.capacitiveSensor(10);
    total[0] = total[0] + readings1[readIndex];

    total[1] = total[1] - readings2[readIndex];
    readings2[readIndex] = sensor2.capacitiveSensor(10);
    total[1] = total[1] + readings2[readIndex];

    total[2] = total[2] - readings3[readIndex];
    readings3[readIndex] = sensor3.capacitiveSensor(30);
    total[2] = total[2] + readings3[readIndex];

    total[3] = total[3] - readings4[readIndex];
    readings4[readIndex] = sensor4.capacitiveSensor(30);
    total[3] = total[3] + readings4[readIndex];

    total[4] = total[4] - readings5[readIndex];
    readings5[readIndex] = sensor5.capacitiveSensor(30);
    total[4] = total[4] + readings5[readIndex];

    total[5] = total[5] - readings6[readIndex];
    readings6[readIndex] = sensor6.capacitiveSensor(30);
    total[5] = total[5] + readings6[readIndex];

    total[6] = total[6] - readings7[readIndex];
    readings7[readIndex] = sensor7.capacitiveSensor(30);
    total[6] = total[6] + readings7[readIndex];

    total[7] = total[7] - readings8[readIndex];
    readings8[readIndex] = sensor8.capacitiveSensor(30);
    total[7] = total[7] + readings8[readIndex];

    total[8] = total[8] - readings9[readIndex];
    readings9[readIndex] = sensor9.capacitiveSensor(30);
    total[8] = total[8] + readings9[readIndex];

    total[9] = total[9] - readings10[readIndex];
    readings10[readIndex] = sensor10.capacitiveSensor(30);
    total[9] = total[9] + readings10[readIndex];

    total[10] = total[10] - readings11[readIndex];
    readings11[readIndex] = sensor11.capacitiveSensor(30);
    total[10] = total[10] + readings11[readIndex];

    total[11] = total[11] - readings12[readIndex];
    readings12[readIndex] = sensor12.capacitiveSensor(30);
    total[11] = total[11] + readings12[readIndex];

    total[12] = total[12] - readings13[readIndex];
    readings13[readIndex] = sensor13.capacitiveSensor(30);
    total[12] = total[12] + readings13[readIndex];

    total[13] = total[13] - readings14[readIndex];
    readings14[readIndex] = sensor14.capacitiveSensor(30);
    total[13] = total[13] + readings14[readIndex];

    total[14] = total[14] - readings15[readIndex];
    readings15[readIndex] = sensor15.capacitiveSensor(30);
    total[14] = total[14] + readings15[readIndex];

    total[15] = total[15] - readings16[readIndex];
    readings16[readIndex] = sensor16.capacitiveSensor(30);
    total[15] = total[15] + readings16[readIndex];
  
    readIndex = readIndex + 1;

    // if we're at the end of the array...
    if (readIndex >= numReadings) {
      // ...wrap around to the beginning:
      readIndex = 0;
    }

    // calculate the average:
    for(int i; i<=sensorCount; i++){
    average[i] = total[i] / numReadings;
    }

//// Uncomment for setup

for(int i = 0; i <= sensorCount-1; i++){
  Serial.print(i+1);
  Serial.print(":");
  Serial.print(average[i]);
  Serial.print("\t");
  if(i == sensorCount-1){
     Serial.print("threshhold: " + String(threshhold));
     Serial.println("\t");
     delay(10);
    }
  }
 
  readSensor(1, 60);
  readSensor(2, 61);
  readSensor(3, 62);
  readSensor(4, 63);
  readSensor(5, 64);
  readSensor(6, 65);
  readSensor(7, 66);
  readSensor(8, 67);
  readSensor(9, 68);
  readSensor(10, 69);
  readSensor(11, 70);
  readSensor(12, 71);
  readSensor(13, 72);
  readSensor(14, 73);
  readSensor(15, 74);
  readSensor(16, 75);
  delay(50);

  while (usbMIDI.read()) {
    // ignore incoming messages
  }
}

void readSensor(int sensorNumber, int midiNote){
   if (average[sensorNumber-1] > threshhold && touchFlag[sensorNumber-1] == 0){
      
      Serial.print("touch" + String(sensorNumber) + ": ");
      Serial.println(average[sensorNumber-1]);
     
      if(sensorNumber == 1 || 2 || 3 || 4){
      sensor1.reset_CS_AutoCal();
      sensor2.reset_CS_AutoCal();
      sensor3.reset_CS_AutoCal();
      sensor4.reset_CS_AutoCal();
      }

      if(sensorNumber == 5 || 6 || 7 || 8){
      sensor5.reset_CS_AutoCal();
      sensor6.reset_CS_AutoCal();
      sensor7.reset_CS_AutoCal();
      sensor8.reset_CS_AutoCal();
      }
      
      if(sensorNumber == 9 || 10 || 11 || 12){
      sensor9.reset_CS_AutoCal();
      sensor10.reset_CS_AutoCal();
      sensor11.reset_CS_AutoCal();
      sensor12.reset_CS_AutoCal();
      }

      if(sensorNumber == 13 || 14 || 15 || 16){
      sensor13.reset_CS_AutoCal();
      sensor14.reset_CS_AutoCal();
      sensor15.reset_CS_AutoCal();
      sensor16.reset_CS_AutoCal();
      }
     
      for(int i; i<8; i++){
         touchFlag[i] = 0;
        }
      touchFlag[sensorNumber-1] = 1;

      ///Send UDP
      Udp.beginPacket(pc, 8888);
      Udp.write(symbolNum[sensorNumber-1]);
      Udp.endPacket();

      ///Send MIDI ON
      usbMIDI.sendNoteOn(midiNote, 127, channel);  // 60 = C4

      ///Flash LED
      elapsedMillis timeElapsed;
      while(timeElapsed < interval){
      //setPixel(sensorNumber ,0, 0, 255);
      digitalWrite(led, HIGH);
      }      

      ///Send MIDI OFF
      usbMIDI.sendNoteOff(midiNote, 0, channel);  // 60 = C4
      digitalWrite(led, LOW);
    }
    
    if (average[sensorNumber-1] < threshhold){
      touchFlag[sensorNumber-1] = 0;
      }

  }
