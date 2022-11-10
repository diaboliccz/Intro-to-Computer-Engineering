#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#include <TimerOne.h>

struct Time {
  int hour;
  int min;
  int sec;
};

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0


// change this to make the song slower or faster
int tempo = 180;

// change this to whichever pin you want to use
int buzzer = 8;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  // Nokia Ringtone 
  // Score available at https://musescore.com/user/29944637/scores/5266155
  
  NOTE_E5, 8, NOTE_D5, 8, NOTE_FS4, 4, NOTE_GS4, 4, 
  NOTE_CS5, 8, NOTE_B4, 8, NOTE_D4, 4, NOTE_E4, 4, 
  NOTE_B4, 8, NOTE_A4, 8, NOTE_CS4, 4, NOTE_E4, 4,
  NOTE_A4, 2, 
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

void Song() {
  // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}



// LDR Settings
#define LDR_PIN A3
unsigned int analogValue;

// LED Settings
#define LED_RED 6
#define LED_GREEN 7

// OLED Settings
#define OLED_RESET -1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Accelermeter Settings
const unsigned int X_AXIS_PIN = 0;
const unsigned int Y_AXIS_PIN = 1;
const unsigned int Z_AXIS_PIN = 2;
const unsigned int NUM_AXES = 3;
const unsigned int PINS[NUM_AXES] = {
  X_AXIS_PIN, Y_AXIS_PIN, Z_AXIS_PIN
};
const unsigned int BUFFER_SIZE = 16;
int buffer[NUM_AXES][BUFFER_SIZE];
int buffer_pos[NUM_AXES] = { 0 };

// Button Settings
#define BTN_1 10
#define BTN_2 11
#define BTN_3 12
#define BTN_4 13
int button[4] = { BTN_1, BTN_2, BTN_3, BTN_4 };
int reading[4];
int buttonState[4];
int lastButtonState[4] = { HIGH, HIGH, HIGH, HIGH };
unsigned long long int debounceDelay = 50;
unsigned long long int lastDebounceTime[4];

#define BUZZER_PIN 8

Time Clock = { 0, 0, 0 };

Time Stopwatch = { 0, 0, 0 };
int Stopwatch_On = 1;

Time Alarm = { 0, 0, 0 };
Time Alarm_Set = { 0, 0, 0 };
int Alarm_On = 0;


// Mode Setting
#define CLOCK_MODE 0
#define STOPWATCH_MODE 1
#define ALARM_MODE 2
int MODE = CLOCK_MODE;

// ACCELEROMETER 
int get_axis(const int axis) {
  delay(1);
  buffer[axis][buffer_pos[axis]] = analogRead(PINS[axis]);
  buffer_pos[axis] = (buffer_pos[axis] + 1) % BUFFER_SIZE;
  long sum = 0;
  for (unsigned int i = 0; i < BUFFER_SIZE; i++)
    sum += buffer[axis][i];
  return round(sum / BUFFER_SIZE);
}

int get_x() {
  return get_axis(0);
}
int get_y() {
  return get_axis(1);
}
int get_z() {
  return get_axis(2);
}

// Debounce
int debounce(int i) {
  int isChange = 0;
  reading[i] = digitalRead(button[i]);
  if (reading[i] != lastButtonState[i]) {
    lastDebounceTime[i] = millis();
  }
  if ((millis() - lastDebounceTime[i]) >= debounceDelay) {
    if (reading[i] != buttonState[i]) {
      buttonState[i] = reading[i];
      isChange = 1;
    }
  }
  lastButtonState[i] = reading[i];
  return isChange;
}

void LED_Status(int num, int State){
  digitalWrite(num, State);
}

// Timer One
void interruptClock() {
  timer();
  Stopwatch_Timer();
  change_timestart_alarm();
}

// Main Clock
void timer() {
  Clock.sec += 1;
  Clock.min += Clock.sec / 60;
  Clock.sec %= 60;
  Clock.hour += Clock.min / 60;
  Clock.min %= 60;
  Clock.hour %= 24;
  if (Clock.sec == 0) {
    EEPROM.update(20, Clock.hour);
    EEPROM.update(21, Clock.min);
  }
}

void change_hour() {
  Clock.hour += 1;
  EEPROM.update(20, Clock.hour);
  Clock.hour %= 24;
}

void change_min() {
  Clock.min += 1;
  Clock.hour += Clock.min / 60;
  EEPROM.update(21, Clock.min);
  Clock.min %= 60;
}

// Stopwatch
void Stopwatch_Timer() {
  if (!Stopwatch_On) {
    Stopwatch.sec += 1;
    Stopwatch.min += Stopwatch.sec / 60;
    Stopwatch.sec %= 60;
    Stopwatch.hour += Stopwatch.min / 60;
    Stopwatch.min %= 60;
  }
}

void Stopwatch_Reset() {
  Stopwatch.hour = 0;
  Stopwatch.min = 0;
  Stopwatch.sec = 0;
}

// Alarm
void change_hour_alarm() {
  Alarm_Set.hour += 1;
  Alarm_Set.hour %= 24;
}
void change_min_alarm() {
  Alarm_Set.min += 1;
  Alarm_Set.min %= 60;
}
void change_timestart_alarm() {
  if (Clock.sec == 0) {
    Alarm_Set.min = Clock.min;
  }
  if (Clock.min == 0) {
    Alarm_Set.hour = Clock.hour;
  }
}

// Clock
void change_mode() {
  MODE += 1;
  MODE %= 3;
  if (MODE == ALARM_MODE) {
    Alarm.hour = Clock.hour;
    Alarm.min = Clock.min;
  }
}

// Time Text
String clockText = "00 : 00";
String stopwatchText = "00 : 00";
String alarmText = "00 : 00";
String alarm_setText = "00 : 00";
void time_text() {
  clockText[0] = (Clock.hour / 10) + '0';
  clockText[1] = (Clock.hour % 10) + '0';

  clockText[5] = (Clock.min / 10) + '0';
  clockText[6] = (Clock.min % 10) + '0';

  clockText[10] = (Clock.sec / 10) + '0';
  clockText[11] = (Clock.sec % 10) + '0';

  stopwatchText[0] = (Stopwatch.min / 10) + '0';
  stopwatchText[1] = (Stopwatch.min % 10) + '0';

  stopwatchText[5] = (Stopwatch.sec / 10) + '0';
  stopwatchText[6] = (Stopwatch.sec % 10) + '0';

  alarmText[0] = (Alarm.hour / 10) + '0';
  alarmText[1] = (Alarm.hour % 10) + '0';

  alarmText[5] = (Alarm.min / 10) + '0';
  alarmText[6] = (Alarm.min % 10) + '0';

  alarm_setText[0] = (Alarm_Set.hour / 10) + '0';
  alarm_setText[1] = (Alarm_Set.hour % 10) + '0';

  alarm_setText[5] = (Alarm_Set.min / 10) + '0';
  alarm_setText[6] = (Alarm_Set.min % 10) + '0';
}

void Display_Text_OLED(int x, int y, int TextSize, String text){
  OLED.setTextSize(TextSize);
  OLED.setCursor(x, y);
  OLED.println(text);
}

void setup() {
  Serial.begin(9600);
  if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
  } else {
    Serial.println("All OLED Start Work !!!");
  }
  for (int i = 0; i < 4; i++) {
    pinMode(button[i], INPUT_PULLUP);
  }
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(interruptClock);

  Clock.hour = EEPROM.read(20);
  Clock.min = EEPROM.read(21);
}

void loop() {

  unsigned int analogValue = analogRead(LDR_PIN);

  OLED.clearDisplay();
  OLED.setTextColor(WHITE);

  if (analogValue > 900) {
    OLED.dim(true);
  } else {
    OLED.dim(false);
  }

  if (debounce(0)) {
    if (!digitalRead(button[0])) {
      switch (MODE) {
        case CLOCK_MODE:
          change_hour();
          break;
        case STOPWATCH_MODE:
          Stopwatch_On = !Stopwatch_On;
          break;
        case ALARM_MODE:
          change_hour_alarm();
          break;
      }
    }
  }

  if (debounce(1)) {
    if (!digitalRead(button[1])) {
      switch (MODE) {
        case CLOCK_MODE:
          change_min();
          break;
        case STOPWATCH_MODE:
          Stopwatch_On = 1;
          Stopwatch_Reset();
          break;
        case ALARM_MODE:
          change_min_alarm();
          break;
      }
    }
  }

  if (debounce(2)) {
    if (!digitalRead(button[2])) {
      switch (MODE) {
        case ALARM_MODE:
          Alarm_On = Alarm_On == 0 ? 1 : 0;
          break;
      }
    }
  }

  if (debounce(3)) {
    if (!digitalRead(button[3])) {
      change_mode();
    }
  }

  if (Alarm_On && Alarm_Set.hour == Clock.hour && Alarm_Set.min == Clock.min && Clock.sec == 0) {
    while (Alarm_On) {
      Song();
      if (debounce(2)) {
        if (!digitalRead(button[2])) {
          OLED.clearDisplay();
          Alarm_On = 0;
          break;
        }
      }
    }
  }

  Serial.print(get_x());
  Serial.print(" ");
  Serial.print(get_y());
  Serial.print(" ");
  Serial.println(get_z());
  time_text();
  if (get_x() > 360) {
    OLED.setRotation(2);
  } else {
    OLED.setRotation(0);
  }

  switch (MODE) {
    case CLOCK_MODE:
      Display_Text_OLED(10, 0, 1, "Clock");
      Display_Text_OLED(10, 12, 2, clockText);
      break;
    case STOPWATCH_MODE:
      Display_Text_OLED(10, 0, 1, "Stopwatch");
      Display_Text_OLED(10, 12, 2, stopwatchText);
      if(Stopwatch_On){
        Display_Text_OLED(110, 0, 1, "OFF");
        LED_Status(LED_RED, 1);
        LED_Status(LED_GREEN, 0);

      }else{
        Display_Text_OLED(110, 0, 1, "ON");
        LED_Status(LED_RED, 0);
        LED_Status(LED_GREEN,1);
      }
      break;
    case ALARM_MODE:
      Display_Text_OLED(10, 0, 1, "Alarm");
      Display_Text_OLED(10, 25, 1, Alarm_On == 0 ? "OFF" : "ON");
      Display_Text_OLED(60, 12, 1, "NOW");
      Display_Text_OLED(86, 12, 1, alarmText);
      Display_Text_OLED(60, 25, 1, "SET");
      Display_Text_OLED(86, 25, 1, alarm_setText);
      if(Alarm_On){
        LED_Status(LED_RED, 0);
        LED_Status(LED_GREEN, 1);
      }else{
        LED_Status(LED_RED, 1);
        LED_Status(LED_GREEN, 0);
      }
      break;
  }
  //Song();
  OLED.display();
}
