#include <inttypes.h>

#define LEDC_CHANNEL_0 0
#define LEDC_CHANNEL_1 1
#define LEDC_CHANNEL_2 2
#define LEDC_CHANNEL_3 3
#define LEDC_TIMER_12_BIT 12
#define LEDC_BASE_FREQ 5000

#define MOTOR1_EN GPIO_NUM_11
#define MOTOR2_EN GPIO_NUM_3
#define MOTOR3_EN GPIO_NUM_6
#define MOTOR4_EN GPIO_NUM_5

#define NUM_LEDS 2
#define DATA_PIN GPIO_NUM_10



void LEDSetup();
void LEDRed();
void LEDGreen();
void LEDBlue();
void LEDYellow();
void ledcAnalogSetup1();
void ledcAnalogSetup2();
void ledcAnalogSetup3();
void ledcAnalogSetup4();
void ledcAnalogWrite1(uint8_t channel, uint32_t value, uint32_t valueMax = 255);
void ledcAnalogWrite2(uint8_t channel, uint32_t value, uint32_t valueMax = 255);
void ledcAnalogWrite3(uint8_t channel, uint32_t value, uint32_t valueMax = 255);
void ledcAnalogWrite4(uint8_t channel, uint32_t value, uint32_t valueMax = 255);