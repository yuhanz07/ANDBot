#include <Servo.h>

// Create servo objects for four channels
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// Define servo pins (adjust as needed)
const int servo1Pin = 5;
const int servo2Pin = 6;
const int servo3Pin = 9;
const int servo4Pin = 10;

// Control case variable (default case is 1)
int controlCase = 0;

void setup() {
  // Attach servos to their respective pins
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  servo4.attach(servo4Pin);

  // Initialize Serial communication for keyboard input
  Serial.begin(9600);
  Serial.println("Robot Control Initialized");
  Serial.println("Enter case number (1, 2, or 3) via keyboard:");
}

void loop() {
  // Check for serial input to change the control case
  if (Serial.available() > 0) {
    char inChar = Serial.read();
    if (inChar == '1' || inChar == '2' || inChar == '3') {
      controlCase = inChar - '0'; // Convert char digit to integer
      Serial.print("Switching to case ");
      Serial.println(controlCase);
      executeControlCase(controlCase);
    } else {
      Serial.println("Invalid input. Please enter 1, 2, or 3.");
    }
  }
  // You can add other periodic tasks here if needed
}

void executeControlCase(int caseNum) {
  Serial.print("Executing case ");
  Serial.println(caseNum);
  
  switch (caseNum) {
    case 1:
      // Case 1: Land Control
      // For continuous rotation servos, values less than 90 or greater than 90 make them spin.
      servo1.write(0);   // spin in one direction
      servo2.write(0);   // spin in the same direction
      // Set servo3 and servo4 to maintain a neutral position (90°)
      servo3.write(90);  
      servo4.write(90);   // need mounted in reverse direction
      break;
      
    case 2:
      // Case 2: Stop servo1 & servo2; servo3 sweeps from 90° to 180°, servo4 sweeps from 90° to 0°
      servo1.write(90);  // stop
      servo2.write(90);  // stop
      
      // Gradually sweep servo3 from 90° to 180°
      for (int pos = 90; pos <= 180; pos++) {
        servo3.write(pos);
        delay(15);  // adjust delay for smooth motion   //for better stop use millis() instead
      }
      
      // Gradually sweep servo4 from 90° to 0° (simulate moving from -90 to -180)
      for (int pos = 90; pos >= 0; pos--) {
        servo4.write(pos);
        delay(15);
      }
      break;
      
    case 3:
      // Case 3: servo3 and servo4 hold extreme positions; servo1 & servo2 spin in opposite directions
      servo3.write(0);  // set to extreme position (180°)
      servo4.write(0);    // set to extreme position (interpreted as -180°)
      
      // For continuous rotation: one spins clockwise, the other anticlockwise
      servo1.write(180);    // spin full speed in one direction
      servo2.write(0);  // spin full speed in the opposite direction
      break;
      
    default:
      // Default behavior: stop all servos
      servo1.write(90);
      servo2.write(90);
      servo3.write(90);
      servo4.write(90);
      break;
  }
}
