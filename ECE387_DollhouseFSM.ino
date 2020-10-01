/*
  Michaela Mitchell
  ECE 387 Individual Project
  Spring 2020 @ Home
  Finite State Machine for Elevator
*/

///////////////////////////////States
static unsigned long state;
static unsigned long prevState;
const int start = 1;
const int floor1 = 2;
const int floor2 = 3;
const int floor3 = 4;
const int haunted = 5;
const int alarm = 6;
const int error = 9;
const int nonProgrammedError = 10;

//////////////////////////////////LCD
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//////////////////////////////////IR Remote and Reciever
#include <IRremote.h>
const int irReceiverPin = 13;
IRrecv irrecv(irReceiverPin);
decode_results results;
int IR;

///////////////////////////////Stepper Motor
#include <Stepper.h>
const int stepsPerRevolution = 2048;
Stepper myStepper = Stepper(stepsPerRevolution, A2, A3, A4, A5);
const int dist = -2600; //number of steps to go distance of one floor
int distTracker;
int movement;

//////////////////////////////////Ultrasonic
const int TrigPin = A0;
const int EchoPin = 10;
const int thresh = 5;
float cm;

//////////////////////////////////Piezo
#include <TonePlayer.h>
TonePlayer tone1 (TCCR1A, TCCR1B, OCR1AH, OCR1AL, TCNT1H, TCNT1L);
const int PiezoPin = 9;
int note = 262;

////////////////////////////////LEDs
const int LED1 = 8;
const int LED2 = 7;
const int LED3 = 6;

void setup() {
  state = start;
  prevState = start;
  IR = 0;
  movement = 0;
  distTracker = 0;
  //////Ultrasonic
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  ////////Piezo
  pinMode(PiezoPin, OUTPUT);
  ////////LEDs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  /////////Stepper
  myStepper.setSpeed(5);
  ////////////Begin stuff
  Serial.begin(9600);
  lcd.begin(16, 2);
  irrecv.enableIRIn();
}

void loop() {
//  //Print State and Previous State
//  Serial.print("State: ");
//  Serial.println(state);
//  Serial.print("Previous State: ");
//  Serial.println(prevState);
  
  //read and print IR value, assign value to IR for state change
  if (irrecv.decode(&results))
  {
    Serial.print("IR_Code: ");
    Serial.println(results.value);
    irrecv.resume();
    if (results.value == 16753245) {
      Serial.println("Numeric value: 1");
      IR = 1;
    }
    else if (results.value == 16736925) {
      Serial.println("Numeric value: 2");
      IR = 2;
    }
    else if (results.value == 16769565) {
      Serial.println("Numeric value: 3");
      IR = 3;
    }
    else if (results.value == 16761405) {
      Serial.println("Numeric value: 6");
      IR = 6;
    }
    else if (results.value == 16750695) {
      Serial.println("Numeric value: 0");
      IR = 0;
    }
    else {
      Serial.println("Could not read data.");
      IR = 10;
    }
  }
  delay(100);

   /////////////////////////////////Ultrasonic Ranging
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  cm = pulseIn(EchoPin, HIGH) / 58.0; //The echo time is converted into cm
  cm = (int(cm * 100.0)) / 100.0; //Keep two decimal places
//  Serial.print("Distance\t=\t");
//  Serial.print(cm);
//  Serial.print("cm");
//  Serial.println();
  delay(10);

  if (abs(cm) > thresh) { //If alarm is not triggered

  switch (state) {
      ////////////////////////Start
      case start:
        prevState = start;
        if (IR == 0) {
          state = start;
          ////////////////LCD print out
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Welcome to");
          lcd.setCursor(0, 1);
          lcd.print("the Dollhouse!");
          delay(100);
          ///////////////LEDs
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, HIGH);
         }
        else if (IR == 1) {
          state = floor1;
          }
        else if (IR == 2) {
          myStepper.step(dist);
          state = floor2;
         }
        else if (IR == 3) {
          myStepper.step(dist*2);
          state = floor3;
        }
        else if (IR == 10) {
          state = error;
        }
        else {
          state = nonProgrammedError;
        }
    break;

/////////////////////////Floor 1
  case floor1:
     prevState = floor1;
    if (IR == 1) {
      state = floor1;
      ////////////////LCD print out
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Floor 1:");
      lcd.setCursor(0, 1);
      lcd.print("Kitchen & Patio");
      delay(100);
      ///////////////LEDs
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
    }
    else if (IR == 0) {
      state = start;
    }
    else if (IR == 2) {
      myStepper.step(dist);
      state = floor2;
    }
    else if (IR == 3) {
      myStepper.step(dist*2);
      state = floor3;
    }
    else if (IR == 10) {
      state = error;
    }
    else {
      state = nonProgrammedError;
    }
    break;

    /////////////////////Floor 2
  case floor2:
    prevState = floor2;
    if (IR == 1) {
      myStepper.step(-dist);
      state = floor1;
    }
    else if (IR == 0) {
      myStepper.step(-dist);
      state = start;
    }
    else if (IR == 2) {
      state = floor2;
      ////////////////LCD print out
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Floor 2:");
      lcd.setCursor(0, 1);
      lcd.print("Lounge Area");
      delay(100);
       ///////////////LEDs
      digitalWrite(LED2, HIGH);
      digitalWrite(LED1, LOW);
      digitalWrite(LED3, LOW);
    }
    else if (IR == 3) {
      myStepper.step(dist);
      state = floor3;
    }
    else if (IR == 10) {
      state = error;
    }
    else if (IR == 6) {
          state = haunted;
        }
    else {
      state = nonProgrammedError;
    }
    break;

    /////////////////////Floor 3
  case floor3:
  prevState = floor3;
    if (IR == 1) {
      myStepper.step(-dist*2);
      state = floor1;
    }
    else if (IR == 0) {
      myStepper.step(-dist*2);
      state = start;
    }
    else if (IR == 2) {
      myStepper.step(-dist);
      state = floor2;
    }
    else if (IR == 3) {
      state = floor3;
      ////////////////LCD print out
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Floor 3:");
      lcd.setCursor(0, 1);
      lcd.print("Bedroom");
      delay(100);
      ///////////////LEDs
      digitalWrite(LED3, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED1, LOW);
    }
    else if (IR == 10) {
      state = error;
    }
    else {
      state = nonProgrammedError;
    }
    break;

  /////////////////////Haunted House
  ////////////////////must start and end on floor 2
  case haunted:
    if (IR == 6) {
      state = haunted;
      ////////////////LCD print out
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("~Spooky Mode~");
      delay(100);
      /////////////////Elevator function
      SpookyElevator();
      ///////////////LEDs function
      SpookyLEDs();
      }
    else if (IR == 2 and prevState == floor2) {
      myStepper.step(-distTracker); //go back to floor2
      state = floor2;
    }
    else if (IR == 10) {
      state = error;
    }
    else {
      state = nonProgrammedError;
    }
    break;

    /////////////////////Error: This state requires the user to first enter the state they are in
    ////////////////////before being able to move onto a different state
  case error:
  ///////////////LEDs
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
    if (IR == 10) {
      state = error;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Could not ");
      lcd.setCursor(0, 1);
      lcd.print("read data.");
      delay(10);
    }
    else if (IR == 1) {
      state = floor1;
    }
    else if (IR == 2) {
      state = floor2;
    }
    else if (IR == 3) {
      state = floor3;
    }
//    else if (IR == 1) {
//      state = floor1;
////      if (prevState == floor1 or prevState == start) { 
////      state = ; }
////      else {
////      lcd.clear();
////      lcd.setCursor(0, 0);
////      lcd.print("Select the floor ");
////      lcd.setCursor(0, 1);
////      lcd.print("you are on.");
////      delay(100); }
//    }
//    else if (IR == 2) {
//      if(prevState == floor2) { 
//      state = floor2; }
//      else {
//      lcd.clear();
//      lcd.setCursor(0, 0);
//      lcd.print("Select the floor ");
//      lcd.setCursor(0, 1);
//      lcd.print("you are on.");
//      delay(100); }
//    }
//    else if (IR == 3) {
//      if(prevState == floor3) { 
//      state = floor3; }
//      else {
//      lcd.clear();
//      lcd.setCursor(0, 0);
//      lcd.print("Select the floor ");
//      lcd.setCursor(0, 1);
//      lcd.print("you are on.");
//      delay(100); }
//    }
    else {
      state = nonProgrammedError;
    }
    break;
  }
  }

  else {
    state = alarm;
  ////////////////LCD print out
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("INTRUDER");
  lcd.setCursor(0, 1);
  lcd.print("ALERT");
  delay(10);
     
  ///////////////LEDs flashing
    digitalWrite(LED1, HIGH);
    delay(500);
    digitalWrite(LED1,LOW);
    digitalWrite(LED2, HIGH);
    delay(500);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    delay(500);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
  ///////////////////Piezo Alarm Noise
  tone1.tone (220);  // 220 Hz
  delay (500);
  tone1.noTone ();

  tone1.tone (440);
  delay (500);
  tone1.noTone ();

  tone1.tone (880);
  delay (500);
  tone1.noTone ();

  // Go to the 3rd floor to check on the alarm system
  if (prevState == start or prevState == floor1) {
      myStepper.step(dist*2);
    }
    else if (prevState == floor2) {
      myStepper.step(dist);
    }
    else {
      //do nothing, already there
    }

  // Wait until the "threat" is gone
  if (cm < thresh) {
    state = floor3;
  }
}
}

///////Elevator moves up and down randomly within the paramters of 1/5
///////of the distance between floors
void SpookyElevator() {
  movement = random(-(dist/5),(dist/5));
  distTracker = distTracker+movement; ///track the total distance the spooky elevator moves
  myStepper.step(movement);
}

void SpookyLEDs() {
      digitalWrite(random(6,9), HIGH);
      delay(random(500,3000));
      digitalWrite(random(6,9), LOW);
      digitalWrite(random(6,9), HIGH);
      delay(random(500,3000));
      digitalWrite(random(6,9), LOW);
      digitalWrite(random(6,9), HIGH);
      delay(random(500,3000));
      digitalWrite(random(6,9),LOW);
}
