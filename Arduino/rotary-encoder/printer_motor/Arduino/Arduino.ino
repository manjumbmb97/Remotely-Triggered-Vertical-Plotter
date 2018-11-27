//Printer motor
//Source & info at: www.HomoFaciens.de/technics-base-circuits-encoder-disc_en_navion.htm

#include <util/delay.h>

int stepStatus = 0;
//int stepStatusOld = 0;
byte sensorStatusA = 0;
byte oldSensorStatusA = 0;
byte sensorStatusB = 0;
int dutyCycle = 0;
signed long setPoint = 0;
signed long actualPoint = 0;
byte readByte = 0;

#define SENSOR_A 3
#define SENSOR_B 2

#define MOTOR_DIRECTION 5
#define MOTOR_PWM 6


#define P_FRACTION 1.0     //Proportional factor of control loop 0.001 - 10.0 (1.0)
#define STEP_MARGIN 4L     //10 - 1000 (1)

#define MIN_DUTYCYCLE 110   //0 - 255 (125)
#define MAX_DUTYCYCLE 180  //0 - 255 (255)



void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print('X');     // send a capital X to indicate that Arduino is running
    delay(300);
  }
}

void change_step(){
  sensorStatusA = digitalRead(SENSOR_A);

   if ((oldSensorStatusA == LOW) && (sensorStatusA == HIGH)) {
     if (sensorStatusB == 0) {
       actualPoint++;
     }
     else{
       actualPoint--;
     }
   } 
   oldSensorStatusA = sensorStatusA;
}

void change_sensor_A(){
  change_step();
}

void change_sensor_B(){
  sensorStatusB = digitalRead(SENSOR_B);
}


void setup() {
  
  digitalWrite(MOTOR_DIRECTION, 0);
  analogWrite(MOTOR_PWM, MAX_DUTYCYCLE);
  delay(1000);
  analogWrite(MOTOR_PWM, 0);
  delay(100);
  
  attachInterrupt(0, change_sensor_A, CHANGE);
  attachInterrupt(1, change_sensor_B, CHANGE);
  
  sensorStatusA = digitalRead(SENSOR_A);
  sensorStatusB = digitalRead(SENSOR_B);
  
  pinMode(MOTOR_DIRECTION, OUTPUT);  
  pinMode(MOTOR_PWM, OUTPUT);    
  

  // start serial port at 115200 bps:
  Serial.begin(115200);
  establishContact();  // send a byte to establish contact until receiver responds 
}

void loop() {    

      dutyCycle = (double)(abs(setPoint - actualPoint)) * (double)P_FRACTION;

      if(dutyCycle < MIN_DUTYCYCLE){
        dutyCycle = MIN_DUTYCYCLE;
      }
      if(dutyCycle > MAX_DUTYCYCLE){
        dutyCycle = MAX_DUTYCYCLE;
      }
      
      if(dutyCycle < MIN_DUTYCYCLE){
        dutyCycle = MIN_DUTYCYCLE;
      }
      if(dutyCycle > MAX_DUTYCYCLE){
        dutyCycle = MAX_DUTYCYCLE;
      }
      if(abs(setPoint - actualPoint) < STEP_MARGIN){
        analogWrite(MOTOR_PWM, 0);
        digitalWrite(MOTOR_DIRECTION, 0);
      }
      else{
        if(actualPoint < setPoint){
          digitalWrite(MOTOR_DIRECTION, 1);
          analogWrite(MOTOR_PWM, 255 - dutyCycle);
        }
        if(actualPoint > setPoint){
          digitalWrite(MOTOR_DIRECTION, 0);
          analogWrite(MOTOR_PWM, dutyCycle);
        }
      }
          
  
  
  readByte = 0;
  if (Serial.available() > 0){//if we get a valid byte, read from serial:
    //get incoming byte:
    readByte = Serial.read();
    Serial.print('r');   // send a 'r' to initiate next data from computer
  
    if(readByte > 0){
      if(readByte == 20){
        setPoint++;
      }
      if(readByte == 19){
        setPoint--;
      }
      if(readByte == 18){
        setPoint+=2875;
      }
      if(readByte == 17){
        setPoint-=2875;
      }
      if(readByte == 16){
        setPoint+=100;
      }
      if(readByte == 15){
        setPoint-=100;
      }
      if(readByte == 14){
        setPoint+=10;
      }
      if(readByte == 13){
        setPoint-=10;
      }
    }//if(readByte > 0){

  }//while (Serial.available() > 0){
   
}


