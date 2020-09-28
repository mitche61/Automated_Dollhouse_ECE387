/*
Finite State Machine
*/

///////////////////////////////States
static unsigned long state;
const int SAFETY = 0;
const int START = 1;
const int YN = 2;
const int DISPENSE = 3;
const int FINISH = 4;
const int error = 9;

/////////////////////////////////Drinks
int choice;
const int the_earlybird = 1;
const int the_cleanser = 2;
const int the_lemonhead = 3;
const int the_patriot = 4;
const int the_hipster = 5;
const int the_ran_zhang = 6;

String demonstrate;
const String theearlybird = "The Earlybird";
const String thecleanser = "The Cleanser";
const String thelemonhead = "The Lemonhead";
const String thepatriot = "The Patriot";
const String thehipster = "The Hipster";
const String theranzhang = "The Ran Zhang";

///////////////////////////////////Parts
const int onep = 1000; //1 second
const int twop = 2000; //2 second
const int threep = 3000; //3 second
const int little = 500; //half a second
const int del = 1000; //1 second

///////////////////////////////////Solenoids
const int water = 13;
const int red = 12;
const int yellow = 11;
const int blue = 10;
const int sparkling = 9;

///////////////////////////////////Photoresistor
const int pr = A0;
int prValue;
const int threshold = 200;

///////////////////////////////////LED
const int LED = 8;

//////////////////////////////////Flag
boolean flag;

///////////////////////////////////LCD
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, A1, A2, A3, A4, A5);

//////////////////////////////////Keypad
#include <Keypad.h>

const byte ROWS = 3; 
const byte COLS = 2; 

char hexaKeys[ROWS][COLS] = {
  {'1', '4'},
  {'2', '5'},
  {'3', '6'},
};

byte rowPins[ROWS] = {6, 5, 4}; 
byte colPins[COLS] = {3, 2,}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

//////////////////////////////setup
void setup() {
  state = SAFETY;
  flag = LOW;
  
  pinMode(water, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(sparkling, OUTPUT);
  
  pinMode(LED, OUTPUT);
  
  lcd.begin(16, 2);
  Serial.begin(9600);
}

//////////////////////////////loop
void loop() {
  //read and print photoresistor
  prValue=analogRead(pr);
  Serial.println(prValue);
  
  //read and print keypad value
  char customKey = customKeypad.getKey();
  Serial.println(customKey);
  
    switch (state) {      
    ////////////////////////Safety 
    case SAFETY:
      if (prValue < threshold) {
        state = START;
      }
      else {
        state = SAFETY;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Place your cup");
      lcd.setCursor(0,1);
      lcd.print("on the cart."); 
      delay(100);
      }
      break;
      
    /////////////////////////Start  
    case START:
      if (customKey) {
        switch (customKey) {
          case '1': 
          choice = the_earlybird;
          demonstrate = theearlybird;
          state = YN; 
          break;
          
          case '2': 
          choice = the_cleanser;
          demonstrate = theearlybird;
          state = YN; 
          break;
          
          case '3':
          choice = the_lemonhead;
          demonstrate = thelemonhead;
          state = YN;
          break;
          
          case '4':
          choice = the_patriot;
          demonstrate = thepatriot;
          state = YN;
          break;
          
          case '5':
          choice = the_hipster;
          demonstrate = thehipster;
          state = YN;
          break;
          
          case '6':
          choice = the_ran_zhang;
          demonstrate = theranzhang;
          state = YN;
          break; 
        }
      }      
      else {
      state = START;      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Choose drink");
      lcd.setCursor(0,1);
      lcd.print("from the keypad."); 
      delay(100);
      }
      break;
     
     /////////////////////Confirm or deny 
    case YN:
      if (customKey) {
        switch (customKey) {
          case '1' : state = DISPENSE; break;
          case '2' : state = START; break;
          default : state = YN; break;         
        }
      }
      else {
        state = YN;        
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(demonstrate);
        lcd.setCursor(0,1);
        lcd.print("Yes:1  Go back:2"); 
        delay(100);
      }
        break;   
  
    /////////////////////Dispense    
    case DISPENSE:
        if (flag == HIGH) {
          state = FINISH;
        }
        else {
          state = DISPENSE;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Dispening");
        lcd.setCursor(0,1);
        lcd.print(":)"); 
        delay(100);
        
        dispenser(choice);
        }
    break;

    case FINISH:
      if () {
        if (customkey) {
          state = START;
        }
      }

      else {
        state = FINISH
        
        LED = HIGH;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Your drink is done!");
        lcd.setCursor(0,1);
        lcd.print("Press any button for another."); 
        delay(100);
      }
  }
  
}

void dispenser(int choice) {
  switch (choice) {
    
    case the_earlybird:
      ///1 part red
      digitalWrite(red, HIGH);
      delay(onep);
      digitalWrite(red, LOW);
      delay(del);
      
      //1 part yellow
      digitalWrite(yellow, HIGH);
      delay(onep);
      digitalWrite(yellow, LOW);
      delay(del);
      
      //1 part sparkling
      digitalWrite(sparkling, HIGH);
      delay(onep);
      digitalWrite(sparkling, LOW);
      delay(del);

      flag = HIGH;
    break;
    
    case the_cleanser:
      //1 part blue
      digitalWrite(blue, HIGH);
      delay(onep);
      digitalWrite(blue, LOW);
      delay(del);
      
      //1 part yellow
      digitalWrite(yellow, HIGH);
      delay(onep);
      digitalWrite(yellow, LOW);
      delay(del);

      flag = HIGH;
    break;
    
    case the_lemonhead:
      //1 part sparkling
      digitalWrite(sparkling, HIGH);
      delay(onep);
      digitalWrite(sparkling, LOW);
      delay(del);
      
      //1 part lemonade
      digitalWrite(yellow, HIGH);
      delay(onep);
      digitalWrite(yellow, LOW);
      delay(del);

      flag = HIGH;
    break;
    
    case the_patriot:
      //1 part water
      digitalWrite(water, HIGH);
      delay(onep);
      digitalWrite(water, LOW);
      delay(del);
      
      //1 part blue
      digitalWrite(blue, HIGH);
      delay(onep);
      digitalWrite(blue, LOW);
      delay(del);
      
      //1 part red
      digitalWrite(red, HIGH);
      delay(onep);
      digitalWrite(red, LOW);
      delay(del);

      flag = HIGH;
    break;
    
    case the_hipster:
      //3 parts water
      digitalWrite(water, HIGH);
      delay(threep);
      digitalWrite(water, LOW);
      delay(del);

      flag = HIGH;
    break;
    
    case the_ran_zhang:
      //little bit of water
      digitalWrite(water, HIGH);
      delay(little);
      digitalWrite(water, LOW);
      delay(del);
      
      //little bit of red
      digitalWrite(red, HIGH);
      delay(little);
      digitalWrite(red, LOW);
      delay(del);
      
      //little bit of yellow
      digitalWrite(yellow, HIGH);
      delay(little);
      digitalWrite(yellow, LOW);
      delay(del);
      
      //little bit of blue
      digitalWrite(blue, HIGH);
      delay(little);
      digitalWrite(blue, LOW);
      delay(del);
      
      //little bit of sparkling
      digitalWrite(sparkling, HIGH);
      delay(little);
      digitalWrite(sparkling, LOW);
      delay(del);

      flag = HIGH;
    break;
  }
  }
