/*
 Controlling a servo position using the buttons on an keypad, 
 and displaying values on an LCD display
 created on 2 Nov 2022
 by Rob Siddall

 References:
 http://www.arduino.cc/en/Tutorial/Knob
 https://wiki.dfrobot.com/LCD_KeyPad_Shield_For_Arduino_SKU__DFR0009#target_3
*/

#include <LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);        // select the pins used on the LCD panel
Servo testservo;  // create servo object to control a servo

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int mode = 1;
int pwmValue = 1500;
const  int pwmIncrement = 50, pwmMax = 2000, pwmMin = 1000;
int sweepIncrement = pwmIncrement, sweepDelay = 50;
int holdIncrement = pwmIncrement;
int stepPWMValue = 1500;
int key = btnNONE, oldKey = btnNONE;
int adc_key_in;
bool updateDisplay = true;

void setup(){
    lcd.begin(16, 2);                       // start the library
    testservo.attach(2);  // attaches the servo on pin 9 to the servo object
    lcd.setCursor(0, 0);                   // set the LCD cursor   position
    lcd.print("Servo Demo Kit");
    lcd.setCursor(0, 1);                   // set the LCD cursor   position
    lcd.print("ENG2125 D.Skills");
    delay(500);
}

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int read_LCD_buttons(){
  adc_key_in = analogRead(A0);
  if (adc_key_in > 1000) key = btnNONE;
  else if (adc_key_in < 50)   key = btnRIGHT;
  else if (adc_key_in < 195)  key = btnUP;
  else if (adc_key_in < 380)  key = btnDOWN;
  else if (adc_key_in < 555)  key = btnLEFT;
  else if (adc_key_in < 790)  key = btnSELECT;
  if(key != oldKey){
    oldKey = key;
    updateDisplay = true;
    return key;
  }
  else return btnNONE;
}

void loop(){
int lcd_key;
lcd_key = read_LCD_buttons();
  if (lcd_key == btnSELECT) {
    mode++;
    pwmValue = 1500;
    sweepDelay  = 50;
    stepPWMValue = pwmValue;
    if (mode ==4) mode=1;
  }  
  switch (mode) {
    case 2:
      if(lcd_key == btnUP)   sweepDelay -=10;
      if(lcd_key == btnDOWN) sweepDelay +=10;
      sweepDelay  = constrain(sweepDelay,0,100);
      pwmValue += sweepIncrement;
      if(pwmValue>=pwmMax) sweepIncrement = -pwmIncrement;
      if(pwmValue<=pwmMin) sweepIncrement =  pwmIncrement;
      delay(sweepDelay);
      updateDisplay = true;
      break;
    case 1:
      if(lcd_key == btnUP)   pwmValue +=  holdIncrement;
      if(lcd_key == btnDOWN) pwmValue -= holdIncrement;      
      break;
    case 3:
      if(lcd_key == btnUP)   stepPWMValue +=  holdIncrement;
      if(lcd_key == btnDOWN) stepPWMValue -= holdIncrement;
      stepPWMValue = constrain(stepPWMValue,pwmMin,pwmMax);
      if(lcd_key == btnRIGHT)   pwmValue =  stepPWMValue;
      break;
    default:
      // nothing
      break;
  }
  if(updateDisplay){
  updateLCD();
  updateDisplay = false;
  delay(50);
  }
  pwmValue = constrain(pwmValue,pwmMin,pwmMax);
  testservo.writeMicroseconds(pwmValue);
}

void updateLCD(){
  lcd.clear();
  lcd.setCursor(0, 0);                   // set the LCD cursor   position
  switch (mode) {
    case 2:
      lcd.print("Sweep");
      lcd.setCursor(0, 1);                   // set the LCD cursor   position
      lcd.print("PWM=");
      lcd.print(pwmValue);
      lcd.print("us");
      break;
    case 1:
      lcd.print("Hold");
      lcd.setCursor(0, 1);                   // set the LCD cursor   position
      lcd.print("PWM=");
      lcd.print(pwmValue);
      lcd.print("us");         
      break;
    case 3:
      lcd.print("Step");
      lcd.setCursor(0, 1);                   // set the LCD cursor   position
      lcd.print("PWM=");
      lcd.print(stepPWMValue);
      lcd.print("us");
      break;
    default:
      // nothing
      break;
  }

}
