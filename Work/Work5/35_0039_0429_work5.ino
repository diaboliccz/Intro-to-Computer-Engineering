// West
#define LED_W_R 4
#define LED_W_Y 3
#define LED_W_G 2
#define WEST_BUTTON_PIN 11

// South
#define LED_S_R 7
#define LED_S_Y 6
#define LED_S_G 5
#define SOUTH_BUTTON_PIN 12

// Walk
#define LED_WALK_G 8
#define LED_WALK_R 9
#define WALK_BUTTON_PIN 13

// Input
#define waitWest 0
#define goWest 1
#define waitSouth 2
#define goSouth 3
#define wait_WALK_SOUTH 4
#define wait_WALK_WEST 5
#define goWalk 6
#define b1_OFF 7
#define b1_ON 8
#define b2_OFF 9
#define b2_ON 10
#define b3_OFF 11
#define b3_ON 12

struct State
{
  unsigned long ST_Out;
  unsigned long Time;
  unsigned long Next[8];
};

typedef const struct State SType;

SType FSM[13] = {
    {B10010100, 400, {goWest, goWest, goWest, goWest, goWest, goWest, goWest, goWest}},                                            // waitWest
    {B10100001, 2000, {goWest, goWest, waitSouth, waitSouth, wait_WALK_WEST, wait_WALK_WEST, waitSouth, waitSouth}},               // goWest
    {B10100010, 400, {goSouth, goSouth, goSouth, goSouth, goSouth, goSouth, goSouth, goSouth}},                                    // waitSouth
    {B10001100, 2000, {goSouth, waitWest, goSouth, waitWest, wait_WALK_SOUTH, wait_WALK_SOUTH, wait_WALK_SOUTH, wait_WALK_SOUTH}}, // goSouth
    {B10010100, 400, {goWalk, goWalk, goWalk, goWalk, goWalk, goWalk, goWalk, goWalk}},                                            // wait_WALK_SOUTH
    {B10100010, 400, {goWalk, goWalk, goWalk, goWalk, goWalk, goWalk, goWalk, goWalk}},                                            // wait_WALK_WEST
    {B01100100, 2000, {goWalk, b1_OFF, b1_OFF, b1_OFF, goWalk, b1_OFF, b1_OFF, b1_OFF}},                                           // goWalk
    {B00100100, 300, {b1_ON, b1_ON, b1_ON, b1_ON, b1_ON, b1_ON, b1_ON, b1_ON}},                                                    // b1_OFF 1
    {B01100100, 300, {b2_OFF, b2_OFF, b2_OFF, b2_OFF, b2_OFF, b2_OFF, b2_OFF, b2_OFF}},                                            // b1_ON
    {B00100100, 300, {b2_ON, b2_ON, b2_ON, b2_ON, b2_ON, b2_ON, b2_ON, b2_ON}},                                                    // b2_OFF 2
    {B01100100, 300, {b3_OFF, b3_OFF, b3_OFF, b3_OFF, b3_OFF, b3_OFF, b3_OFF, b3_OFF}},                                            // b2_ON
    {B00100100, 300, {b3_ON, b3_ON, b3_ON, b3_ON, b3_ON, b3_ON, b3_ON, b3_ON}},                                                    // b3_OFF 3
    {B01100100, 300, {goWalk, goWest, goSouth, goWest, goWalk, goWest, goSouth, goWest}},                                          // b3_ON
};
unsigned long S = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(LED_W_G, OUTPUT);
  pinMode(LED_W_Y, OUTPUT);
  pinMode(LED_W_R, OUTPUT);
  pinMode(WEST_BUTTON_PIN, INPUT_PULLUP);

  pinMode(LED_S_G, OUTPUT);
  pinMode(LED_S_Y, OUTPUT);
  pinMode(LED_S_R, OUTPUT);
  pinMode(SOUTH_BUTTON_PIN, INPUT_PULLUP);

  pinMode(LED_WALK_G, OUTPUT);
  pinMode(LED_WALK_R, OUTPUT);
  pinMode(WALK_BUTTON_PIN, INPUT_PULLUP);
}
int West, South, Walk, input, inputPrev;
void loop()
{
  // West
  digitalWrite(LED_W_G, FSM[S].ST_Out & B00000001);
  digitalWrite(LED_W_Y, FSM[S].ST_Out & B00000010);
  digitalWrite(LED_W_R, FSM[S].ST_Out & B00000100);

  // South
  digitalWrite(LED_S_G, FSM[S].ST_Out & B00001000);
  digitalWrite(LED_S_Y, FSM[S].ST_Out & B00010000);
  digitalWrite(LED_S_R, FSM[S].ST_Out & B00100000);

  // Walk
  digitalWrite(LED_WALK_G, FSM[S].ST_Out & B01000000);
  digitalWrite(LED_WALK_R, FSM[S].ST_Out & B10000000);

  delay(FSM[S].Time);
  West = !digitalRead(WEST_BUTTON_PIN);
  South = !digitalRead(SOUTH_BUTTON_PIN);
  Walk = !digitalRead(WALK_BUTTON_PIN);
  
  input = Walk * 4 + South * 2 + West;

  if(S == 6){
    inputPrev = input;
  }

  if(S == 12){
    S = FSM[S].Next[inputPrev];
  }
  else{
    S = FSM[S].Next[input];
  }
  Serial.println(input);Serial.println(inputPrev);Serial.println("------------");
}