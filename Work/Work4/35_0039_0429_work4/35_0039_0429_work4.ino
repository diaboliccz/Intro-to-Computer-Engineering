//#include "Timer.h"
#include "LedControl.h"
#include <math.h>
LedControl lc = LedControl(11, 13, 10, 4); // CLK,DIN,CS,Numberof LED Module
#define PIN A0
#define SLIDE_SIZE 3
int speaker = 8;
int devices = lc.getDeviceCount();
int address;
int addr_slide = 3;
int ball_move_time = 50;

int ball_x = random(1, 7);
int ball_y = 0;
int slide_val;
int slide_pos;
int slide_x;

int Bounce = 0;
int ballUp = 0;
int ballLeft = 1;
int GameOn = 1;

void Update();
void updateBall();
void gameOver();
void draw();
void Slide();
void Show_Led(int address, int x, int y);
void Clear_Led(int address, int x, int y);
void Start_Led();
void Bounce_Sound();

void setup()
{
  int devices = lc.getDeviceCount(); // find no of LED modules//we have to initall devices in a loop
  for (address = 0; address < devices; address++)
  { // set up each device
    lc.shutdown(address, false);
    lc.setIntensity(address, 8);
    lc.clearDisplay(address);
  }
  Start_Led();
  delay(1000);
  for(address=0;address<devices;address++) {
    lc.clearDisplay(address);
  }
}

void loop()
{
  while (GameOn)
  {
    Slide();
  }
}

void Start_Led(){
  //P
  Show_Led(3,0,0);Show_Led(3,0,1);Show_Led(3,0,2);Show_Led(3,0,3);Show_Led(3,0,4);Show_Led(3,0,5);Show_Led(3,0,6);Show_Led(3,0,7);
  Show_Led(3,1,0);Show_Led(3,1,3);
  Show_Led(3,2,0);Show_Led(3,2,3);
  Show_Led(3,3,0);Show_Led(3,3,3);
  Show_Led(3,4,0);Show_Led(3,4,3);
  Show_Led(3,5,0);Show_Led(3,5,3);
  Show_Led(3,6,0);Show_Led(3,6,1);Show_Led(3,6,2);Show_Led(3,6,3);

  //O
  Show_Led(2,0,0);Show_Led(2,0,1);Show_Led(2,0,2);Show_Led(2,0,3);Show_Led(2,0,4);Show_Led(2,0,5);Show_Led(2,0,6);Show_Led(2,0,7);
  Show_Led(2,1,0);Show_Led(2,1,7);
  Show_Led(2,2,0);Show_Led(2,2,7);
  Show_Led(2,3,0);Show_Led(2,3,7);
  Show_Led(2,4,0);Show_Led(2,4,7);
  Show_Led(2,5,0);Show_Led(2,5,7);
  Show_Led(2,6,0);Show_Led(2,6,1);Show_Led(2,6,2);Show_Led(2,6,3);Show_Led(2,6,4);Show_Led(2,6,5);Show_Led(2,6,6);Show_Led(2,6,7);

  //N
  Show_Led(1,0,0);Show_Led(1,0,1);Show_Led(1,0,2);Show_Led(1,0,3);Show_Led(1,0,4);Show_Led(1,0,5);Show_Led(1,0,6);Show_Led(1,0,7);
  Show_Led(1,1,1);
  Show_Led(1,2,2);
  Show_Led(1,3,3);
  Show_Led(1,4,4);
  Show_Led(1,5,5);
  Show_Led(1,6,6);
  Show_Led(1,7,0);Show_Led(1,7,1);Show_Led(1,7,2);Show_Led(1,7,3);Show_Led(1,7,4);Show_Led(1,7,5);Show_Led(1,7,6);Show_Led(1,7,7);

  //G
  Show_Led(0,0,0);Show_Led(0,0,1);Show_Led(0,0,2);Show_Led(0,0,3);Show_Led(0,0,4);Show_Led(0,0,5);Show_Led(0,0,6);Show_Led(0,0,7);
  Show_Led(0,1,0);Show_Led(0,1,7);
  Show_Led(0,2,0);Show_Led(0,2,7);
  Show_Led(0,3,0);Show_Led(0,3,7);
  Show_Led(0,4,0);Show_Led(0,4,3);Show_Led(0,4,7);
  Show_Led(0,5,0);Show_Led(0,5,3);Show_Led(0,5,7);
  Show_Led(0,6,0);Show_Led(0,6,3);Show_Led(0,6,7);
  Show_Led(0,7,0);Show_Led(0,7,3);Show_Led(0,7,4);Show_Led(0,7,5);Show_Led(0,7,6);Show_Led(0,7,7);
}

void Slide()
{
  slide_val = analogRead(PIN);
  slide_x = (slide_val * 30 / 1024) + 1; //  1<=x<=30
  for (int i = 0; i < 8; i++)
  {
    if (i >= slide_x - 1 && i <= slide_x + 1)
    {
      Show_Led(3, i, 7);
    }
    else
    {
      Clear_Led(3, i, 7);
    }
  }

  for (int i = 8; i < 16; i++)
  {
    if (i >= slide_x - 1 && i <= slide_x + 1)
    {
      Show_Led(2, i - 8, 7);
    }
    else
    {
      Clear_Led(2, i - 8, 7);
    }
  }

  for (int i = 16; i < 24; i++)
  {
    if (i >= slide_x - 1 && i <= slide_x + 1)
    {
      Show_Led(1, i - 16, 7);
    }
    else
    {
      Clear_Led(1, i - 16, 7);
    }
  }

  for (int i = 24; i < 32; i++)
  {
    if (i >= slide_x - 1 && i <= slide_x + 1)
    {
      Show_Led(0, i - 24, 7);
    }
    else
    {
      Clear_Led(0, i - 24, 7);
    }
  }
  Ball();
}

void Ball()
{
  if (ball_y >= 0 && ball_y < 32)
  {
    if (ballUp)
    {
      ball_y--;
    }
    else
    {
      ball_y++;
    }
    if (ball_y == 7)
    {
      ballUp = 1;
    }
    if (ball_y == 0)
    {
      ballUp = 0;
    }

    // bounce check
    if (ball_y == 6 && (ball_x >= slide_x-2 && ball_x <= slide_x + 2))
    {
      ballUp = 1;
      Bounce = 1;
      Bounce_Sound();
    }
    
    /*if(ball_y == 7){
      isGameOn = 0;
    }*/
  }

  if (ball_x >= 0 && ball_x < 32)
  {
    if (ballLeft)
    {
      ball_x--;
    }
    else
    {
      ball_x++;
    }
    if (ball_x == 31)
    {
      ballLeft = 1;
    }
    if (ball_x == 0)
    {
      ballLeft = 0;
    }

    if (ball_x >= 0 && ball_x < 8)
    {
      Show_Led(3, ball_x, ball_y);
      delay(ball_move_time);
      Clear_Led(3, ball_x, ball_y);
    }

    if (ball_x >= 8 && ball_x < 16)
    {
      Show_Led(2, ball_x - 8, ball_y);
      delay(ball_move_time);
      Clear_Led(2, ball_x - 8, ball_y);
    }

    if (ball_x >= 16 && ball_x < 24)
    {
      Show_Led(1, ball_x - 16, ball_y);
      delay(ball_move_time);
      Clear_Led(1, ball_x - 16, ball_y);
    }

    if (ball_x >= 24 && ball_x < 32)
    {
      Show_Led(0, ball_x - 24, ball_y);
      delay(ball_move_time);
      Clear_Led(0, ball_x - 24, ball_y);
    }
  }
}

void Bounce_Sound()
{
  if (Bounce)
  {
    tone(speaker, 1000, 20);
  }
}

void Clear_Led(int addr, int x, int y)
{
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (i == y && j == x)
      {
        lc.setLed(addr, y, x, false);
      }
    }
  }
}
void Show_Led(int addr, int x, int y)
{
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (i == y && j == x)
      {
        lc.setLed(addr, y, x, true);
      }
    }
  }
}

void gameOver()
{
  GameOn = 0;
}
