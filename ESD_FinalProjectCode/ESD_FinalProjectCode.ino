
#include <LiquidCrystal.h>
#include "pitches.h"
//initialise the library with the numbers of the interface pins 
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); 
 

  
void setup() { 
 
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);

  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT); 
  pinMode(13,OUTPUT); 
} 
  
void loop() { 
  phManager();
  delay(500);
  temperatureManager();
  delay(500);  
  waterLevelManager();
  delay(500); 
  oxyzenManager();   
  delay(500);  
}


void waterLevelManager(){
  // Print a message to the LCD.
  lcd.clear(); 
  lcd.print("WATER LEVEL: "); 
  
  int resval =0;  // holds the value
  int respin = A3; // sensor pin used

  // set the cursor to column 0, line 1 

  lcd.setCursor(0,1);  
  resval = analogRead(respin); //Read data from analog pin and store it to resval variable
   
  if (resval<=100){ 
    lcd.println("Empty");
    tone(8, NOTE_B3, 500);
    showWarningMessage("more than 300cm");
    digitalWrite(10,HIGH);
    } 
  else if (resval>100 && resval<=300){ 
    lcd.println("Low"); 
    tone(8, NOTE_B3, 500);
    showWarningMessage("more than 300cm");
    digitalWrite(10,HIGH);
    }
  else if (resval>300 && resval<=330){ 
    lcd.println("Medium");
    digitalWrite(10,LOW); 
    } 
  else if (resval>330){ 
    lcd.print("High");
    digitalWrite(10,LOW); 
    }  
  
}

void phManager(){
  lcd.clear();  
  float calibration_value = 21.34;
  int phval = 0; 
  unsigned long int avgval; 
  int buffer_arr[10],temp;

 for(int i=0;i<10;i++) 
 { 
  buffer_arr[i]=analogRead(A0);
  delay(30);
 }
 for(int i=0;i<9;i++)
 {
    for(int j=i+1;j<10;j++)
      {
      if(buffer_arr[i]>buffer_arr[j])
        {
        temp=buffer_arr[i];
        buffer_arr[i]=buffer_arr[j];
        buffer_arr[j]=temp;
        }
    }
 }
  avgval=0;
  for(int i=2;i<8;i++)
    avgval+=buffer_arr[i];
  float volt=(float)avgval*5.0/1024/6;
  float ph_act = -5.70 * volt + calibration_value;
  if(ph_act<0)
    ph_act=0;
  else if(ph_act>14)
    ph_act=14;

  if(ph_act<6.5 || ph_act>9){
      tone(8, NOTE_B3, 500);
      digitalWrite(9,HIGH); 
  }
  else
    digitalWrite(9,LOW);       
  lcd.setCursor(0, 0);
  lcd.print("pH Val: ");
  lcd.print(ph_act);
  delay(100);
  if(ph_act<6.5 || ph_act>9){
      tone(8, NOTE_B3, 500);
      showWarningMessage("6.5 to 9");
  }   
}
void temperatureManager(){
  lcd.clear(); 
  float val = analogRead(A1);
  float temp= (val/1024.0) * 5000;
  temp=temp/10; 
  lcd.print("Temp  = ");
  lcd.print(temp);
  lcd.print("C");
  if(temp> 36 || temp<26 ){
      tone(8, NOTE_B3, 500);
      showWarningMessage("26C to 36C");
      digitalWrite(11,HIGH);              
}
 else
  digitalWrite(11,LOW);  
}
void oxyzenManager(){
  lcd.clear(); 
  if(digitalRead(12) == HIGH){
    lcd.setCursor(0,0);
    lcd.print("O2 level high");
    digitalWrite(13,LOW);
  }
else{
  lcd.setCursor(0,0);
  lcd.print("O2 level low ");
  tone(8, NOTE_B3, 500);
  showWarningMessage("5 to 20 ppm");
  digitalWrite(13,HIGH);
}
}
void showWarningMessage(String warning){
      lcd.setCursor(0,2);
      lcd.println("NOT SAFE");
      delay(500);
      lcd.clear();                      
      lcd.setCursor(0,0);
      lcd.print("SUGGESTED RANGE:");
      lcd.setCursor(0,1);
      lcd.println(warning);  
}