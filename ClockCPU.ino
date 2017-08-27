#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GPS.h>
#include "Adafruit_LEDBackpack.h"
Adafruit_7segment matrix = Adafruit_7segment();

#define A B01110111 //Custom letter definitions for the 7 segment display;
#define B B01111111
#define C B00111001
#define D B01011110
#define E B01111001
#define F B01110001
#define G B01101111
#define H B01110110
#define I B00110000
#define J B00011110
#define L B00111000
#define N B00110111
#define M1 B00110011
#define M2 B00100111
#define O B00111111
#define P B01110011
#define Q B01100111
#define R B00110001
#define S B01101101
#define T B01111000
#define U B00111110
#define V B00111110
#define X B01110110
#define Y B01101110
#define Z B01011011
#define COLON B00000010
#define TLDOT B00000100
#define BLDOT B00001000
#define DEGREE B00010000
#define BLANK B00000000

const int VBatPin = A3;
const int OptionButton = A2;
const int SyncButton = A1;
const int VSensePin = A0;
const int V3SHDN = 2;
const int Hz32768 = 3;
const int Hz128 = 4;
const int ESPEnable = 1;
const int Hz2 = 10;
const int LEDA = 5;
const int LEDB = 6;
const int LEDC = 7;
const int LEDD = 8;
const int LEDE = 9;

int Time;
int PM = 0;
int Hours = 1;
int Minutes = 0;
int Seconds = 0;
int SecondsState;
int LastSecondsState;
int SyncHours = 0;
int SyncHours2 = 0;
int SyncMinutes = 0;
int SyncSeconds = 0;
int SyncCalibration = 0;

int VBat;
int VBatA;
int VBatB;
int VBatC;

float VDec = /*.004887*/1;

int i = 0;
int j = 0;
int k = 0;
int l = 0;
int m = 0;
int n = 0;
int o = 0;
int p = 0;

String x = "";
String y = "";
String z = "";
String SyncTime = "";

int OptionState = HIGH;         
int OptionButtonState;             
int LastOptionButtonState = HIGH;
int OptionRead;
int Option = 0;
int SyncState = HIGH;         
int SyncButtonState;             
int LastSyncButtonState = HIGH;
int SyncRead;
int Sync = 0;
int SyncFail = 0;
int ACharge1 = 0;
int ACharge2 = 0;
int ACharge3 = 0;
int AChargeI = 0;
int VDelta = 0;
int VDelta2 = 0;

float VRatio = 1; //Battery measurements are >5V and are taken through a voltage divider
float VRatioSense = 1;
float VBat2 = 0;
float VSense = 0;
float ACharge = 0;
float RSense = .5;
   
unsigned long LastOptionDebounceTime = 0;
unsigned long LastSyncDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long SyncCount = 0;
unsigned long SyncTimer = 20000;
unsigned long ESPDelay = 2000;
unsigned long FailCount = 0;
unsigned long OptionCount = 0;
unsigned long ChargeCount = 0;

void setup() {
  
  pinMode(OptionButton, INPUT);
  pinMode(SyncButton, INPUT);
  pinMode(Hz32768, INPUT);
  pinMode(LEDA, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDC, OUTPUT);
  pinMode(LEDD, OUTPUT);
  pinMode(LEDE, OUTPUT);
  pinMode(ESPEnable, OUTPUT);
  pinMode(Hz2, INPUT);
  pinMode(V3SHDN, OUTPUT);
  pinMode(VBatPin, INPUT);
  pinMode(VSensePin, INPUT);
  
  matrix.begin(0x70);

  Serial.begin(9600); 
    
}

void loop() {

  //if (Serial.available() > 0) {    
   //   SyncTime = Serial.readString();
 // }
      
  LastOptionButtonState = OptionRead;
  OptionRead = digitalRead(OptionButton);
  if (OptionRead != LastOptionButtonState) {
     LastOptionDebounceTime = millis();
  }
  if ((millis() - LastOptionDebounceTime) > debounceDelay) {
    if (OptionRead != OptionButtonState) {
      OptionButtonState = OptionRead;
      if (OptionButtonState == LOW) {
        OptionState = !OptionState;
        Option++;
        if(Option > 4){
          Option = 0;
        }
      }
    }
  }
  if((VDelta > 0) && (Option == 2)){
  digitalWrite(LEDA, HIGH);
}else{
  digitalWrite(LEDA, LOW);
}
if((VDelta > 3) && (Option == 2)){
  digitalWrite(LEDB, HIGH);
}else{
  digitalWrite(LEDB, LOW);
}
if((VDelta > 6) && (Option == 2)){
  digitalWrite(LEDC, HIGH);
}else{
  digitalWrite(LEDC, LOW);
}
if((VDelta > 9) && (Option == 2)){
  digitalWrite(LEDD, HIGH);
}else{
  digitalWrite(LEDD, LOW);
}
if((VDelta > 12 ) && (Option == 2)){
  digitalWrite(LEDE, HIGH);
}else{
  digitalWrite(LEDE, LOW);
} 
  if(Option == 1){
    if(k == 0){
    OptionCount = millis();
    k++;
    }
    if((millis() - OptionCount) > 15000){
      Option = 0;
      k = 0;
    }
    if(((millis() - OptionCount) < 9500) && (Option == 1)){
      matrix.writeDigitRaw(0,B);
      matrix.writeDigitRaw(1,A);
      matrix.writeDigitRaw(2,BLANK);
      matrix.writeDigitRaw(3,T);
      matrix.writeDigitRaw(4,T);
      matrix.writeDisplay();

      analogRead(VBatPin); //ADC Primer
      VBat = 100*VDec*VRatio*analogRead(VBatPin)+.01;
      VBat2 = analogRead(VBatPin);
      VBatA = VBat/100;
      VBatB = VBat%10;
      VBatC = (VBat*10-(VBatA*1000+VBatB*100));
      //Serial.print("VBAT2: ");
      //Serial.print(VBat2);
      //Serial.print("\n");
      //Serial.print("VBatA: ");
      //Serial.print(VBatA);
     // Serial.print("\n");
      //Serial.print("VBatC: ");
      //Serial.print(VBatC);
      //Serial.print("\n");
 
    }
    /*if(((millis() - OptionCount) > 2500) && (Option == 1)){
      matrix.writeDigitRaw(0,BLANK);
      matrix.writeDigitNum(1,VBatA);
      matrix.writeDigitNum(3,VBatB);
      matrix.writeDigitRaw(2,COLON);
      matrix.writeDigitNum(4,BLANK);
      matrix.writeDisplay();
    }*/
  }
 
  if(Option == 2){
    if(k == 1){
      OptionCount = millis();
      k++;

    }
    if(((millis() - OptionCount) > 5000) && (Sync == 0)){
      Option = 0;
      k = 0;
    }
    if(((millis() - OptionCount) > 40000) && (Sync == 1)){
      Option = 0;
      k = 0;
      Sync = 0;
    }
    if(((millis() - OptionCount) < 250) && (Option == 2)){
      matrix.writeDigitRaw(0,S);
      matrix.writeDigitRaw(1,O);
      matrix.writeDigitRaw(3,L);
      matrix.writeDigitRaw(4,A);
      matrix.writeDisplay();
    }
        if(((millis() - OptionCount) > 500) && ((millis() - OptionCount) < 750) && (Option == 2) && (Sync == 0)){
      matrix.writeDigitRaw(0,O);
      matrix.writeDigitRaw(1,L);
      matrix.writeDigitRaw(3,A);
      matrix.writeDigitRaw(4,R);
      matrix.writeDisplay();
    }
        if(((millis() - OptionCount) > 750) && ((millis() - OptionCount) < 1000) && (Option == 2) && (Sync == 0)){
      matrix.writeDigitRaw(0,L);
      matrix.writeDigitRaw(1,A);
      matrix.writeDigitRaw(3,R);
      matrix.writeDigitRaw(4,BLANK);
      matrix.writeDisplay();
    }
       
        if(((millis() - OptionCount) > 1000) && ((millis() - OptionCount) < 1250) && (Option == 2) && (Sync == 0)){
      matrix.writeDigitRaw(0,A);
      matrix.writeDigitRaw(1,R);
      matrix.writeDigitRaw(3,BLANK);
      matrix.writeDigitRaw(4,C);
      matrix.writeDisplay();
    }
       
        if(((millis() - OptionCount) > 1250) && ((millis() - OptionCount) < 1500) && (Option == 2) && (Sync == 0)){
      matrix.writeDigitRaw(0,R);
      matrix.writeDigitRaw(1,BLANK);
      matrix.writeDigitRaw(3,C);
      matrix.writeDigitRaw(4,U);
      matrix.writeDisplay();
    }
       
        if(((millis() - OptionCount) > 1500) && ((millis() - OptionCount) < 1750) && (Option == 2) && (Sync == 0)){
      matrix.writeDigitRaw(0,BLANK);
      matrix.writeDigitRaw(1,C);
      matrix.writeDigitRaw(3,U);
      matrix.writeDigitRaw(4,R);
      matrix.writeDisplay();
    }
       
        if(((millis() - OptionCount) > 1750) && ((millis() - OptionCount) < 2000) && (Option == 2) && (Sync == 0)){
      matrix.writeDigitRaw(0,C);
      matrix.writeDigitRaw(1,U);
      matrix.writeDigitRaw(3,R);
      matrix.writeDigitRaw(4,R);
      matrix.writeDisplay();
    }
       
        if(((millis() - OptionCount) > 2250) && ((millis() - OptionCount) < 2500) && (Option == 2) && (Sync == 0)){
      matrix.writeDigitRaw(0,U);
      matrix.writeDigitRaw(1,R);
      matrix.writeDigitRaw(3,R);
      matrix.writeDigitRaw(4,E);
      matrix.writeDisplay();
    }
        if(((millis() - OptionCount) > 2500) && ((millis() - OptionCount) < 2750) && (Option == 2) && (Sync == 0)){
      matrix.writeDigitRaw(0,R);
      matrix.writeDigitRaw(1,R);
      matrix.writeDigitRaw(3,E);
      matrix.writeDigitRaw(4,N);
      matrix.writeDisplay();
    }
       
        if(((millis() - OptionCount) > 2750) && ((millis() - OptionCount) < 3000) && (Option == 2) && (Sync == 0)){
      matrix.writeDigitRaw(0,R);
      matrix.writeDigitRaw(1,E);
      matrix.writeDigitRaw(3,N);
      matrix.writeDigitRaw(4,T);
      matrix.writeDisplay();
    }
       
    if((Sync == 1) && (Option == 2)){

      if(p == 0){
      ChargeCount = millis();
     
      analogRead(VSensePin);
      VSense = VDec*VRatioSense*analogRead(VSensePin);
      analogRead(VBatPin);
      VBat2 = VDec*VRatio*analogRead(VBatPin);
      analogRead(VSensePin);
      VSense = VDec*VRatioSense*analogRead(VSensePin)+VSense;
      analogRead(VBatPin);
      VBat2 = VDec*VRatio*analogRead(VBatPin)+VBat2;
            analogRead(VSensePin);
      VSense = VDec*VRatioSense*analogRead(VSensePin)+VSense;
      analogRead(VBatPin);
      VBat2 = VDec*VRatio*analogRead(VBatPin)+VBat2;
            analogRead(VSensePin);
      VSense = VDec*VRatioSense*analogRead(VSensePin)+VSense;
      analogRead(VBatPin);
      VBat2 = VDec*VRatio*analogRead(VBatPin)+VBat2;
            analogRead(VSensePin);
      VSense = VDec*VRatioSense*analogRead(VSensePin)+VSense;
      analogRead(VBatPin);
      VBat2 = VDec*VRatio*analogRead(VBatPin)+VBat2;
            analogRead(VSensePin);
      VSense = VDec*VRatioSense*analogRead(VSensePin)+VSense;
      analogRead(VBatPin);
      VBat2 = VDec*VRatio*analogRead(VBatPin)+VBat2;
                  analogRead(VSensePin);
      VSense = VDec*VRatioSense*analogRead(VSensePin)+VSense;
      analogRead(VBatPin);
      VBat2 = VDec*VRatio*analogRead(VBatPin)+VBat2;
                  analogRead(VSensePin);
      VSense = VDec*VRatioSense*analogRead(VSensePin)+VSense;
      analogRead(VBatPin);
      VBat2 = VDec*VRatio*analogRead(VBatPin)+VBat2;
                  analogRead(VSensePin);
      VSense = VDec*VRatioSense*analogRead(VSensePin)+VSense;
      analogRead(VBatPin);
      VBat2 = VDec*VRatio*analogRead(VBatPin)+VBat2;
                  analogRead(VSensePin);
      VSense = VDec*VRatioSense*analogRead(VSensePin)+VSense;
      analogRead(VBatPin);
      VBat2 = VDec*VRatio*analogRead(VBatPin)+VBat2;

      
      
      VDelta = (VSense-VBat2)*(.1);
      if(VDelta < 0){
        VDelta = 0;
      }
      ACharge = (VDelta)/RSense;

      VDelta2 = VDelta*.4375;
      AChargeI = ACharge*1000;
      ACharge1 = AChargeI/100;
      ACharge2 = (AChargeI%(ACharge1*100))/10;
      ACharge3 = ((AChargeI%(ACharge1*100)))%ACharge2;
       p++;
      }
      if((millis() - ChargeCount) > 250){
        Serial.print("VS: ");
        Serial.print(VSense);
        Serial.print("\n");
        Serial.print("VB: ");
        Serial.print(VBat2);
        Serial.print("\n");
        Serial.print(VDelta2);
        Serial.print("\n");
        //Serial.print(ACharge3);
        //Serial.print("\n");*/

      //matrix.writeDigitNum(0,ACharge1);
      matrix.writeDigitNum(0,0);
      //matrix.writeDigitNum(1,ACharge2);
      matrix.writeDigitNum(1,VDelta2);
      //matrix.writeDigitNum(3,ACharge3);
      matrix.writeDigitNum(3,0);
      matrix.writeDigitRaw(4,A);
      matrix.writeDigitRaw(2,BLDOT);
      matrix.writeDisplay();
     
        p = 0;
      }
      
    }
  }
  if(Option == 3){
    if(k == 2){
      OptionCount = millis();
      k++;
    }
    if(((millis() - OptionCount) < 2500) && (Option == 3) && (Sync == 0)){
      if(m == 0){
      matrix.writeDigitRaw(0,H);
      matrix.writeDigitRaw(1,O);
      matrix.writeDigitRaw(3,U);
      matrix.writeDigitRaw(4,R);
      matrix.writeDisplay();
      m++;
      }
      if(l == 1){            
      matrix.print(Hours, DEC);
      matrix.writeDisplay();
      l = 0;
      }
    }
    if(((millis() - OptionCount) < 2500) && (Option == 3) && (Sync == 1)){
      if(l == 0){
      OptionCount = millis();
      l++;
      Sync = 0;
      Hours++;
      }
    }
    if(((millis() - OptionCount) > 5000) && (Sync == 0)){
      Option = 0;
      k = 0;
      l = 0;
      m = 0;
    }
  }
    if(Option == 4){
    if(k == 3){
      OptionCount = millis();
      k++;
    }
    if(((millis() - OptionCount) < 5000) && (Option == 4) && (Sync == 0)){
      if(n == 0){
      matrix.writeDigitRaw(0,M1);
      matrix.writeDigitRaw(1,M2);
      matrix.writeDigitRaw(3,I);
      matrix.writeDigitRaw(4,N);
      matrix.writeDisplay();
      n++;
      }
      if(o == 1){            
      matrix.print(Minutes, DEC);
      matrix.writeDisplay();
      o = 0;
      }
    }
    if(((millis() - OptionCount) < 5000) && (Option == 4) && (Sync == 1)){
      if(o == 0){
      OptionCount = millis();
      o++;
      Sync = 0;
      Minutes++;
      }
    }
    if(((millis() - OptionCount) > 5000) && (Sync == 0)){
      Option = 0;
      k = 0;
      n = 0;
      o = 0;
    }
  }
  LastSyncButtonState = SyncRead;

    SyncRead = digitalRead(SyncButton);
  if (SyncRead != LastSyncButtonState) {
     LastSyncDebounceTime = millis();
  }
  if ((millis() - LastSyncDebounceTime) > debounceDelay) {
    if (SyncRead != SyncButtonState) {
      SyncButtonState = SyncRead;
      if (SyncButtonState == LOW) {
        SyncState = !SyncState;
        Sync++;
      }
    }
  }
  LastSyncButtonState = SyncRead;

  SecondsState = digitalRead(Hz2);
  if (SecondsState != LastSecondsState) {
    if (SecondsState == HIGH) {
        Seconds++;
        //Serial.print(Time);
        //Serial.print("\n");
    }
  }
  LastSecondsState = SecondsState;

  if(Seconds > 119){
    Seconds = 0;
    Minutes++;
  }
  if(Minutes > 59){
    Minutes = 0;
    Hours++;
  }
  if(Hours > 12){
    Seconds = 0;
    Minutes = 0;
    Hours = 1;
  }
  
  Time = Hours*100+Minutes;
  if((Sync == 0) && (Option == 0)){
    matrix.print(Time, DEC);
    matrix.writeDigitRaw(2,COLON);
    matrix.writeDisplay();
  }
  if((Sync == 1) && (Option == 0)){
    matrix.writeDigitRaw(0,S);
    matrix.writeDigitRaw(1,Y);
    matrix.writeDigitRaw(3,N);
    matrix.writeDigitRaw(2,BLANK);
    matrix.writeDigitRaw(4,C);
    matrix.writeDisplay(); 
    digitalWrite(V3SHDN, HIGH);
  }
  if((SyncFail == 1) && (Option == 0)){
    matrix.writeDigitRaw(0,F);
    matrix.writeDigitRaw(1,A);
    matrix.writeDigitRaw(3,I);
    matrix.writeDigitRaw(4,L);
    matrix.writeDisplay(); 
    digitalWrite(V3SHDN, LOW);
  }
 
  if(((millis() - FailCount) > 3000) && (j == 2)){
    SyncFail = 0;
    j = 0;
    Sync = 0;
    //Back to normal.
  }
  

  if((Sync == 1) && (Option == 0)){ //If the sync button his pressed;
    if(j == 0){
    SyncCount = millis();
    //Serial.flush();
    j++;
    }
   

    if(((millis() - SyncCount) > SyncTimer) && (i == 0) && (j == 1)){
      SyncFail = 1;
      Sync = 2;
      FailCount = millis();
      j = 2;
    }
    matrix.writeDigitRaw(0,S);
    matrix.writeDigitRaw(1,Y);
    matrix.writeDigitRaw(3,N);
    matrix.writeDigitRaw(2,BLANK); 
    matrix.writeDigitRaw(4,C);
    matrix.writeDisplay(); 
  
    if(((millis() - SyncCount) > ESPDelay) && (i == 0) && (j == 1)) {
      digitalWrite(ESPEnable, HIGH);
    }else{
      digitalWrite(ESPEnable, LOW);
    }
    
    while (Serial.available() > 0) {
     
      SyncTime = Serial.readString();
      //Serial.print(SyncTime);
      if((SyncTime.length() > 45)){
      i++;
      Sync = 0;
      }else{
        Serial.flush();
      }
    }   
  }
  if(i > 0){
    x = SyncTime.substring(16, 18);
    y = SyncTime.substring(19, 21);
    z = SyncTime.substring(22, 24);
    SyncHours2 = x.toInt()-4;
    SyncMinutes = y.toInt();
    SyncSeconds = z.toInt()+SyncCalibration;
     if(SyncHours2 < 0){
      SyncHours2 = 12+SyncHours2;
     }
    if(SyncHours > 12){
      SyncHours = SyncHours2-12;
      PM = 1;
    }
    if(SyncHours2 < 12){
      PM = 0;
    }
    if((SyncHours2 < 13) && (SyncHours2 > 0)){
      SyncHours = SyncHours2;
    }
    
    Hours = SyncHours;
    if(Hours == 0){
      Hours = 1;
    }
    Minutes = SyncMinutes;
    Seconds = SyncSeconds;
    Sync = 0;
    i = 0;
    
  }
}

