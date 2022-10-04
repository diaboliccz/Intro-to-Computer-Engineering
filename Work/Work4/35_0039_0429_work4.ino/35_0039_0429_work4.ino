//#include "Timer.h"
#include "LedControl.h"
#include <math.h>
LedControl lc=LedControl(11,13,10,4);  // CLK,DIN,CS,Numberof LED Module
#define PIN A0
#define SLIDE_SIZE 3
int speaker = 8;

int ball_x = random(1,7);
int ball_y = 0;
int slide_val;
int slide_pos;
int slide_x;
int pos;

int Bounce = 0;
int ballUp = 0;
int ballLeft = 1;
int isGameOn = 1;

void Update();
void updateBall();
void gameOver();
void draw();
void Slide();
void Show_Led(int x,int y);
void Clear_Led(int x, int y);
void Start_Led();
void Bounce_Sound();
unsigned long delaytime=10;  // time to updates of the display

void setup() {
  int devices=lc.getDeviceCount();  // find no of LED modules//we have to initall devices in a loop
  for(int address=0;address<devices;address++) {  // set up each device 
    lc.shutdown(address,false);
    lc.setIntensity(address,8);
    
    lc.clearDisplay(address);
  }
  Start_Led();
  delay(1000);
  lc.clearDisplay(3);
}

void loop() {
  while(isGameOn){
    Slide();
  }
}

void Start_Led(){
  //P
  Show_Led(0,0);Show_Led(0,1);Show_Led(0,2);Show_Led(0,3);
  Show_Led(1,0);Show_Led(1,2);
  Show_Led(2,0);Show_Led(2,1);Show_Led(2,2);

  //O
  Show_Led(4,0);Show_Led(4,1);Show_Led(4,2);Show_Led(4,3);
  Show_Led(5,0);Show_Led(5,3);
  Show_Led(6,0);Show_Led(6,3);
  Show_Led(7,0);Show_Led(7,1);Show_Led(7,2);Show_Led(7,3);

  //N
  Show_Led(0,5);Show_Led(0,6);Show_Led(0,7);
  Show_Led(1,6);
  Show_Led(2,5);Show_Led(2,6);Show_Led(2,7);

  //G
  Show_Led(4,5);Show_Led(4,6);Show_Led(4,7);
  Show_Led(5,5);Show_Led(5,7);
  Show_Led(6,5);Show_Led(6,6);Show_Led(6,7);
  Show_Led(7,5);Show_Led(7,6);Show_Led(7,7);
}


void Slide(){
  slide_val = analogRead(PIN);
  slide_x = (slide_val*6/1024)+1;//  1<=x<=6
  if(slide_x<0.1){
    slide_x = 1;
  }
  for(int i = 0;i<=7;i++){
    if(i >= slide_x-1 && i <= slide_x+1){ 
      Show_Led(i,7);
    }
    else{
      Clear_Led(i,7);
    }
  }
  Ball();
}

void Ball(){
  if(ball_y >=0 && ball_y<=8){
    if(ballUp){
      ball_y--;
    }
    else{
      ball_y++;
    }
    if(ball_y == 7){
      ballUp = 1;
    }
    if(ball_y == 0){
      ballUp = 0;
    }

    // bounce check
    if(ball_y == 6 && ((ball_x >= slide_x-2) && (ball_x <= slide_x+2))){
      ballUp = 1;
      Bounce=1;
      Bounce_Sound();
    }
    /*if(ball_y == 7){
      isGameOn = 0;
    }*/
  }
  
  if(ball_x >=0 && ball_x<=8){
    if(ballLeft){
      ball_x--;
    }
    else{
      ball_x++;
    }
    if(ball_x == 7){
      ballLeft = 1;
    }
    if(ball_x == 0){
      ballLeft = 0;
    }
  }

  Show_Led(ball_x,ball_y);
  delay(50);
  Clear_Led(ball_x,ball_y);
}

void Bounce_Sound(){
  if(Bounce){
    tone(speaker,1000,20);
  }
}

void Clear_Led(int x,int y){
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      if(i == y && j == x){
        lc.setLed(3,y,x,false);
      }
    }
  }
}
void Show_Led(int x,int y){
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      if(i == y && j == x){
        lc.setLed(3,y,x,true);
      }
    }
  }
}



void gameOver(){
  isGameOn = 0;
}
