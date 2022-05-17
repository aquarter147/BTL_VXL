
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLNTE_sKQt"
#define BLYNK_DEVICE_NAME "BTL"
#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG
#define APP_DEBUG
#define RXD2 16
#define TXD2 17
#include "BlynkEdgent.h"


char sent[8];
int count=0;
BlynkTimer timer;


int ledpin =13;
int ledpin2 = 27; // canh bao do am cao
int bellpin = 12;
int fanpin=14;


int temperature=26;
int humidity=78;
double dirt = 0.5;
int gas=0;

BLYNK_WRITE(V0){
  int pinValue= param.asInt();
  digitalWrite(ledpin,pinValue);
}
BLYNK_WRITE(V5){
  int pinValue= param.asInt();
  digitalWrite(bellpin,pinValue);
}
BLYNK_WRITE(V4){
  int pinValue= param.asInt();
  digitalWrite(fanpin,pinValue);
}

int char2int(char a){
  return (a - '0'); 
}

void sendData(){
  for(int i=0;i<8;){
      if(Serial2.available()){
        sent[i] = Serial2.read();
        delay(50);
        Serial.println(sent[i]);
        count++; 
        i++;
        if(count==8){
         humidity = char2int(sent[0])*10+char2int(sent[1]);
          temperature = sent[2].toInt()*10+sent[3].toInt();
          dirt = sent[4].toInt()*100 + sent[5].toInt()*10 + sent[6].toInt();
          gas = sent[7].toInt();
          delay(1000);
          count = 0;
          i=0;        
        }    
      }     
}
  Blynk.virtualWrite(V1,temperature);
  if(temperature>60){
    Blynk.virtualWrite(V5,HIGH);
    Blynk.virtualWrite(V0,HIGH);
    Blynk.virtualWrite(V4,HIGH);
    digitalWrite(bellpin,HIGH);
    digitalWrite(ledpin,HIGH);
    digitalWrite(fanpin,HIGH);  
   }
   else{
    Blynk.virtualWrite(V5,LOW);
    Blynk.virtualWrite(V0,LOW);
    Blynk.virtualWrite(V4,LOW);
    digitalWrite(bellpin,LOW);
    digitalWrite(ledpin,LOW);
    digitalWrite(fanpin,LOW);
   }
  Blynk.virtualWrite(V2,humidity);

  Blynk.virtualWrite(V3,dirt);
  
  Blynk.virtualWrite(V6,gas);
  if(gas==1){
    Blynk.virtualWrite(V5,HIGH);
    Blynk.virtualWrite(V0,HIGH);
    Blynk.virtualWrite(V4,HIGH);
    digitalWrite(bellpin,HIGH);
    digitalWrite(ledpin,HIGH);
    digitalWrite(fanpin,HIGH);
  }
  else{
    Blynk.virtualWrite(V5,LOW);
    Blynk.virtualWrite(V0,LOW);
    Blynk.virtualWrite(V4,LOW);
    digitalWrite(bellpin,LOW);
    digitalWrite(ledpin,LOW);
    digitalWrite(fanpin,LOW);
   }
}


double adc2dirt(int a){
  double voltage;
  voltage = 5.0/255 * a;
  dirt = 0.17*voltage -0.1;
}
BLYNK_CONNECTED(){
  Blynk.syncVirtual(V0);
}

void setup()
{
  Serial.begin(115200);
  Serial2.begin(1200, SERIAL_8N1, RXD2, TXD2);
  delay(100);
  pinMode(ledpin2,OUTPUT);
  pinMode(ledpin,OUTPUT);
  pinMode(bellpin,OUTPUT);
  pinMode(fanpin,OUTPUT);
  BlynkEdgent.begin();

//setup a function to be called every 2second
  timer.setInterval(2000L,sendData);
  
}

void loop() {
  BlynkEdgent.run();
  timer.run();
  while(dirt>0.4){
    digitalWrite(ledpin2,HIGH); 
    delay(400);
    digitalWrite(ledpin2,LOW);
    delay(400);
  }
  while(dirt>0.2 && dirt <0.4){
    digitalWrite(ledpin2,HIGH); 
    delay(1000);
    digitalWrite(ledpin2,LOW);
    delay(1000);
  }
}
