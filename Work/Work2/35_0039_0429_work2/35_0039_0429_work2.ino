//button setup
int button[3] = {10, 11, 12};
bool pressed[3] = {LOW, LOW, HIGH};

//LED setup
int LED[3] = {2, 3, 4};
bool LED_state[3] = {LOW, LOW, LOW};

//debounce setup
bool buttonState[3] = {HIGH, HIGH, LOW};
bool lastButtonState[3] = {HIGH, HIGH, LOW};
long lastDebounceTime[3] = {0, 0, 0};
long debounceDelay = 50;

//cooldown timer setup
int LED_timer[3] = {3000, 2000, 3000};
long lastTimer[3] = {0, 0, 0};

//logic setup
bool blinkState[3] = {0, 0, 0};
long yellowTimer = 0;

void setup() {
  Serial.begin(9600);
  for(int i=0;i<3;++i){
    pinMode(button[i], INPUT);
    pinMode(LED[i], OUTPUT);
  }
  pinMode(button[0], INPUT_PULLUP);
}

int debounce(int reading, int i){
  bool isChange = 0; // check if button state is change
  if(reading != lastButtonState[i]){
    lastDebounceTime[i] = millis();
  }
  if(millis() - lastDebounceTime[i] > debounceDelay){
    if(reading != buttonState[i]){
      buttonState[i] = reading;
      isChange = 1;
    }
  }
  lastButtonState[i] = reading;
  return isChange; 
}

int cooldown(int i){
//  Serial.println(currentTimer[i] - lastTimer[i]);
  if(millis() - lastTimer[i] > LED_timer[i]){
    return 1;
  }
  return 0;
}

/*
 0 -> green
 1 -> yellow
 2 -> red
 */
int LED_logic_check(int i){
  if(i==2) return 1; // if green
  if(i==0 && !LED_state[2]) return 1; // if red and not green
  if(i==1 && !LED_state[0] && !LED_state[2]) return 1; // if yellow and not green not red
  return 0; // else
}

void yellowState(){
  if(millis() - yellowTimer > 500){
    yellowTimer = millis();
    LED_state[1] = !LED_state[1];
  }
}

void LED_blink(int i){
  for(int i=0;i<3;++i){
    if(blinkState[i]==1){
      if(i==1){
        yellowState();
      }
      else{
        LED_state[i] = HIGH;
      }
    }
    else{
      LED_state[i] = LOW;
    }
    if(cooldown(i)){
      blinkState[i] = 0;
      LED_state[i] = LOW;
    }
    digitalWrite(LED[i], LED_state[i]);
  }
}

void loop() {
  for(int i=0;i<3;++i){
    int Readswitch = digitalRead(button[i]);
    if(debounce(Readswitch, i) && Readswitch == pressed[i]){
      if(blinkState[i] == 0 && LED_logic_check(i)){
        blinkState[i] = 1;
        lastTimer[i] = millis();
      }
      else if(blinkState[i] == 1){
        blinkState[i] = 0;
      }
    }
    LED_blink(i);
  }
}
