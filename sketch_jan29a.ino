/*
Signal Sling
Thomas Graham
DIGF-3010-001 Advanced Wearable Electronics
OCAD University
Created on January 29, 2020; last updated January 31, 2020
*/

int elbowSignalThreshold = 256; //Threshold values: how much signal do we consider 'high?' 
int armpitSignalThreshold = 256;
  //(Should really take baseline at boot instead—or implement some form of calibration)
  //I bet a potentiometer would be great for that
int squareMs = 500; //Length of square wave for blinker
int squareX = 0; //Position along square wave
int tickMs = 50; //High latency: less snappy but less finicky
  //#TODO: Address finickyness by lerping values
bool blinkerOn = false;
bool turningRight = false;
bool turningLeft = false;

void setup() {
  pinMode(A3, INPUT); //A3: Elbow signal  
  pinMode(A4, INPUT); //A4: Armpit signal
  pinMode(2, OUTPUT); //2: Left-turn signal
  pinMode(9, OUTPUT); //9: Right-turn signal
  Serial.begin(9600);  
}

void loop() {
  Serial.print("Elbow signal: ");
  Serial.println( analogRead(A3) );  
  
  Serial.print("Armpit signal: ");
  Serial.println( analogRead(A4) );

  //SIGNAL INTERPRETATION
  //Using the signal from the stretch sensors to determine state

  if ( analogRead (A4) > armpitSignalThreshold ) { //If arm is raised
    if ( analogRead (A3) > elbowSignalThreshold ) { //And forearm is pointed up, it's a right-turn signal
      turningRight = true;
    } else { //If otherwise, then it's a left-turn signal
      turningLeft = true;
    }
  } else { //If the arm is down, no hand signal is being made
    turningRight = false;
    turningLeft = false;
  }

  //BLINKER CONTROL
  //Here we're keeping a square wave going for the blinkers

  if (blinkerOn) { //If the square wave is at the end of its cycle
    if (turningRight) { //And we're turning right
      digitalWrite(9, HIGH);
    } else if (turningLeft) {
      digitalWrite(2, HIGH);
    }    
  } else {
    digitalWrite(9, LOW);
    digitalWrite(2, LOW);
  }

  squareX += tickMs; //Progress the square wave
  if (squareX >= squareMs) { //If at end of square wave's domain
    blinkerOn = !blinkerOn; //Invert state of blinker
    squareX = 0; //And loop back to beginning of wave
  }
  
  delay(tickMs);
}

