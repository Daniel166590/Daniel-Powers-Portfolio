#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMegaPi.h>

double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;
MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);
MeEncoderOnBoard Encoder_3(SLOT3);
MeEncoderOnBoard Encoder_4(SLOT4);
MeLineFollower linefollower_6(6);
int count = 0;
bool turnLeft = false;

void isr_process_encoder1(void){
  if(digitalRead(Encoder_1.getPortB()) == 0){
    Encoder_1.pulsePosMinus();
  }else{
    Encoder_1.pulsePosPlus();
  }
}

void isr_process_encoder2(void){
  if(digitalRead(Encoder_2.getPortB()) == 0){
    Encoder_2.pulsePosMinus();
  }else{
    Encoder_2.pulsePosPlus();
  }
}

void isr_process_encoder3(void){
  if(digitalRead(Encoder_3.getPortB()) == 0){
    Encoder_3.pulsePosMinus();
  }else{
    Encoder_3.pulsePosPlus();
  }
}

void isr_process_encoder4(void){
  if(digitalRead(Encoder_4.getPortB()) == 0){
    Encoder_4.pulsePosMinus();
  }else{
    Encoder_4.pulsePosPlus();
  }
}
void move(int direction, int speed){
  int leftSpeed = 0;
  int rightSpeed = 0;
  if(direction == 1){ // Right
    leftSpeed = -1 * speed;
    rightSpeed = speed;
  }else if(direction == 2){ // Left
    leftSpeed = speed;
    rightSpeed = -1 * speed;
  }else if(direction == 3){ // Forwards
    leftSpeed = speed;
    rightSpeed = speed;
  }else if(direction == 4){ // Backwards
    leftSpeed = -1 * speed;
    rightSpeed = -1 * speed;
  }
  Encoder_1.setTarPWM(leftSpeed);
  Encoder_2.setTarPWM(rightSpeed * 1.20);
}

void setup() {
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
  attachInterrupt(Encoder_1.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(Encoder_2.getIntNum(), isr_process_encoder2, RISING);
  // DO SOMETHING

}

void _loop() {
  Encoder_1.loop();
  Encoder_2.loop();
}

void loop() {
  int sensorValue = linefollower_6.readSensors();  // Read the sensor once

  if(sensorValue == 1.000000){ // White Line Right
    move(1, 50 / 100.0 * 255);
    delay(100);
    if (sensorValue == 1.000000) {
      turnLeft == true;
    }
  }
  if(sensorValue == 2.000000){ // White Line Left
    move(2, 50 / 100.0 * 255);
    delay(100);
    if (sensorValue == 2.000000) {
      turnLeft == false;
    }
  }
  if (sensorValue == 0.0000000) { // Black Line All
    turn();
    delay(100);
  }
  if(sensorValue == 3.000000){ // White Line All
    move(3, 30 / 100.0 * 255);
    delay(100);
  }

  _loop();
}

void turn() {
  if (turnLeft == false) {
    move(2, 35 / 100.0 * 255);  // Turn Left
  } else {
    move(1, 35 / 100.0 * 255);  // Turn Right
  }
}