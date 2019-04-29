/*
  _      _                    __            _                 _   _
  \ \    / /                   |  _|          | |               | | ()
  \ \  / /_ _ _  _   _ _  | |_  _  _ _ | | _  _ _ _ | | _  _  _ _
   \ \/ / _ \ '_ \| | | / | |  _| \ \/ / ' \| |/ _ \| '/ _` | _| |/ _ \| ' \
    \  /  / | | | || \_ \ | |_ >  <| |) | | () | | | (| | || | () | | | |
     \/ \_|| ||\,|/ |_//\\ ./||\_/||  \,|\_||\_/|| ||
                                          | |
                                          |_|
    Authors: J.A.Arts, etc...
    Academic year 2016-2017
    Department of Electrical Engineering
    Eindhoven University of Technology

    Version 1.0
*/
#include <Servo.h>
#include <Math.h>
// [===========================  PIN NUMBERS  ===========================]
int encoderLeft = 7;                                      // Left Encoder.
int encoderRight = 8;                                     // Right Encoder.
int ultrasoundServo = 11;                                 // Servo for the Ultrasound.
const int pingPin = 9;                                    // Ultrasound.
int sweepArray[180];
int i, j;
int field[20][20];
int infraredLeft = A0;
int infraredRight = A1;
int infraredSample = A2;
int infraredDetectFront = A3;
int infraredDetectLeft = A4;
int infraredDetectRight = A5;
int samplePosition = 3 ;

// [===========================  PARAMETERS  ===========================]
Servo motorLeft;                                          //
Servo motorRight;                                         //
Servo USservo;                                            //
Servo GripperServo;                                       //
int oldEncoderReadRight ;                                 //
int oldEncoderReadLeft;                                   //
int counter;                                              //
int angle = 0;                                            //
int initialPosition[2] = {9, 9};
int currentPosition[2] = {9, 9};
char compas = 'N';


// [******** COMPONENTS *********]

int encoder_counterLeft(int x) {                                // [===========================  * ENCODER LEFT FUNCTION *  ===========================]
  x = (x / 1.27);                                               //
  counter = 0;                                                  //
  while (counter < x) {                                         //
    if (digitalRead(encoderLeft) != oldEncoderReadLeft) {       //
      oldEncoderReadLeft = digitalRead(encoderLeft);            //
      counter ++;                                               //
    }
  }
  return (1) ;
}

int encoder_counterRight(int x) {                               // [===========================  * ENCODER RIGHT FUNCTION *  ===========================]
  x = (x / 1.27);                                               //
  counter = 0;                                                  //
  while (counter < x) {                                         //
    if (digitalRead(encoderRight) != oldEncoderReadRight) {     //
      oldEncoderReadRight = digitalRead(encoderRight);          //
      counter ++;                                               //
    }                                                           //
  }                                                             //
  return (1);                                                   //
}                                                               //

void drive(int left, int right) {                               // [===========================  * DRIVE FUNCTION *  ===========================]
  switch (left) {
    case 0:
      motorLeft.writeMicroseconds(1500);
      break;
    case 1:
      motorLeft.writeMicroseconds(1000);
      break;
    case -1:
      motorLeft.writeMicroseconds(2000);
      break;
  }
  switch (right) {
    case 0:
      motorRight.writeMicroseconds(1500);
      break;
    case 1:
      motorRight.writeMicroseconds(2000);
      break;
    case -1:
      motorRight.writeMicroseconds(1000);
      break;

  }
}

int driveForward(int x) {                                     // [===========================  * DRIVE FORWARD FUNCTION *  ===========================]

  if (x == 0) {
    drive(1, 1);
    return (0);
  }

  while (1) {
    drive(1, 1);
    if (encoder_counterLeft(x) == 1) {
      drive(0, 0);
      break;
    }
  }
  return (1);
}

int driveLeft(int angle) {                                    // [===========================  * DRIVE LEFT FUNCTION *  ===========================]
  int x;
  float z;

  if (angle == 0) {
    drive(1, -1);
  }
  else {

    z =  3.14 * 12 * angle / 360 ;
    x = (int)z;
    while (1) {
      drive(1, -1);
      if (encoder_counterRight(x) == 1) {
        drive(0, 0);
        break;
      }
    }
    return (1);
  }
}

int driveRight(int angle) {                                  // [===========================  * DRIVE RIGHT FUNCTION *  ===========================]
  int x;
  float z;
  if (angle == 0) {
    drive(-1, 1);
  }
  else {
    z =  3.14 * 12 * angle / 360 ;
    x = (int)z;
    while (1) {
      drive(-1, 1);
      if (encoder_counterLeft(x) == 1) {
        drive(0, 0);
        break;
      }
    }
    return (1);
  }
}

int driveBackward(int x) {                                // [===========================  * DRIVE BACKWARD FUNCTION *  ===========================]
  while (1) {
    drive(-1, -1);
    if (encoder_counterLeft(x) == 1) {
      drive(0, 0);
      break;
    }
  }
  return (1);
}

int ultrasoundValue() {                                  // [===========================  * ULTRASOUND VALUE FUNCTION *  ===========================]
  long duration, cm;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  cm = duration / 58 ;
  return (cm);
}

void ultrasound_position(int x) {                          // [===========================  * ULTRASOUND POSITION FUNCTION *  ===========================]
  USservo.attach(ultrasoundServo);
  USservo.write(x);                                        //Angle 0-180 from R-L
}

void gripperOpen() {                                       // [===========================  * GRIPPER OPEN FUNCTION *  ===========================]
  GripperServo.write(115);                                 // Open the gripper.
}

void gripperClose() {                                      // [===========================  * GRIPPER CLOSE FUNCTION *  ===========================]
  GripperServo.write(0);                                   // Close the gripper.
}

int sweep() {                                              // [===========================  * SWEEP FUNCTION *  ===========================]
  for (i = 0; i < 180; i++) {
    ultrasound_position(i);
    delay(100);
    sweepArray[i] = ultrasoundValue();
    delay(100);

  }
  ultrasound_position(84);
}

int colorLeft() {
  if ((analogRead(infraredLeft) >= 330) ) {       //WHITE
    return (0);
  }
  if ((analogRead(infraredLeft) > 180) && (analogRead(infraredLeft) < 200)) {       //Green

    return (2);
  }
  if (analogRead(infraredLeft) < 100) {       //Black
    return (1);
  }

  else return (3);
}

int colorRight() {
  if ((analogRead(infraredRight) >= 340) ) {       //WHITE
    return (0);
  }
  if ((analogRead(infraredRight) > 280) && (analogRead(infraredRight) < 300)) {       //Green
    return (2);
  }
  if (analogRead(infraredRight) <= 100) {       //Black
    return (1);
  }

}

int colorLeftSearch() {
  if ((analogRead(infraredLeft) >= 30) ) {       //WHITE
    return (0);
  }
  else if (analogRead(infraredLeft) < 30) {       //Black
    return (1);
  }
  else {
    return (2) ;
  }

}

int colorRightSearch() {
  if ((analogRead(infraredRight) >= 50) ) {       //WHITE
    return (0);
  }
  if (analogRead(infraredRight) < 50) {       //Black
    return (1);
  }
  else {
    return (2) ;
  }
}

int labAllign(int allign) {

  if (allign == 0) {
    while (analogRead(infraredRight) < 400) {
      drive(1, 0);
    }
  }

  if (allign == 1) {
    while (analogRead(infraredLeft) < 280) {
      drive(0, 1);
    }
  }
  drive(0, 0);
  delay(1000);
}

// [******** SUB SYSTEMS **********]
int randomSearching() {
  delay(1000);
  while (1) {
    driveForward(0);
    if (colorLeftSearch() == 1) {
      Serial.print("Left: ");
      Serial.print(analogRead(infraredLeft));
      driveBackward(5);
      driveRight(90);
    }
    if (colorRightSearch() == 1) {
      Serial.print("Right: ");
      Serial.print(analogRead(infraredRight));
      driveBackward(5);
      driveLeft(90);
    }

    if (ultrasoundValue() < 15) {
      Serial.print("I saw something!");
      driveBackward(4);
      driveLeft(50);
    }
    if (analogRead(infraredDetectLeft) > 60) {
      samplePosition = 0 ;
      break;
    }
    if (analogRead(infraredDetectRight) > 60) {
      samplePosition = 1;
      break;
      if (analogRead(infraredRight) > 400) {
        Serial.println("white ");
        drive(0, 0);
        delay(1000);
        labAllign(1);
        return (0);
      }


      if (analogRead(infraredLeft) > 280) {
        Serial.println("white ");
        drive(0, 0);
        delay(5000);
        labAllign(0);
        return (0);
      }
      if (analogRead(infraredRight) > 400) {
        driveBackward(5);
        driveLeft(90);
      }


      if (analogRead(infraredLeft) > 280) {
        driveBackward(5);
        driveRight(90);
      }
    }
  }
}

int boardSample( int sample) {

  sample = samplePosition ;
  if (sample == 0) { //Left
    while (1) {
      driveBackward(15);

      driveLeft(0);
      delay(160);
      drive(0, 0);
      gripperOpen();
      delay(900);
      driveForward(0);
      delay(750);
      drive(0, 0);
      gripperClose();
      delay(500) ;
      return (0);
    }
  }
  else if (sample == 1) {
    while (1) {
      driveBackward(15);
      delay(2);
      driveRight(0);
      delay(160);
      drive(0, 0);
      gripperOpen();
      delay(900);
      driveForward(0);
      delay(750);
      drive(0, 0);
      gripperClose();
      delay(500);
      return (0);

    }
  }
}

int returnToLab() {
  delay(1000);

  while (1) {
    driveForward(0);
    if (analogRead(infraredRight) > 400) {
      Serial.println("white ");
      drive(0, 0);
      delay(1000);
      labAllign(1);
      return (0);
    }


    if (analogRead(infraredLeft) > 280) {
      Serial.println("white ");
      drive(0, 0);
      delay(5000);
      labAllign(0);
      return (0);
    }


    if (colorLeftSearch() == 1) {
      Serial.print("Left: ");
      driveBackward(5);
      driveRight(90);
    }
    if (colorRightSearch() == 1) {
      Serial.print("Right: ");
      driveBackward(5);
      driveLeft(90);
    }
    if (ultrasoundValue() < 15) {
      Serial.print("I saw something!");
      driveBackward(4);
      driveLeft(50);
    }
  }
}

int rampDrop() {
  driveForward(20);
  while (1) {
    driveForward(0);

    if (colorLeftSearch() == 1) {
      driveBackward(5);
      driveRight(25);
    }
    if (colorRightSearch() == 1) {
      driveBackward(5);
      driveLeft(25);
    }
    if (analogRead(infraredRight) > 700) {
      Serial.println("white ");
      drive(0, 0);
      delay(1000);
      labAllign(1);
      break;
    }


    if (analogRead(infraredLeft) > 600) {
      Serial.println("white ");
      drive(0, 0);
      delay(1000);
      labAllign(0);
      break;
    }

  }
  gripperOpen() ;
  delay(2000);
  gripperClose();
  delay(1000);
  driveBackward(90);
  driveLeft(45);
}


int avoidBordersCraters() {
  while (1) {
    driveBackward(0);
    if (colorLeftSearch() == 1) {
      driveRight(47);
    }
    if (colorRightSearch() == 1) {
      driveLeft(47) ;
    }
  }
}

// [******** SETUP & LOOP  **********]

void setup() {                                             // [===========================  * SETUP *  ===========================]
  Serial.begin(9600);                                      // Open the serial-port at 9600 baud [bits/sec].
  motorLeft.attach(12);                                    //
  motorRight.attach(13);                                   //
  for (i = 0; i < 10; i++) {                               //
    for (j = 0; j < 10; j++) {                             //
      field[i][j] == 0;
    }
  }
  GripperServo.attach(10);                                 //
  oldEncoderReadRight = digitalRead(encoderRight);         //
  oldEncoderReadLeft = digitalRead(encoderLeft);           //
  ultrasound_position(84);                                 //
  gripperClose();                                          // Set the Gripper to its initial position, which is closed.
  drive(0, 0);
  delay(2000);                                             // Initial delay.
}

int calibrate() {
  while (1) {
    Serial.print("gripper ");
    Serial.print(analogRead(infraredSample)) ;
    Serial.print("Left: ");
    Serial.print(analogRead(infraredLeft)) ;
    Serial.print("\n");
    Serial.print("Right: ");
    Serial.print(analogRead(infraredRight)) ;
    Serial.print("\n");
    Serial.print("\n");
    driveForward(0);
  }
}

void loop() {                                              // [===========================  * MAIN FUNCTION *  ===========================]
  //calibrate();
  delay(2000);
  while (1) {
    randomSearching();
    boardSample(samplePosition);
    if (analogRead(infraredSample) > 200) {
      break;
    }
  }
  delay(1000);

  returnToLab();
  rampDrop();
}
