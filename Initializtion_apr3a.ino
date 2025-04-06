#include <Servo.h>

Servo servo3;
Servo servo4;

const int servo3Pin = 9;
const int servo4Pin = 10;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  servo3.attach(servo3Pin);
  servo4.attach(servo4Pin);

  delay(200);

  servo3.write(180);
  servo4.write(180);

  Serial.println("Attached...");
}

void loop() {
  // put your main code here, to run repeatedly:

}


