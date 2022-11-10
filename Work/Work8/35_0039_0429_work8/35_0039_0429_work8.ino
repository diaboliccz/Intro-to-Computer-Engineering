#include <Arduino_FreeRTOS.h>
#include <queue.h>

// Define LED
#define LED_RED 2
#define LED_YELLOW 3
#define LED_GREEN 4

// Define Button
#define BUTTON_RED 5
#define BUTTON_YELLOW 6
#define BUTTON_GREEN 7

// Defin Delay
#define Delay(x) vTaskDelay(pdMS_TO_TICKS(x))

QueueHandle_t RED_QUEUE;
QueueHandle_t YELLOW_QUEUE;
QueueHandle_t GREEN_QUEUE;

void setup()
{
    //Serial.begin(9600);
    RED_QUEUE = xQueueCreate(10, sizeof(bool));
    YELLOW_QUEUE = xQueueCreate(10, sizeof(bool));
    GREEN_QUEUE = xQueueCreate(10, sizeof(bool));

    xTaskCreate(redTaskReceiver, "RED TASK RECIEVER", 128, (void *)LED_RED, 1, NULL);
    xTaskCreate(redTaskSender, "RED TASK SENDER", 128, (void *)BUTTON_RED, 1, NULL);
    xTaskCreate(yellowTaskReceiver, "YELLOW TASK RECIEVER", 128, (void *)LED_YELLOW, 1, NULL);
    xTaskCreate(yellowTaskSender, "YELLOW TASK SENDER", 128, (void *)BUTTON_YELLOW, 1, NULL);
    xTaskCreate(greenTaskReceiver, "GREEN TASK RECEIVER", 128, (void *)LED_GREEN, 1, NULL);
    xTaskCreate(greenTaskSender, "GREEN TASK SENDER", 128, (void *)BUTTON_GREEN, 1, NULL);
}

int Button_Pressed_Check(int pin)
{
    Delay(10);
    if (digitalRead(pin) == 0)
        return 1;
    return 0;
}

void redTaskSender(void *pvParameters)
{
    unsigned short button = (unsigned short)pvParameters;
    int Button_Pressed = 0;

    pinMode(button, INPUT_PULLUP);

    while (1)
    {
        if (Button_Pressed_Check(button) && !Button_Pressed)
        {
            Button_Pressed = 1;
            xQueueSend(RED_QUEUE, &button, 0);
        }
        if (!Button_Pressed_Check(button))
            Button_Pressed = 0;
        Delay(10);
    }
}

void redTaskReceiver(void *pvParameters)
{
    unsigned short led = (unsigned short)pvParameters;
    pinMode(led, OUTPUT);
    unsigned short Red_now = 0;

    while (1)
    {
        // เวลาลดลงเรื่อยๆ
        if (Red_now > 0 && led == LED_RED)
            Red_now-=1;

        // เริ่มมายังไม่ส่งข้อมูล
        int Received = 0;

        // Receive
        if (led == LED_RED)
            Received = xQueueReceive(RED_QUEUE, NULL, 0);

        // Red Led increase duration
        if (Received && led == LED_RED)
            Red_now += 3;

        // Red Led Condition
        if (Red_now > 0 && led == LED_RED) // เวลายังไม่หมดให้โชว์แสง
            digitalWrite(LED_RED, 1);
        else if (led == LED_RED)
            digitalWrite(LED_RED, 0);
        Delay(1000);
    }
}

void yellowTaskSender(void *pvParameters)
{
    unsigned short button = (unsigned short)pvParameters;
    int Button_Pressed = 0;

    pinMode(button, INPUT_PULLUP);

    while (1)
    {
        if (Button_Pressed_Check(button) && !Button_Pressed)
        {
            Button_Pressed = 1;
            xQueueSend(YELLOW_QUEUE, &button, 0);
        }
        if (!Button_Pressed_Check(button))
            Button_Pressed = 0;

        Delay(10);
    }
}

void yellowTaskReceiver(void *pvParameters)
{

    unsigned short led = (unsigned short)pvParameters;
    pinMode(led, OUTPUT);

    int Yellow_Blink_Checked = 1; // เช็คโหมดว่าโหมดซ้ำหรือไม่
    int Yellow_Blink = 1;

    while (1)
    {
        // เริ่มมาไม่มีรับค่า
        int Received = 0;
        if (led == LED_YELLOW)
            Received = xQueueReceive(YELLOW_QUEUE, NULL, 0);

        // รับค่าปุ่ม
        if (Received && led == LED_YELLOW)
            Yellow_Blink_Checked = !Yellow_Blink_Checked; // สลับโหมด

        // LED Yellow 
        if (led == LED_YELLOW){
            if (Yellow_Blink_Checked){
                digitalWrite(LED_YELLOW, !Yellow_Blink);
                Yellow_Blink = !Yellow_Blink; // สลับค่ากันเรื่อยๆ
            }
            else if(!Yellow_Blink_Checked){
                digitalWrite(LED_YELLOW, 0);
            }
            Delay(500);
        }
    }
}

void greenTaskSender(void *pvParameters)
{
    unsigned short button = (unsigned short)pvParameters;
    int Button_Pressed = 0;

    pinMode(button, INPUT_PULLUP);

    while (1)
    {
        if (Button_Pressed_Check(button) && !Button_Pressed)
        {
            Button_Pressed = 1;
            xQueueSend(GREEN_QUEUE, &button, 0);
        }

        if (!Button_Pressed_Check(button))
        {
            Button_Pressed = 0;
        }
        Delay(10);
    }
}

void greenTaskReceiver(void *pvParameters)
{
    unsigned short led = (unsigned short)pvParameters;

    pinMode(led, OUTPUT);

    unsigned short Green_now = 0;

    while (1)
    {
        // เริ่มมายังไม่ส่งข้อมูล
        int Received = 0;
        // Green Received
        if (led == LED_GREEN)
            Received = xQueueReceive(GREEN_QUEUE, NULL, 0);

        // เพิ่มจำนวน ติด(3)/ดับ(3)
        if (Received && led == LED_GREEN && Green_now == 0)
            Green_now = 6;

        if (Green_now > 0 && led == LED_GREEN){
            if(Green_now % 2 != 0){
                digitalWrite(led, 0);
            }
            else{
                digitalWrite(led, 1);
            }
            Green_now -= 1;
            Delay(500);
        }
    }
}

void loop() {}