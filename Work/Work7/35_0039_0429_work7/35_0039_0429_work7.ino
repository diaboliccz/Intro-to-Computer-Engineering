#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimerOne.h>

// Struct Time
struct Time
{
  int hour;
  int min;
  int sec;
};

// OLED Setting
#define OLED_RESET -1 //ขา reset เป็น -1 ถ้าใช้ร่วมกับขา Arduino reset
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_ADDRESS 0x3C
// กำหนดขาต่อ I2C กับจอ OLED
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Button Setting
#define button_1 10
#define button_2 11
#define button_3 12
#define button_4 13
int button[4] = {button_1, button_2, button_3, button_4};
int read[4];
int buttonState[4];
int lastButtonState[4] = {HIGH, HIGH, HIGH, HIGH};
unsigned long long int debounceDelay = 50;
unsigned long long int lastDebounceTime[4];

// LED Setting
#define LED_RED 6
#define LED_GREEN 7

// ==== ACCELEROMETER SETTING ====
int X_AXIS_PIN = 0;
int Y_AXIS_PIN = 1;
int Z_AXIS_PIN = 2;
#define NUM_AXES 3
int PINS[NUM_AXES] = {
  X_AXIS_PIN, Y_AXIS_PIN, Z_AXIS_PIN
};
const unsigned int BUFFER_SIZE = 16;
int buffer[NUM_AXES][BUFFER_SIZE];
int buffer_pos[NUM_AXES] = { 0 };

// ==== ACCELEROMETER ====
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

void LED_RED_Status(int status){
  digitalWrite(LED_RED,status);
}
void LED_GREEN_Status(int status){
  digitalWrite(LED_GREEN,status);
}

// Buzzer Setting(เสียง)
#define BUZZER_PIN 8
/* 
  Nokia Tune
  Connect a piezo buzzer or speaker to pin 11 or select a new pin.
  More songs available at https://github.com/robsoncouto/arduino-songs                                            
                                              
                                              Robson Couto, 2019
*/

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

void Song(){
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

// Clock(นาฬิกา)
Time Clock = {0, 0, 0};

// Stopwatch(นาฬิกาจับเวลา)
Time Stopwatch = {0, 0, 0};
int Stopwatch_On = 0; // เริ่มมาให้หยุดก่อนยังไม่ต้องนับ
int Countdown_On = 0;

// Alarm Clock(นาฬิกาปลุก)
Time Alarm = {0, 0, 0};
Time Alarm_Set{0, 0, 0}; // เวลาที่เซฟไว้
int Alarm_On = 0;

// Mode
#define CLOCK_MODE 0
#define STOPWATCH_MODE 1
#define ALARM_MODE 2
int MODE = ALARM_MODE;

// สำหรับ Debounce button
int debounce(int i)
{
  int isChange = 0;
  read[i] = digitalRead(button[i]);
  if (read[i] != lastButtonState[i])
  {
    lastDebounceTime[i] = millis();
  }
  if ((millis() - lastDebounceTime[i]) >= debounceDelay)
  {
    if (read[i] != buttonState[i])
    {
      buttonState[i] = read[i];
      isChange = 1;
    }
  }
  lastButtonState[i] = read[i];
  return isChange;
}

// ส่วน Timer One
void interruptClock()
{
  Clock_Timer();
  Stopwatch_Timer();
  Alarm_Change_Start();
}

// นาฬิกา
void Clock_Timer()
{
  Clock.sec += 1;
  Clock.min += Clock.sec / 60;
  Clock.sec %= 60;

  Clock.hour += Clock.min / 60;
  Clock.min %= 60;

  Clock.hour %= 24;
}

// เพิ่ม ชั่วโมง
void timer_change_hour()
{
  Clock.hour += 1;
  Clock.hour %= 24;
}
void timer_change_min()
{
  Clock.min += 1;
  Clock.min %= 60;
}

// นาฬิกาจับเวลาแบบ เพิ่มไปเรื่อยๆ จนกว่าจะกดหยุด
void Stopwatch_Timer()
{
  if (Stopwatch_On == 1 && Countdown_On == 0)
  { // ถ้าไม่หยุด
    Stopwatch.sec += 1;
    Stopwatch.min += Stopwatch.sec / 60;
    Stopwatch.sec %= 60;

    Stopwatch.hour += Stopwatch.min / 60;
    Stopwatch.min %= 60;

    Stopwatch.hour %= 24;
  }
  else if(Stopwatch_On == 0 && Countdown_On == 1){
    Stopwatch.sec-= 1;
    Stopwatch.min -= Stopwatch.sec / 60;
    Stopwatch.sec %= 60;

    Stopwatch.hour -= Stopwatch.min / 60;
    Stopwatch.min %= 60;

    Stopwatch.hour %= 24;

    if(Stopwatch.sec <= 0){
      Stopwatch_On = 0;
      Countdown_On = 0;
      Stopwatch.sec = 0;
      tone(BUZZER_PIN, 500, 100);
    }
  }
}

// Stopwatch Reset
void Stopwatch_Reset()
{
  Stopwatch_On = 0;
  Countdown_On = 0;
  LED_RED_Status(1);
  LED_GREEN_Status(0);

  Stopwatch.sec = 0;
  Stopwatch.min = 0;
  Stopwatch.hour = 0;
}

// นาฬิกาปลุก
void alarm_change_hour()
{
  Alarm_Set.hour += 1;
  Alarm_Set.hour %= 24;
}
void alarm_change_min()
{
  Alarm_Set.min += 1;
  Alarm_Set.min %= 60;
}
void Alarm_Change_Start()
{
  if (Clock.sec == 0)
  {
    Alarm.min = Clock.min;
  }
  if (Clock.min == 0)
  {
    Alarm.hour = Clock.hour;
  }
}

// Mode Select(ใช้สำหรับเลือกโหมด)
void Mode_Change()
{
  MODE += 1;
  MODE %= 3;

  if (MODE == ALARM_MODE) // ปรับเวลาในAlarm ให้เท่ากับClock
  {
    Alarm.hour = Clock.hour;
    Alarm.min = Clock.min;
  }
}

// การแสดงข้อความ Time Text
String Timer_Text = "00 : 00 : 00";
String Stopwatch_Text = "00 : 00 : 00";
String Alarm_Text = "00 : 00";
String Alarm_Set_Text = "00 : 00";
void time_text()
{

  // Timer Text
  // Timer hour
  Timer_Text[0] = (Clock.hour / 10) + '0';
  Timer_Text[1] = (Clock.hour % 10) + '0';

  // Timer min
  Timer_Text[5] = (Clock.min / 10) + '0';
  Timer_Text[6] = (Clock.min % 10) + '0';

  // Timer sec
  Timer_Text[10] = (Clock.sec / 10) + '0';
  Timer_Text[11] = (Clock.sec % 10) + '0';

  // Count Up Text
  // Count Up hour
  Stopwatch_Text[0] = (Stopwatch.hour / 10) + '0';
  Stopwatch_Text[1] = (Stopwatch.hour % 10) + '0';

  // Count Up min
  Stopwatch_Text[5] = (Stopwatch.min / 10) + '0';
  Stopwatch_Text[6] = (Stopwatch.min % 10) + '0';

  // Count Up sec
  Stopwatch_Text[10] = (Stopwatch.sec / 10) + '0';
  Stopwatch_Text[11] = (Stopwatch.sec % 10) + '0';

  // Alarm Text
  // Alarm hour
  Alarm_Text[0] = (Alarm.hour / 10) + '0';
  Alarm_Text[1] = (Alarm.hour % 10) + '0';

  // Alarm min
  Alarm_Text[5] = (Alarm.min / 10) + '0';
  Alarm_Text[6] = (Alarm.min % 10) + '0';

  // Alarm Save Text
  // Alarm Save hour
  Alarm_Set_Text[0] = (Alarm_Set.hour / 10) + '0';
  Alarm_Set_Text[1] = (Alarm_Set.hour % 10) + '0';

  // Alarm Save min
  Alarm_Set_Text[5] = (Alarm_Set.min / 10) + '0';
  Alarm_Set_Text[6] = (Alarm_Set.min % 10) + '0';
}

// แสดงข้อความในจอ OLED
void Display_Show(int x_1, int y_1, String text)
{
  OLED.setCursor(x_1, y_1);
  OLED.println(text);
}

// ในส่วนของ Setup
void setup()
{
  Serial.begin(9600);
  if (!OLED.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  { // สั่งให้จอ OLED เริ่มทำงานที่ Address 0x3C
    Serial.println("SSD1306 allocation failed");
  }
  else
  {
    Serial.println("ArdinoAll OLED Start Work !!!");
  }

  for (int i = 0; i < 4; i++) {
    pinMode(button[i], INPUT_PULLUP);
  }

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  Timer1.initialize(1000000);
  Timer1.attachInterrupt(interruptClock);
}

void loop()
{
  if (debounce(0))
  {
    if (!digitalRead(button[0]))
    {
      if (MODE == CLOCK_MODE)
      {
        timer_change_hour();
      }
      if (MODE == STOPWATCH_MODE)
      {
        Stopwatch_On = !Stopwatch_On;
        Countdown_On = 0;
        tone(BUZZER_PIN, 100, 100);
      }
      if (MODE == ALARM_MODE)
      {
        alarm_change_hour();
      }
    }
  }
  if (debounce(1))
  {
    if (!digitalRead(button[1]))
    {
      if (MODE == CLOCK_MODE)
      {
        timer_change_min();
      }
      if (MODE == STOPWATCH_MODE)
      {
        Countdown_On = !Countdown_On;
        Stopwatch_On = 0;
        //countUpReset();
        tone(BUZZER_PIN, 100, 100);
      }
      if (MODE == ALARM_MODE)
      {
        alarm_change_min();
      }
    }
  }
  if (debounce(2))
  {
    if (!digitalRead(button[2]))
    {
      Mode_Change();
    }
  }
  if (debounce(3))
  {
    if (!digitalRead(button[3]))
    {
      if (MODE == STOPWATCH_MODE){
        Stopwatch_On = 0;
        Stopwatch_Reset();
      }
      if (MODE == ALARM_MODE)
      {
        Alarm_On = Alarm_On == 0 ? 1 : 0;
        /*if (Alarm_On)
        {
          Alarm_Set.hour = Alarm.hour;
          Alarm_Set.min = Alarm.min;
        }*/
      }
    }
  }

  // ส่วนของเสียง
  if ((Alarm_On) && (Alarm_Set.hour == Alarm.hour) && (Alarm_Set.min == Alarm.min) && Clock.sec == 0)
  {
    Song();
    Alarm_On = 0;
  }

  time_text();
  OLED.clearDisplay();
  OLED.setTextColor(WHITE);
  if (MODE == CLOCK_MODE)
  {
    Display_Show(0,0, "Main Clock");
    Display_Show(0, 25, Timer_Text);
  }
  else if (MODE == STOPWATCH_MODE)
  {
    Display_Show(0,0, "StopWatch");
    Display_Show(0, 25, Stopwatch_Text);
    if(Stopwatch_On == 0 && Countdown_On == 0){
      OLED.setCursor(86,0);
      OLED.setTextColor(BLACK,WHITE);
      OLED.println("PAUSE");
      //Display_Show(86,0, "PAUSE");
      LED_RED_Status(1);
      LED_GREEN_Status(0);
    }
    else if (Stopwatch_On == 1 || Countdown_On == 1){
      LED_RED_Status(0);
      LED_GREEN_Status(1);
    }
  }
  else if (MODE == ALARM_MODE)
  {
    LED_RED_Status(0);
    LED_GREEN_Status(0);
    Display_Show(0,0, "Alarm");
    Display_Show(60, 12, "Now");
    Display_Show(86, 12, Alarm_Text);

    Display_Show(60, 25, "Set");
    Display_Show(86, 25, Alarm_Set_Text);
    Display_Show(0, 25, Alarm_On == 0 ? "OFF" : "ON");
    if(Alarm_On){
      LED_GREEN_Status(1);
      LED_RED_Status(0);
    }
    else{
      LED_GREEN_Status(0);
      LED_RED_Status(1);
    }
  }

  for(int i = 0;i<4;i++){
    Serial.print(!digitalRead(button[i]));
    if(i == 3){
      Serial.println(" ");
    }
  }
  //Song();
  OLED.setTextSize(1);
  OLED.display();

  Serial.print(get_x());
  Serial.print(" ");
  Serial.print(get_y());
  Serial.print(" ");
  Serial.println(get_z());
}
