

void setup() {
  // put your setup code here, to run once:
 
pinMode(8,OUTPUT);
pinMode(9,OUTPUT);
pinMode(10,OUTPUT);
pinMode(11,OUTPUT);
pinMode(12,OUTPUT);
pinMode(13,OUTPUT);
 
}

void loop() {
  // put your main code here, to run repeatedly:
    analogWrite(9,150);
    digitalWrite(8,HIGH);//left  8-H 10-L out
    digitalWrite(10,LOW);

    delay(100);

    digitalWrite(8,HIGH);
    digitalWrite(10,HIGH);

    delay(100);

    analogWrite(11,150);
    digitalWrite(12,LOW);//right 12-L 13-H out 
    digitalWrite(13,HIGH);

    delay(100);

    digitalWrite(12,HIGH);
    digitalWrite(13,HIGH);

    delay(100);
}
