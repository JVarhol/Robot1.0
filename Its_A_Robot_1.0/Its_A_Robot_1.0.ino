/* So I wrote this to test a bunch of components of the robot with the button pad, because I dont have my controlled Yet
 * The button pad is super amazing 3 wires, 16 buttons. Im really happy with it so far
 * cant wait to get the 12 Channel RC Remote
 * The motor controllers are IBT-2 controllers, they are pretty easy to use Only really require 4 wires. For somereason GND and VCC arent Required??
 * Not 100% sure, but they work without GND and VCC, even with the arduino and motors on different power sources
 * I started setting some stuff up to use with the remote but Im not 100% sure how I am going to integrate it so I will get there when it comes in
 * I am a coding noob, so I probably did some stuff wrong, but if its wrong and works is it wrong?
 * the A Modes are for arduinos on i2c which I intended to use for driving the accessories but the more I think about it the more I hate that Idea
 * I mean do I really need a mega and 2 arduino nanos? Probably not :P SO Ill probably just use the mega outputs
 * Ill still need the A modes because of how accessories will work (#HotSwap :D)
 * This initial comment thing is getting really long isnt it?
 * Also, Fair warning I really suck at commenting, So, if you don't understand something, message me or file a complaint or something, just do the thing
 */

int ch1, ch2, ch3, ch4, ch5, ch6, ch7, ch8, ch9, ch10, ch11, ch12; //Define Integers for RC Remote
int buttonPad = 0;  // variable to store the value coming from the buttonPad
int button; //I Mean, its a button. Its pretty Self Explanatory
int sensorPin = A0; //This is actually the button Pad Input
int a1Mode; //Mode or DeviceID of Acc port 1
int a2Mode; //Mode or DeviceID of acc port 2
int bWait = 200; //Time to wait after button press
int hLightState = 0;
int RPWM_OutputR = 5; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_OutputR = 6; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)
int RPWM_OutputL = 10; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_OutputL = 11; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)
int mSpeedR;  //Speed of RMotor
int mSpeedL;  //Speed of LMotor
int mSpeedRC; //Speed of R Motor for Reporting on Status Check
int mSpeedLC; //Speed of L Motor for Reporting on Status Check
int headLights = 13; //set PWM Output for headlights

void setup() {

  Serial.begin(9600); // I like Serial, It tastes good :D
}

void loop() {

  buttonSet(); //Check Button Pad to see if theres input, Can we talk about how amazing buttons are?
  if (button == 9) {
    setA1();
  }
  if (button == 13) {
    setA2();
  }
  if (button == 16) {
    statusCheck();
  }
  if (button == 4) {
    motor1();
  }
  if (button == 8) {
    motor2();
  }
  if (button == 12) {
    hLights();
  }
}


void setA1() {   //Set Accessory #1 Function
  Serial.println("Please Select A1 Mode");
  button = 0;
  while (button == 0) {
    buttonSet();
    a1Mode = button;
  }
  Serial.print("a1Mode: ");
  Serial.println(a1Mode);
  delay(bWait);
  return;
}
void setA2() {  //Set Accessory #2 Function
  Serial.println("Please Select A2 Mode");
  button = 0;
  while (button == 0) {
    buttonSet();
    a2Mode = button;
  }
  Serial.print("a2Mode: ");
  Serial.println(a2Mode);

  delay(bWait);
  return;
}
void statusCheck() {   //Just a simple function that reports the Status of everything SUPER HANDY
  Serial.println("DEVICE STATUS CHECK");
  Serial.print("A1 Mode: ");
  Serial.println(a1Mode);
  Serial.print("A2 Mode: ");
  Serial.println(a2Mode);
  Serial.print("Headlights: ");
  Serial.println(hLightState);
  Serial.print("Right Motor Speed: ");
  if (mSpeedR < 512 && mSpeedR > 0) {
    mSpeedRC = -(map(mSpeedR, 0, 511, 511, 0)); // Honestly, I like the map command, its so nice and easy and simple
  } else {
    mSpeedRC = map(mSpeedR, 512, 1023, 0, 511);
  }
  if (mSpeedR == 0) {
    mSpeedRC = mSpeedR;
  }
  Serial.println(mSpeedRC);

  Serial.print("Left Motor Speed: ");
  if (mSpeedL < 512 && mSpeedL > 0) {
    mSpeedLC = -(map(mSpeedL, 0, 511, 511, 0));
  } else {
    mSpeedLC = map(mSpeedL, 512, 1023, 0, 511);
  }
  if (mSpeedL == 0) {
    mSpeedLC = mSpeedL;
  }
  Serial.println(mSpeedLC);
  button = 0;
  return;
}

void motor1()  //Controls the Right Motor
{
  delay(bWait);
  Serial.println("Select Right Motor Speed");  //Do I really need to explain what this does?
  button = 0;
  while (button == 0) {
    buttonSet(); //Check Buttonpad to see if theres input
    if (button > 0) {
      mSpeedR = map(button, 1, 16, 0, 1023);
    }
    // sensor value is in the range 0 to 1023
    // the lower half of it we use for reverse rotation; the upper half for forward rotation
    if (mSpeedR < 512)
    {
      // reverse rotation
      int reversePWM = -(mSpeedR) / 2;
      analogWrite(LPWM_OutputR, 0);
      analogWrite(RPWM_OutputR, reversePWM);
    }
    else
    {
      // forward rotation
      int forwardPWM = (mSpeedR) / 2;
      analogWrite(LPWM_OutputR, forwardPWM);
      analogWrite(RPWM_OutputR, 0);
    }
  }
  Serial.println("SET");
  delay(bWait);
  button = 0;
  return;
}
void motor2()  //Controls the Left Motor

{
  delay(bWait);
  Serial.println("Select Left Motor Speed");
  button = 0;
  while (button == 0) {
    buttonSet(); //Check Buttonpad to see if theres input
    if (button > 0) {
      mSpeedL = map(button, 1, 16, 0, 1023);
    }
    // sensor value is in the range 0 to 1023
    // the lower half of it we use for reverse rotation; the upper half for forward rotation
    if (mSpeedL < 512)
    {
      // reverse rotation
      int reversePWM = -(mSpeedL) / 2;
      analogWrite(LPWM_OutputL, 0);
      analogWrite(RPWM_OutputL, reversePWM);
    }
    else
    {
      // forward rotation
      int forwardPWM = (mSpeedL) / 2;
      analogWrite(LPWM_OutputL, forwardPWM);
      analogWrite(RPWM_OutputL, 0);
    }
  }
  Serial.println("SET");

  delay(bWait);
  button = 0;
  return;
}
void hLights() { 
  if (hLightState < 1) // Just makes it so I can do On/Off with 1 Button, Neat huh?
  hLightState = 1000; //Sets Brightness to 100%
  Serial.println("Lights: On");  //Honestly My serial prints are basically my comments, I mean just read those :D
}
else {
  hLightState = 0;
    Serial.println("Lights: Off");
}
analogWrite(headLights, hLightState);
button = 0;
delay(bWait);
return;
}
void buttonSet() {  //Analog pad cant easily be mapped from 0,1023 to 1,16 so I had to make this..... Yea......
// On a side note I got this 16 button pad, works on 3 wires, its super amazing, especially for prototyping and testing
// Yea, I have conversations in my code, deal with it B)   (Also I use Emojis :D)
  buttonPad = analogRead(sensorPin);
  while (buttonPad > 100) {
    if (buttonPad > 230 && buttonPad < 246) {
      button = 16;
    }
    else if (buttonPad > 270 && buttonPad < 284) {
      button = 15;
    }
    else if (buttonPad > 320 && buttonPad < 340) {
      button = 14;
    }
    else if (buttonPad > 400 && buttonPad < 420) {
      button = 13;
    }
    else if (buttonPad > 435 && buttonPad < 445) {
      button = 12;
    }
    else if (buttonPad > 455 && buttonPad < 465) {
      button = 11;
    }
    else if (buttonPad > 477 && buttonPad < 487) {
      button = 10;
    }
    else if (buttonPad > 502 && buttonPad < 522) {
      button = 9;
    }
    else if (buttonPad > 560 && buttonPad < 580) {
      button = 8;
    }
    else if (buttonPad > 590 && buttonPad < 610) {
      button = 7;
    }
    else if (buttonPad > 630 && buttonPad < 650) {
      button = 6;
    }
    else if (buttonPad > 670 && buttonPad < 680) {
      button = 5;
    }
    else if (buttonPad > 770 && buttonPad < 790) {
      button = 4;
    }
    else if (buttonPad > 840 && buttonPad < 860) {
      button = 3;
    }
    else if (buttonPad > 920 && buttonPad < 940) {
      button = 2;
    }
    else if (buttonPad > 1013 && buttonPad < 1084) {
      button = 1;
    }
    delay(bWait);
    return;
  }
}
