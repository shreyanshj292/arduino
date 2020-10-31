#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10);
const byte rxAddr[6] = "12345";
struct data{
  byte ch1, ch2, ch3, ch4, ch5, ch6, ch7;
};
int ch1, ch2, ch3, ch4, ch5, ch6, ch7;
data sent_data;


#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
MPU6050 mpu;
#define OUTPUT_READABLE_YAWPITCHROLL
#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}


void setup()
{

  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (115200 chosen because it is required for Teapot Demo output, but it's
    // really up to you depending on your project)
    Serial.begin(9600);
    while (!Serial); 
    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);
    mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed");
    devStatus = mpu.dmpInitialize();
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
    if (devStatus == 0) {
        mpu.CalibrateAccel(6);
        mpu.CalibrateGyro(6);
        mpu.PrintActiveOffsets();
        mpu.setDMPEnabled(true);
        digitalPinToInterrupt(INTERRUPT_PIN);
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();
        dmpReady = true;
        packetSize = mpu.dmpGetFIFOPacketSize();
    } 
    pinMode(LED_PIN, OUTPUT);

  
  radio.begin();
  radio.openWritingPipe(rxAddr);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  sent_data.ch1 = 127;
  sent_data.ch2 = 127;
  sent_data.ch3 = 127;
  sent_data.ch4 = 127;
  sent_data.ch5 = 127;
  sent_data.ch6 = 127;
  sent_data.ch7 = 127;
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  Serial.begin(9600);
}
void loop()
{
  if (!dmpReady) return;
    while (!mpuInterrupt && fifoCount < packetSize) {
        if (mpuInterrupt && fifoCount < packetSize) {
          fifoCount = mpu.getFIFOCount();
        }  
    }
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();
    fifoCount = mpu.getFIFOCount();
  if(fifoCount < packetSize){
          //Lets go back and wait for another interrupt. We shouldn't be here, we got an interrupt from another event
      // This is blocking so don't do it   while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
  }
    // check for overflow (this should never happen unless our code is too inefficient)
    else if ((mpuIntStatus & _BV(MPU6050_INTERRUPT_FIFO_OFLOW_BIT)) || fifoCount >= 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
      //  fifoCount = mpu.getFIFOCount();  // will be zero after reset no need to ask
        Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & _BV(MPU6050_INTERRUPT_DMP_INT_BIT)) {

        // read a packet from FIFO
  while(fifoCount >= packetSize){ // Lets catch up to NOW, someone is using the dreaded delay()!
    mpu.getFIFOBytes(fifoBuffer, packetSize);
    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;
  }
        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            Serial.print("ypr\t");
            Serial.print(ypr[0] * 180/M_PI);
            ch5 = ypr[0] * 180/M_PI;
            Serial.print("\t");
            Serial.print(ypr[1] * 180/M_PI);
            ch6 = ypr[1] * 180/M_PI;
            Serial.print("\t");
            Serial.println(ypr[2] * 180/M_PI);
            ch7 = ypr[2] * 180/M_PI;
        #endif
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);
    }
  radio.stopListening();
  ch1 = analogRead(A0); //rv
  ch2 = analogRead(A1); //rh
  ch3 = analogRead(A2); //rb
  ch4 = analogRead(A3); //lv
  sent_data.ch1 = map(ch1, 0, 1023, 0, 255);
  sent_data.ch2 = map(ch2, 0, 1023, 0, 255);
  sent_data.ch3 = map(ch3, 0, 1023, 0, 255);
  sent_data.ch4 = map(ch4, 0, 1023, 0, 255);
  sent_data.ch5 = ch5;
  sent_data.ch6 = ch6;
  sent_data.ch7 = ch7;
  bool b = radio.write(&sent_data, sizeof(data));
  Serial.println("");
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
  delay(25);
}
