
#include "Arduino.h"
#include <Servo.h>
#include <digitalWriteFast.h>
#include <util/delay.h>   // library for high performance reads and writes by jrraines
                               // see http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1267553811/0
                               // and http://code.google.com/p/digitalwritefast/
 
// It turns out that the regular digitalRead() calls are too slow and bring the arduino down when
// I use them in the interrupt routines while the motor runs at full speed creating more than
// 40000 encoder ticks per second per motor.
 
// Quadrature encoders
// Left encoder
#define c_LeftEncoderInterrupt INT1
#define c_LeftEncoderPinA 3
#define c_LeftEncoderPinB 5
#define LeftEncoderIsReversed
volatile bool _LeftEncoderBSet;
volatile long _LeftEncoderTicks = 16179;


// Right encoder
#define c_RightEncoderInterrupt INT0
#define c_RightEncoderPinA 2
#define c_RightEncoderPinB 4
volatile bool _RightEncoderBSet;
volatile long _RightEncoderTicks = 16693;


//Left motor
#define L_MOTOR_1 8
#define L_MOTOR_2 10
#define L_MOTOR_PWM 9

//Right motor
#define R_MOTOR_1 13
#define R_MOTOR_2 12
#define R_MOTOR_PWM 11

int servopos;
Servo myservo;


#define STEP_MARGIN 1L        //10 - 1000 (1)
#define MIN_DUTYCYCLE 50      //0 - 255 (125)
#define MAX_DUTYCYCLE 225     //0 - 255 (255)
#define P_FRACTION 1.5 

unsigned long stepTime = 1;
unsigned long millisStart = 0;
int R_dutyCycle = 100; // 10 - 255
signed long R_setPoint = 0;
signed long R_actualPoint = 0;
int L_dutyCycle = 100; // 10 - 255
signed long L_setPoint = 0;
signed long L_actualPoint = 0;
String pos;

void establishContact() {
  while (Serial.available() <= 0) {
    //Serial.print('X');   // send a capital X
    delay(3000);
  }
}

void setup()
{
  Serial.begin(115200);
  establishContact();
 // Quadrature encoders

  // Left encoder
  pinMode(c_LeftEncoderPinA, INPUT);      // sets pin A as input
  digitalWrite(c_LeftEncoderPinA, LOW);  // turn on pullup resistors
  pinMode(c_LeftEncoderPinB, INPUT);      // sets pin B as input
  digitalWrite(c_LeftEncoderPinB, LOW);  // turn on pullup resistors
  attachInterrupt(c_LeftEncoderInterrupt, HandleLeftMotorInterruptA, RISING);
 
  // Right encoder
  pinMode(c_RightEncoderPinA, INPUT);      // sets pin A as input
  digitalWrite(c_RightEncoderPinA, LOW);  // turn on pullup resistors
  pinMode(c_RightEncoderPinB, INPUT);      // sets pin B as input
  digitalWrite(c_RightEncoderPinB, LOW);  // turn on pullup resistors
  attachInterrupt(c_RightEncoderInterrupt, HandleRightMotorInterruptA, RISING);

  pinMode(L_MOTOR_1, OUTPUT); digitalWrite(L_MOTOR_1, HIGH);
  pinMode(L_MOTOR_2, OUTPUT); digitalWrite(L_MOTOR_2, HIGH);
  pinMode(R_MOTOR_1, OUTPUT); digitalWrite(R_MOTOR_1, HIGH);
  pinMode(R_MOTOR_2, OUTPUT); digitalWrite(R_MOTOR_2, HIGH);
  pinMode(L_MOTOR_PWM, OUTPUT);
  pinMode(R_MOTOR_PWM, OUTPUT);
  
  myservo.attach(6);


}
 
void loop()
{
  R_actualPoint = _RightEncoderTicks;
  L_actualPoint = _LeftEncoderTicks;
  
  
  Serial.print(_LeftEncoderTicks);
  Serial.print(",");
  Serial.print(_RightEncoderTicks);
  Serial.print("\n");
 
  delay(20);
  millisStart = millis();
  while(millis() - millisStart < stepTime){
    
    R_dutyCycle = (double)(abs(R_setPoint - R_actualPoint)) * (double)P_FRACTION;
      if(R_dutyCycle < MIN_DUTYCYCLE){
        R_dutyCycle = MIN_DUTYCYCLE;
      }
      if(R_dutyCycle > MAX_DUTYCYCLE){
        R_dutyCycle = MAX_DUTYCYCLE;
      }


      if(abs(R_setPoint - R_actualPoint) < STEP_MARGIN){
        analogWrite(R_MOTOR_PWM, 0);
        digitalWrite(R_MOTOR_1, 0);
        digitalWrite(R_MOTOR_2, 0);
        R_dutyCycle = MIN_DUTYCYCLE;
        //Serial.print('r');
      }
      else{
        if(R_actualPoint < R_setPoint){
          digitalWrite(R_MOTOR_1, 1);
          digitalWrite(R_MOTOR_2, 0);
          analogWrite(R_MOTOR_PWM, R_dutyCycle); //255-duty cycle
        }
        if(R_actualPoint > R_setPoint){
          digitalWrite(R_MOTOR_1, 0);
          digitalWrite(R_MOTOR_2, 1);
          analogWrite(R_MOTOR_PWM, R_dutyCycle);
        }
      }
        
    L_dutyCycle = (double)(abs(L_setPoint - L_actualPoint)) * (double)P_FRACTION;
      if(L_dutyCycle < MIN_DUTYCYCLE){
        L_dutyCycle = MIN_DUTYCYCLE;
      }
      if(L_dutyCycle > MAX_DUTYCYCLE){
        L_dutyCycle = MAX_DUTYCYCLE;
      }


      if(abs(L_setPoint - L_actualPoint) < STEP_MARGIN){
        analogWrite(L_MOTOR_PWM, 0);
        digitalWrite(L_MOTOR_1, 0);
        digitalWrite(L_MOTOR_2, 0);
        L_dutyCycle = MIN_DUTYCYCLE;
        //Serial.print('l');
      }
      else{
        if(L_actualPoint < L_setPoint){
          digitalWrite(L_MOTOR_1, 1);
          digitalWrite(L_MOTOR_2, 0);
          analogWrite(L_MOTOR_PWM, L_dutyCycle); //255-duty cycle
        }
        if(L_actualPoint > L_setPoint){
          digitalWrite(L_MOTOR_1, 0);
          digitalWrite(L_MOTOR_2, 1);
          analogWrite(L_MOTOR_PWM, L_dutyCycle);
        }
      }
  }
  
  if (Serial.available()>0){
  pos = Serial.readString();
  L_setPoint = getValue(pos,',',1).toInt(); 
  R_setPoint = getValue(pos,',',2).toInt();
  servopos = getValue(pos,',',0).toInt();
  if(servopos == 0){myservo.write(15);}
  if(servopos == 1){myservo.write(90);}
  //Serial.print(L_setPoint);Serial.print(',');Serial.println(R_setPoint);
  }
  
delay(20);

}
 
// Interrupt service routines for the left motor's quadrature encoder
void HandleLeftMotorInterruptA()
{
  // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
  _LeftEncoderBSet = digitalReadFast(c_LeftEncoderPinB);   // read the input pin
 
  // and adjust counter + if A leads B
  #ifdef LeftEncoderIsReversed
    _LeftEncoderTicks -= _LeftEncoderBSet ? -1 : +1;
  #else
    _LeftEncoderTicks += _LeftEncoderBSet ? -1 : +1;
  #endif
}
 
// Interrupt service routines for the right motor's quadrature encoder
void HandleRightMotorInterruptA()
{
  // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
  _RightEncoderBSet = digitalReadFast(c_RightEncoderPinB);   // read the input pin
 
  // and adjust counter + if A leads B
  #ifdef RightEncoderIsReversed
    _RightEncoderTicks -= _RightEncoderBSet ? -1 : +1;
  #else
    _RightEncoderTicks += _RightEncoderBSet ? -1 : +1;
  #endif
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

