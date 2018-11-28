
// include the library code:
#include <util/delay.h>

int stepStatus = 0;
int sensorStatusA = 0;
int sensorStatusB = 0;
int count = 0;
unsigned long stepTime = 5;
unsigned long millisStart = 0;
int dutyCycle = 100; // 10 - 255
signed long setPoint = 0;
signed long actualPoint = 0;
byte readByte = 0;

#define SENSOR_A 2
#define SENSOR_B 4

#define L_MOTOR_1 13
#define L_MOTOR_2 12
#define L_MOTOR_PWM 11

#define STEP_MARGIN 1L        //10 - 1000 (1)
#define MIN_DUTYCYCLE 50      //0 - 255 (125)
#define MAX_DUTYCYCLE 255     //0 - 255 (255)
#define P_FRACTION 10.0        //Proportional factor of control loop 0.001 - 10.0 (1.0)


// initialize the library with the numbers of the interface pins

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print('X');   // send a capital X
    delay(300);
  }
}


void setup() {
  // set up the LCD's number of columns and rows: 
 
  
  pinMode(SENSOR_A, INPUT);
  pinMode(SENSOR_B, INPUT);  
  pinMode(L_MOTOR_1, OUTPUT);  
  pinMode(L_MOTOR_2, OUTPUT);  
  pinMode(MOTOR_PWM, OUTPUT);    
  
  count = 0;
  // start serial port at 115200 bps:
  Serial.begin(115200);
  establishContact();  // send a byte to establish contact until receiver responds 
 
}

void loop() {    

  millisStart = millis();
  while(millis() - millisStart < stepTime){
      sensorStatusA = digitalRead(SENSOR_A);
      sensorStatusB = digitalRead(SENSOR_B);
    
      if(sensorStatusB == 0 && sensorStatusA == 0){
        if(stepStatus == 1){
          actualPoint--;
        }
        if(stepStatus == 3){
          actualPoint++;
        }
        stepStatus = 0;
      }
    
      if(sensorStatusB == 1 && sensorStatusA == 0){
        if(stepStatus == 0){
          actualPoint++;
        }
        if(stepStatus == 2){
          actualPoint--;
        }
        stepStatus = 1;
      }
    
      if(sensorStatusB == 1 && sensorStatusA == 1){
        if(stepStatus == 3){
          actualPoint--;
        }
        if(stepStatus == 1){
          actualPoint++;
        }
        stepStatus = 2;
      }
    
      if(sensorStatusB == 0 && sensorStatusA == 1){
        if(stepStatus == 2){
          actualPoint++;
        }
        if(stepStatus == 0){
          actualPoint--;
        }
        stepStatus = 3;
      }

      dutyCycle = (double)(abs(setPoint - actualPoint)) * (double)P_FRACTION;
      if(dutyCycle < MIN_DUTYCYCLE){
        dutyCycle = MIN_DUTYCYCLE;
      }
      if(dutyCycle > MAX_DUTYCYCLE){
        dutyCycle = MAX_DUTYCYCLE;
      }


      if(abs(setPoint - actualPoint) < STEP_MARGIN){
        analogWrite(MOTOR_PWM, 0);
        digitalWrite(L_MOTOR_1, 0);
        digitalWrite(L_MOTOR_2, 0);
        dutyCycle = MIN_DUTYCYCLE;
      }
      else{
        if(actualPoint < setPoint){
          digitalWrite(L_MOTOR_1, 1);
          digitalWrite(L_MOTOR_2, 0);
          analogWrite(MOTOR_PWM, dutyCycle); //255-duty cycle
        }
        if(actualPoint > setPoint){
          digitalWrite(L_MOTOR_1, 0);
          digitalWrite(L_MOTOR_2, 1);
          analogWrite(MOTOR_PWM, dutyCycle);
        }
      }
      
    if(readByte == 0){
      millisStart = 0L;
    }
  }//while(millis() - millisStart < stepTime){
  
  readByte = 0;
  if (Serial.available() > 0){//if we get a valid byte, read from serial:
    //get incoming byte:
    readByte = Serial.read();
    Serial.print('r');   // send a 'r' to initiate next data from computer
  }//if (Serial.available() > 0){


  
  if(readByte > 0){

    if(readByte == 128){
      if(stepTime < 1000){
        stepTime+=10;
      }
    }
    if(readByte == 64){
      if(stepTime > 10){
        stepTime-=10;
      }
    }
    if(readByte == 32){
      setPoint++;
      dutyCycle = MAX_DUTYCYCLE;
    }
    if(readByte == 16){
      setPoint--;
      dutyCycle = MAX_DUTYCYCLE;
    }
    if(readByte == 8){
      setPoint-=100;
      dutyCycle = MAX_DUTYCYCLE;
    }
    if(readByte == 4){
      setPoint+=100;
      dutyCycle = MAX_DUTYCYCLE;
    }

      
  }
  
   
}


