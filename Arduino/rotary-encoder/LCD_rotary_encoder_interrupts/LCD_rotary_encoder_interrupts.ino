// Rotary encoder with interrupts. Source & info:
// http://homofaciens.de/technics-base-circuits-encoder-disc_en.htm
//

#include <LiquidCrystal.h>

char lineText[]="  Hello World!  ";
int sensor01;
int sensor02;

int stepOld;
int step;

long PulseCount;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(6, 7, 8, 9, 10, 11);

#define SENSOR_A 3
#define SENSOR_B 2


void checkState(){
  sensor01 = digitalRead(SENSOR_A);
  sensor02 = digitalRead(SENSOR_B);

  if(sensor01 == 1 && sensor02 == 1){
    step = 0;
    if(stepOld == 1){
      PulseCount--;
    }
    if(stepOld == 3){
      PulseCount++;
    }
    stepOld = 0;
  }

  if(sensor01 == 0 && sensor02 == 1){
    step = 1;
    if(stepOld == 2){
      PulseCount--;
    }
    if(stepOld == 0){
      PulseCount++;
    }
    stepOld = 1;
  }

  if(sensor01 == 0 && sensor02 == 0){
    step = 2;
    if(stepOld == 3){
      PulseCount--;
    }
    if(stepOld == 1){
      PulseCount++;
    }
    stepOld = 2;
  }

  if(sensor01 == 1 && sensor02 == 0){
    step = 3;
    if(stepOld == 0){
      PulseCount--;
    }
    if(stepOld == 2){
      PulseCount++;
    }
    stepOld = 3;
  }

}

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");

  attachInterrupt(0, checkState, CHANGE);
  attachInterrupt(1, checkState, CHANGE);



  PulseCount = 0;
}

void loop() {

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);

  lcd.print("Pulses:");
  if(PulseCount > -1){
    lcd.print(" ");
  }

  if(PulseCount < 10 && PulseCount > -10){
    lcd.print(" ");
  }

  if(PulseCount < 100 && PulseCount > -100){
    lcd.print(" ");
  }

  lcd.print(PulseCount);
  if(sensor01 == 1){
    lcd.print(" HIGH");
  }
  else{
    lcd.print(" LOW ");
  }




  lcd.setCursor(0, 1);

  if(sensor02 == 1){
    lcd.print("            HIGH");
  }
  else{
    lcd.print("            LOW ");
  }

}


