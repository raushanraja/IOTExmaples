#include <FastLED.h>
#include <led.h>

CRGB leds[NUM_LEDS];

void LEDSetup()
{
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS); // GRB ordering is typical
    FastLED.setBrightness(255);
    leds[1] = CRGB::Gray;
}
void LEDRed()
{
    leds[0] = CRGB::Red;
    FastLED.show();
}

void LEDGreen()
{
    leds[0] = CRGB::Green;
    FastLED.show();
}

void LEDBlue()
{
    leds[0] = CRGB::Blue;
    FastLED.show();
}

void LEDYellow()
{
    leds[0] = CRGB::Yellow;
    FastLED.show();
}

void ledcAnalogSetup1()
{
    ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
    ledcAttachPin(MOTOR1_EN, LEDC_CHANNEL_0);
}

void ledcAnalogSetup2()
{
    ledcSetup(LEDC_CHANNEL_1, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
    ledcAttachPin(MOTOR2_EN, LEDC_CHANNEL_1);
}

void ledcAnalogSetup3()
{
    ledcSetup(LEDC_CHANNEL_2, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
    ledcAttachPin(MOTOR3_EN, LEDC_CHANNEL_2);
}

void ledcAnalogSetup4()
{
    ledcSetup(LEDC_CHANNEL_3, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
    ledcAttachPin(MOTOR4_EN, LEDC_CHANNEL_3);
}

void ledcAnalogWrite1(uint8_t channel, uint32_t value, uint32_t valueMax)
{
    uint32_t duty = (4095 / valueMax) * min(value, valueMax);
    Serial.println("Duty: " + String(duty));
    ledcWrite(channel, duty);
}

// 3 Similr ledcAnanlog method with 2,3,4
void ledcAnalogWrite2(uint8_t channel, uint32_t value, uint32_t valueMax)
{
    uint32_t duty = (4095 / valueMax) * min(value, valueMax);
    ledcWrite(channel, duty);
}

void ledcAnalogWrite3(uint8_t channel, uint32_t value, uint32_t valueMax)
{
    uint32_t duty = (4095 / valueMax) * min(value, valueMax);
    ledcWrite(channel, duty);
}

void ledcAnalogWrite4(uint8_t channel, uint32_t value, uint32_t valueMax)
{
    uint32_t duty = (4095 / valueMax) * min(value, valueMax);
    ledcWrite(channel, duty);
}