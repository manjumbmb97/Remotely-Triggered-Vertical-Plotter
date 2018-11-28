//Turn a DC motor into a stepper motor with an encoder disc
//
//
//The circuit:
//LCD RS pin to digital pin 11
//LCD Enable pin to digital pin 12
//LCD D4 pin to digital pin 2
//LCD D5 pin to digital pin 3
//LCD D6 pin to digital pin 4
//LCD D7 pin to digital pin 5
//LCD R/W pin to ground
//10K potentiometer:
//ends to +5V and ground
//wiper to LCD VO pin (pin 3)
//
//Source & info at: www.HomoFaciens.de/technics-computer-arduino-uno_en_navion.htm

// include the library code:
#include <LiquidCrystal.h>
#include <util/delay.h>

int stepStatus = 0;
int sensorStatusA = 0;
int sensorStatusB = 0;
int count = 0;
unsigned long stepTime = 1;
unsigned long millisStart = 0;
int dutyCycle = 100; // 10 - 255
signed long setPoint = 0;
signed long actualPoint = 0;
byte readByte = 0;

#define SENSOR_A 2
#define SENSOR_B 4

#define MOTOR_DIRECTION 7
#define MOTOR_PWM 6

#define STEP_MARGIN 1L        //10 - 1000 (1)
#define MIN_DUTYCYCLE 50      //0 - 255 (125)
#define MAX_DUTYCYCLE 255     //0 - 255 (255)
#define P_FRACTION 10.0        //Proportional factor of control loop 0.001 - 10.0 (1.0)


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print('X');   // send a capital X
    delay(300);
  }
}


void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Waiting for     ");
  lcd.setCursor(0, 1);
  lcd.print("commands...     ");
  
  pinMode(SENSOR_A, INPUT);
  pinMode(SENSOR_B, INPUT);  
  pinMode(MOTOR_DIRECTION, OUTPUT);  
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
        digitalWrite(MOTOR_DIRECTION, 0);
        dutyCycle = MIN_DUTYCYCLE;
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

      
  }//if(readByte > 0){

  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):

  if(millis() > 90000){
    lcd.setCursor(0, 0);
    lcd.print("actualPoint:");
    if(actualPoint>0){
      lcd.print(" ");
    }
    if(abs(actualPoint)<100){
      lcd.print(" ");
    }
    if(abs(actualPoint)<10){
      lcd.print(" ");
    }
    if(abs(actualPoint)==0){
      lcd.print(" ");
    }
    lcd.print(actualPoint);

    lcd.setCursor(0, 1);
    lcd.print("setPoint:   ");
    if(setPoint>0){
      lcd.print(" ");
    }
    if(abs(setPoint)<100){
      lcd.print(" ");
    }
    if(abs(setPoint)<10){
      lcd.print(" ");
    }
    if(abs(setPoint)==0){
      lcd.print(" ");
    }
    lcd.print(setPoint);
  }
  else{
    lcd.setCursor(0, 0);
    lcd.print("Count = ");
    if(actualPoint>0){
      lcd.print(" ");
    }
    if(abs(actualPoint)<100){
      lcd.print(" ");
    }
    if(abs(actualPoint)<10){
      lcd.print(" ");
    }
    if(abs(actualPoint)==0){
      lcd.print(" ");
    }
    lcd.print(actualPoint);

    lcd.setCursor(0, 1);
    lcd.print("                ");
  }
  
   
}


