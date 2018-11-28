#include <Servo.h>

String a,xval,yval;
int x1=730,y1=730;
Servo uservo;  // create servo object to control a servo
Servo dservo;
double l = 730,x,y,r,th,phi,uval,dval;
void setup() {
  Serial.begin(9600);
  uservo.attach(9);
  dservo.attach(10);
  uservo.write(90);
  delay(10);
  dservo.write(90);
  delay(10);
}

void loop() {

  while(Serial.available()) {

  a= Serial.readString();// read the incoming data as string
  Serial.println(a);
  String xval = getValue(a, ',', 0);
  String yval = getValue(a, ',', 1);

  Serial.println("X:" + xval);
  Serial.println("Y:" + yval);

  x1 = xval.toInt();
  y1 = yval.toInt();
}

  x = (double)x1;
  y = (double)y1;
  r = sqrt((x*x)+(y*y));
  th = round( atan2 (y, x) * 180/3.14159265 );
  phi = acos(r/(2*l)) * 180/3.14159265 ;
  uval = th + phi;
  dval = th - phi;
  /*Serial.print(x);
  Serial.print("  , ");
  Serial.print(y);
  Serial.print("  , ");
  Serial.print(r);
  Serial.print("  , ");
  Serial.print(th);
  Serial.print("  , ");
  Serial.print(phi);
  Serial.print("  , ");
  Serial.print(180-(th+phi));
  Serial.print("  , ");
  Serial.println(th-phi+90);*/
  uservo.write(180-uval);                  
  dservo.write(dval+90);
  delay(50);   
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
