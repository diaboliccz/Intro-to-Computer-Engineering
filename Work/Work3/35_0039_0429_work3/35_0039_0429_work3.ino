#define b1 2
#define b2 3
#define delayTime 70
int number; // number to display
int bitPattern[11] = {B00111111,B00000110,B01011011,B01001111,B01100110,B01101101,B01111101,B00000111,B01111111,B01101111};
const byte numPins = 8;
const int segmentPins[8] = {12,11,10,9,7,6,5,4};
bool reading[2] ;
bool lastbuttonState[2] = {HIGH,HIGH};
unsigned long long int lastDebounceTime[2];
int button[2] = {b1,b2};
bool buttonStatee[2] = {HIGH,HIGH};
int y=0,check=0;
int randomNo;


void debounce(int c){
  reading[c]=digitalRead(button[c]);
  if(reading[c] != lastbuttonState[c]){
    lastDebounceTime[c] = millis();
  }
  if(millis()-lastDebounceTime[c] > delayTime)
  {
    buttonStatee[c] = reading[c];
  }
  lastbuttonState[c] = reading[c];
}

void setup () {
Serial.begin(9600);
pinMode(b1,INPUT);
pinMode(b2,INPUT);
for (int i = 0; i < numPins; i++){
        pinMode(segmentPins[i], OUTPUT);
}
randomSeed(analogRead(0));
randomNo = random(0,10);
Serial.print("ran = ");
Serial.println(randomNo);
}

void loop() {
boolean isBitSet;
if(y==0 && check==0){
  for (int segment = 0; segment < 8; segment++)
  { 
  isBitSet = bitRead(bitPattern[0], segment);
  digitalWrite(segmentPins[segment],!isBitSet);
  }
  check=1;
}
if(digitalRead(b2) == HIGH){
   debounce(1);
  if(y==randomNo){
    for(int j=0;j<8;j++){
      isBitSet = bitRead(bitPattern[0], j);
      digitalWrite(segmentPins[j],!isBitSet);
    }
      Serial.println("=random");
      randomNo = random(0,10);
      Serial.print("ran = ");
      Serial.println(randomNo);          
    }
  else if(y<randomNo){
    for(int k=0;k<8;k++){
      isBitSet = bitRead(B00111000, k);
      digitalWrite(segmentPins[k],!isBitSet);
    }
     Serial.println("<random");
  }
  else if(y>randomNo){
    for(int l=0;l<8;l++){
      isBitSet = bitRead(B00111101, l);
      digitalWrite(segmentPins[l],!isBitSet);
    }
      Serial.println(">random");
  }
}
debounce(0);
if(digitalRead(b1) == HIGH && digitalRead(b1)!= buttonStatee[0]){
  y++;
  if(y==10){
    y=0;
    y++;
  }
  Serial.println("press1");
  if(y!=0){
  for (int segment = 0; segment < 8; segment++)
  { 
  isBitSet = bitRead(bitPattern[y], segment);
  digitalWrite(segmentPins[segment],!isBitSet);
  }
Serial.print("y = ");
Serial.println(y);
  }
}
  delay(150);
}
