void setup() {
  
}

void loop() {
  pinMode(12,OUTPUT);    
  digitalWrite(12,HIGH);
  pinMode(11,OUTPUT);
  digitalWrite(11,LOW);
  pinMode(10,INPUT);
  delay(250);

  pinMode(12,OUTPUT);   
  digitalWrite(12,LOW);
  pinMode(11,OUTPUT);
  digitalWrite(11,HIGH);
  pinMode(10,INPUT);
  delay(250);

  pinMode(12,INPUT);
  pinMode(11,OUTPUT);
  digitalWrite(11,HIGH);
  pinMode(10,OUTPUT);
  digitalWrite(10,LOW);
  delay(250);

  pinMode(12,INPUT);
  pinMode(11,OUTPUT);
   digitalWrite(11,LOW);
  pinMode(10,OUTPUT);
   digitalWrite(10,HIGH);
  delay(250);

  pinMode(12,OUTPUT);
  digitalWrite(12,HIGH);
  pinMode(11,INPUT);
  pinMode(10,OUTPUT);
    digitalWrite(10,LOW);
  delay(250);

  pinMode(12,OUTPUT);
  digitalWrite(12,LOW);
  pinMode(11,INPUT);        
  pinMode(10,OUTPUT);
   digitalWrite(10,HIGH);
  delay(250);

//

pinMode(12,OUTPUT);
  digitalWrite(12,LOW);
  pinMode(11,INPUT);        
  pinMode(10,OUTPUT);
   digitalWrite(10,HIGH);
  delay(250);

pinMode(12,OUTPUT);
  digitalWrite(12,HIGH);
  pinMode(11,INPUT);
  pinMode(10,OUTPUT);
    digitalWrite(10,LOW);
  delay(250);

  pinMode(12,INPUT);
  pinMode(11,OUTPUT);
   digitalWrite(11,LOW);
  pinMode(10,OUTPUT);
   digitalWrite(10,HIGH);
  delay(250);

  pinMode(12,INPUT);
  pinMode(11,OUTPUT);
  digitalWrite(11,HIGH);
  pinMode(10,OUTPUT);
  digitalWrite(10,LOW);
  delay(250);

  pinMode(12,OUTPUT);   
  digitalWrite(12,LOW);
  pinMode(11,OUTPUT);
  digitalWrite(11,HIGH);
  pinMode(10,INPUT);
  delay(250);

  pinMode(12,OUTPUT);    
  digitalWrite(12,HIGH);
  pinMode(11,OUTPUT);
  digitalWrite(11,LOW);
  pinMode(10,INPUT);
  delay(250);
}
