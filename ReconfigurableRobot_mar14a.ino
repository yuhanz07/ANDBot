#include "arduino_secrets.h"
/* 
  Sketch generated by the Arduino IoT Cloud Thing "ReconfigurableRobot "
  https://create.arduino.cc/cloud/things/9aff2028-a945-4f42-89e8-11ee01053ae4 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  float servo_speed_l;
  float servo_speed_r;
  int robot_state;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

// ## how servos are mounted - joint servos; water mode wing servos should go like breaststroke
// ## too many loops:( -> only print when status changes
// ## delay -> millis()

// Joint servos need to be set to a ABSOLUTE 0or90 value beforehand
#include "thingProperties.h"
#include <Servo.h>

Servo wing_servo_l;
Servo wing_servo_r;
Servo joint_servo_l;
Servo joint_servo_r;

const int wing_servo_l_pin = 5;
const int wing_servo_r_pin = 6;
const int joint_servo_l_pin = 9;
const int joint_servo_r_pin = 10;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 
  // ## change to millis()

  // Set up servo pins
  wing_servo_l.attach(wing_servo_l_pin);
  wing_servo_r.attach(wing_servo_r_pin);
  joint_servo_l.attach(joint_servo_l_pin);
  joint_servo_r.attach(joint_servo_r_pin);

  // Initialize servo positions
  wing_servo_l.write(90);
  wing_servo_r.write(90);
  joint_servo_l.write(90); // I'm assuming Land as default ON/L
  joint_servo_r.write(90);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  Serial.println("Robot IoT Control Initialized");
}

// ## too many loops:( -> only print when status changes;; delay -> millis()
void loop() {
  ArduinoCloud.update();
  // Your code here 
  onRobotStateChange();
}

/*
  Since RobotState is READ_WRITE variable, onRobotStateChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onRobotStateChange()  {
  // Add your code here to act upon RobotState change
  Serial.print("Robot state changed to: ");
  Serial.println(robot_state);

  onServoSpeedLChange();
  onServoSpeedRChange();

  int mapped_speed_l = map(servo_speed_l, 0, 100, 90, 180);
  int mapped_speed_r = map(servo_speed_r, 0, 100, 90, 180);

  switch (robot_state) {
    
    case 0:
      // stop all servos
      wing_servo_l.write(90);
      wing_servo_r.write(90);
      
      // joint_servo_l.write(0);
      // joint_servo_r.write(0);
      
      break;
    
    case 1:
      // Land
      // For continuous rotation servos, values less than 90 or greater than 90 make them spin.
      wing_servo_l.write(180 - mapped_speed_l);  
      wing_servo_r.write(mapped_speed_r);   // 180- makes it go round in circles real fast; making different speed scale mapping makes it a panel
      
      // Set joint servos to maintain 90
      joint_servo_l.write(90);  
      joint_servo_r.write(90);   // ## check how they're mounted; need mounted in reverse direction
      
      break;
      
    case 2:
      // Land to water
      // Stop wing servos, joint servos sweep from 90 to 180/ from 90 to 0
      wing_servo_l.write(90);  // stop
      wing_servo_r.write(90);  
      
      // Ramping
      for (int pos = 90; pos <= 180; pos++) {
        joint_servo_l.write(pos);
        joint_servo_r.write(180 - pos);
        // ## sweep one by one? Did sweep one by one from water to land 
        delay(15);  // adjust delay for smooth motion   
        // ## for better stop use millis() instead
      }
      
      break;
      
    case 3:
      // Water
      // Wing servos spin in opposite directions, joint servos hold positions
      // ## check how they're mounted
      wing_servo_l.write(mapped_speed_l);    
      // ## speed control variable needed
      wing_servo_r.write(180 - mapped_speed_r);  
      
      joint_servo_l.write(180);  
      joint_servo_r.write(0);   
      
      break;

    case 4:
      // Water to land
      // Stop wing servos, joint servos sweep to 90
      wing_servo_l.write(90);    // stop
      wing_servo_r.write(90); 

      for (int pos = 180; pos >= 90; pos--) {
        joint_servo_l.write(pos);
        delay(15);  // adjust delay for smooth motion   
        // ## for better stop use millis() instead
      }
      for (int pos = 180; pos >= 90; pos--) {
        joint_servo_r.write(180 - pos);
        delay(15); 
      }
      
      break;
  }
}

/*
  Since ServoSpeedL is READ_WRITE variable, onServoSpeedLChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onServoSpeedLChange()  {
  // Add your code here to act upon ServoSpeedL change
  Serial.print("Left servo speed changed to: ");
  Serial.println(servo_speed_l);
}

/*
  Since ServoSpeedR is READ_WRITE variable, onServoSpeedRChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onServoSpeedRChange()  {
  // Add your code here to act upon ServoSpeedR change
  Serial.print("Right servo speed changed to: ");
  Serial.println(servo_speed_r);
}

