#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);

const byte rxAddr[6] = "12345";
struct data{
  byte chs, ch0, ch1, ch2, ch3, ch4, ch5, ch6, ch7;
};
data sent_data;
void setup()
{
  radio.begin();
  radio.openWritingPipe(rxAddr);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  sent_data.chs = 127;
  sent_data.ch0 = 127;
  sent_data.ch1 = 127;
  sent_data.ch2 = 127;
  sent_data.ch3 = 127;
  sent_data.ch4 = 127;
  sent_data.ch5 = 127;
  sent_data.ch6 = 127;
  sent_data.ch7 = 127;
  Serial.begin(9600);
}

void loop()
{
  radio.stopListening();
  int a = 5;
  sent_data.chs = map( analogRead(2), 0, 255, 0, 255);
  sent_data.ch0 = map( analogRead(A7), 0, 1024, 0, 255);
  sent_data.ch1 = map( analogRead(A0), 0, 1024, 0, 255);
  sent_data.ch2 = map( analogRead(A1), 0, 1024, 0, 255);
  sent_data.ch3 = map( analogRead(A4), 0, 1024, 0, 255);
  sent_data.ch4 = map( analogRead(A2), 0, 1024, 0, 255);
  sent_data.ch5 = map( analogRead(A3), 0, 1024, 0, 255);
  sent_data.ch6 = map( analogRead(A5), 0, 1024, 0, 255);
  sent_data.ch7 = map( analogRead(A6), 0, 1024, 0, 255);
  bool b = radio.write(&sent_data, sizeof(data));
  Serial.println(b);
  Serial.print(sent_data.chs);
  Serial.print(" ");
  Serial.print(sent_data.ch0);
  Serial.print(" ");
  Serial.print(sent_data.ch1);
  Serial.print(" ");
  Serial.print(sent_data.ch2);
  Serial.print(" ");
  Serial.print(sent_data.ch3);
  Serial.print(" ");
  Serial.print(sent_data.ch4);
  Serial.print(" ");
  Serial.print(sent_data.ch5);
  Serial.print(" ");
  Serial.print(sent_data.ch6);
  Serial.print(" ");
  Serial.print(sent_data.ch7);
  Serial.print(" ");
  radio.startListening();
  delay(50);
}
