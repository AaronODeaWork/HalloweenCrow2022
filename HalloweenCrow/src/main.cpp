#include <Arduino.h>
#include <Servo.h>
#include <LowPower.h>

//----------------------EYE LEDs------------------------
//blink timer
unsigned long previousMillis = 0U;

//Interval between blinks
unsigned int intervalMin = 3000U;
unsigned int intervalMax = 6000U;

unsigned int interval = 0U;
unsigned int BlinkTime = 100U; // time it takes to blink
//-----------------------------------------------------

//----------------------Head Servo------------------------
Servo myservo;

//does not go above 360
unsigned short MaxAngle = 110;
unsigned short MinAngle = 60U;

//random position of the servo
unsigned short RandomPosition = 0;
unsigned short CurentPosition = 0;

//Servo timer
unsigned long previousMillisServo = 0U;

//update timer between servo updates
unsigned int intervalServo = 20U;
unsigned int intervalServoWait = 0;
unsigned int intervalServoWaitMax = 12000U;
unsigned int intervalServoWaitMin = 6000U;

bool HeadWait = false; 

bool Running = true; 

//-----------------------------------------------------

//----------------------Pins------------------------
int ServoPin = 9;
int LeftLedPin = 13;
int RightLedPin = 12;

//Function for the leds blinking 
void EyeBlinking()
{  
  //Timer for when to blink
  if(millis() - previousMillis > interval) {

    digitalWrite(LeftLedPin, LOW);
    digitalWrite(RightLedPin, LOW);

  }
  //Timer for a single blink
  if(millis() - previousMillis > (interval + BlinkTime)) {
      previousMillis = millis();

      digitalWrite(LeftLedPin, HIGH);
      digitalWrite(RightLedPin, HIGH);

      interval = random(intervalMin , intervalMax);

  }
}
//Function for the leds blinking 
void HeadTilt()
{

  //check if crow should wait to turn head
  if(!HeadWait)
  {
    //timer for update on position(speed of movement)
    if((millis() - previousMillisServo) > intervalServo)
    {
      previousMillisServo = millis();
      
      if ( RandomPosition >= CurentPosition)
      {
        CurentPosition++;
        myservo.write(CurentPosition);
      }
      else if ( RandomPosition <= CurentPosition)
      {
        CurentPosition--;
        myservo.write(CurentPosition);
      }

      if(RandomPosition == CurentPosition)
      {
        HeadWait = true;
      }
    }
  
  }
  else
  {
    // interval for waiting to move
    if((millis() - previousMillisServo) > intervalServoWait)
      {
        HeadWait = false;
        intervalServoWait = random(intervalServoWaitMin, intervalServoWaitMax);
        RandomPosition = random(MinAngle, MaxAngle);
        
      }
  }

}

void setup() {

  //Servo set up
  myservo.attach(ServoPin);

  //Pin Set up
  pinMode(LeftLedPin, OUTPUT);
  pinMode(RightLedPin, OUTPUT);

  //Set Leds On
  digitalWrite(LeftLedPin, LOW);
  digitalWrite(RightLedPin, LOW);

  //Set random starting Position
  RandomPosition = random(MinAngle, MaxAngle);
  CurentPosition = 0;

  myservo.write(90);

  //Set random starting blink
  interval = random(intervalMin , intervalMax);

  //set up wait
  intervalServoWait = random(intervalServoWaitMin, intervalServoWaitMax);

}

void loop() {

  if(Running)
  {
    HeadTilt();
    EyeBlinking();
  }
  

}

void Detach()
{
  myservo.detach();
}