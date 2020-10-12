#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
int ch_width_s = 0;
int ch_width_0 = 0;
int ch_width_1 = 0;
int ch_width_2 = 0;
int ch_width_3 = 0;
int ch_width_4 = 0;
int ch_width_5 = 0;
int ch_width_6 = 0;
int ch_width_7 = 0;
struct Signal {
  byte chs;
  byte ch0;
  byte ch1;
  byte ch2;
  byte ch3;
  byte ch4;
  byte ch5;
  byte ch6;
  byte ch7;
};
Signal data;
const byte pipeIn[6] = "12345";
RF24 radio(9, 10);
void ResetData()
{
  data.chs = 127;
  data.ch0 = 127;
  data.ch1 = 127;
  data.ch2 = 127;
  data.ch3 = 200;
  data.ch4 = 127;
  data.ch5 = 127;
  data.ch6 = 127;
  data.ch7 = 127;

}
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
unsigned long lastRecvTime = 0;
void loop()
{ 
  radio.startListening();
  delay(60);
  bool b = radio.read(&data, sizeof(Signal));
  lastRecvTime = millis();   // receive the data | data alınıyor
  unsigned long now = millis();
  if ( now - lastRecvTime > 1000 ) {
    ResetData(); // Signal lost.. Reset data | Sinyal kayıpsa data resetleniyor
  }
  ch_width_s = map(data.chs, 0, 255, 1000, 2000);
  ch_width_0 = map(data.ch0, 0, 255, 1000, 2000);
  ch_width_1 = map(data.ch1, 0, 255, 1000, 2000);     // pin D2 (PWM signal)
  ch_width_2 = map(data.ch2, 0, 255, 1000, 2000);     // pin D3 (PWM signal)
  ch_width_3 = map(data.ch3, 0, 255, 1000, 2000);     // pin D4 (PWM signal)
  ch_width_4 = map(data.ch4, 0, 255, 1000, 2000);     // pin D5 (PWM signal)
  ch_width_5 = map(data.ch5, 0, 255, 1000, 2000);     // pin D3 (PWM signal)
  ch_width_6 = map(data.ch6, 0, 255, 1000, 2000);     // pin D4 (PWM signal)
  ch_width_7 = map(data.ch7, 0, 255, 1000, 2000);     // pin D5 (PWM signal)
  Serial.print(ch_width_s);
  Serial.print(ch_width_0);
  Serial.print(ch_width_1);
  Serial.print(ch_width_2);
  Serial.print(ch_width_3);
  Serial.print(ch_width_4);
  Serial.print(ch_width_5);
  Serial.print(ch_width_6);
  Serial.print(ch_width_7);
  Serial.println("");
  radio.stopListening();
  delay(60);
}
