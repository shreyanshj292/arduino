#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
int yam = 0;
int pitch = 0;
int roll = 0;
int ch8 = 0; 
struct Signal {
  byte ch1;
  byte ch2;
  byte ch3;
  byte ch4;
  byte ch5;
  byte ch6;
  byte ch7;
  byte ch8;
};
Signal data;
const byte pipeIn[6] = "12345";
RF24 radio(9, 10);
void ResetData()
{
  data.ch1 = 127;
  data.ch2 = 127;
  data.ch3 = 200;
  data.ch4 = 127;
  data.ch5 = 127;
  data.ch6 = 127;
  data.ch7 = 127;
  data.ch8 = 127;
}
int p1 = 0, p2 = 0, p3 = 0, p4 = 0, p5 = 0, p6 = 0, p7 = 0, p8 = 0;
void setup()
{
  ResetData();
  radio.begin();
  radio.openReadingPipe(0, pipeIn);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening(); //start the radio comunication for receiver | Alıcı olarak sinyal iletişimi başlatılıyor
  Serial.begin(9600);
}
int a;
void loop()
{ 
  radio.startListening();
  delay(60);
  a = radio.read(&data, sizeof(Signal));
  if (data.ch1  <= 4)
  {
    p1 = 1; // fire
  }
  else if (120 < data.ch1 < 135)
  {
    p1 = 2; // reload
  }
  else if (160 < data.ch1 < 175)
  {
    p1 = 3; // flashbang
  }
  else if (185 < data.ch1 < 193)
  {
    p1 = 4; // grenade
  }
  else{
    p1 = 0;
  }

 /////////////////////////////////////analog pin A0///////////////////////////////////////////////////////////
  
  if (data.ch2  <= 4)
  {
    p2 = 1; // aim
  }
  else if (120 < data.ch2 < 135)
  {
    p2 = 2; // toogle
  }
  else if (160 < data.ch2 < 175)
  {
    p2 = 3; // exchange
  }
  else if (185 < data.ch2 < 193)
  {
    p2 = 4; // melee
  }
  else{
    p2 = 0;
  }  

  /////////////////////////////////analog pin A1////////////////////////////////////////////////////////////////

  if (data.ch3  <= 4)
  {
    p3 = 1; // sprint
  }
  else if (120 < data.ch3 < 135)
  {
    p3 = 2; // crouch
  }
  else if (160 < data.ch3 < 175)
  {
    p3 = 3; // prone
  }
  else if (185 < data.ch3 < 193)
  {
    p3 = 4; // special weapon
  }
  else if (197 < data.ch3 < 203)
  {
    p3 = 5; // night vision
  }
  else{
    p3 = 0;
  }  

  
  ///////////////////////////////analog pin A2//////////////////////////////////////////////////////////////////////

  yam = data.ch5;
  pitch = data.ch6;
  roll = data.ch7;

  Serial.print("?");
  Serial.print(p1);
  Serial.print("?");
  Serial.print(p2);
  Serial.print("?");
  Serial.print(p3);
  Serial.print("?");
  Serial.print(yam);
  Serial.print("?");
  Serial.print(pitch);
  Serial.print("?");
  Serial.print(roll);
  Serial.print("?");
  radio.stopListening();
  delay(60);
}
