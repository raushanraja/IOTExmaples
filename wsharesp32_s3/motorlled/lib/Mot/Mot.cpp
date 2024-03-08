#include <Arduino.h>
#include <led.h>
#include <Mot.h>

static uint8_t latch_state;
Controller::Controller(void) {}

void Controller::enable(void)
{
    Serial.println("Controller enable Method");

    pinMode(MOTORLATCH, OUTPUT);
    pinMode(MOTORCLK, OUTPUT);
    pinMode(MOTORENABLE, OUTPUT);
    pinMode(MOTORDATA, OUTPUT);

    Serial.println("Latch, Clock, Enable, Data pins set to output");
    Serial.println("Current state of latch: " + String(latch_state));

    Serial.println("Enabling motor controller by setting enable pin to LOW");
    digitalWrite(MOTORENABLE, LOW);
}

void Controller::latch_tx(void)
{
    Serial.println("Latch TX Method");
    uint8_t i;

    digitalWrite(MOTORLATCH, LOW);
    digitalWrite(MOTORDATA, LOW);

    for (i = 0; i < 8; i++)
    {
        Serial.println("Latch state: " + String(latch_state));
        digitalWrite(MOTORCLK, LOW);

        if (latch_state & (1 << (7 - i)))
        {
            Serial.print("Setting data pin to HIGH Converting ");
            Serial.print(7 - i);
            Serial.print(" to binary: 0x");
            Serial.println(latch_state & (1 << (7 - i)), HEX);
            digitalWrite(MOTORDATA, HIGH);
        }
        else
        {
            Serial.print("Setting data pin to LOW Converting ");
            Serial.print(7 - i);
            Serial.print(" to binary: 0x");
            Serial.println(latch_state & (1 << (7 - i)), HEX);
            digitalWrite(MOTORDATA, LOW);
        }

        Serial.println("Setting clock pin to HIGH After setting data pin");
        digitalWrite(MOTORCLK, HIGH);
    }
    Serial.println("Setting latch pin to HIGH After setting clock pin");
    digitalWrite(MOTORLATCH, HIGH);
}

static Controller MC; // This is the motor controller object

////////////////////////////////////////// Motor Class
Motor::Motor(uint8_t num)
{
    motornum = num;
}

void Motor::start(void)
{
    Serial.println("Motor Enable Method Called");
    MC.enable();

    switch (motornum)
    {
    case 1:
        latch_state &= ~(1 << MOTOR1_A) & ~(1 << MOTOR1_B);
        MC.latch_tx();
        break;
    case 2:
        latch_state &= ~(1 << MOTOR2_A) & ~(1 << MOTOR2_B);
        MC.latch_tx();
        break;
    case 3:
        latch_state &= ~(1 << MOTOR3_A) & ~(1 << MOTOR3_B);
        MC.latch_tx();
        break;
    case 4:
        latch_state &= ~(1 << MOTOR4_A) & ~(1 << MOTOR4_B);
        MC.latch_tx();
        break;
    default:
        break;
    }
}

void Motor::run(uint8_t cmd)
{
    delay(30);
    uint8_t a, b;
    switch (motornum)
    {
    case 1:
        Serial.println("Motor 1 Run Method Called");
        a = MOTOR1_A;
        b = MOTOR1_B;
        break;
    case 2:
        Serial.println("Motor 2 Run Method Called");
        a = MOTOR2_A;
        b = MOTOR2_B;
        break;
    case 3:
        Serial.println("Motor 3 Run Method Called");
        a = MOTOR3_A;
        b = MOTOR3_B;
        break;
    case 4:
        Serial.println("Motor 4 Run Method Called");
        a = MOTOR4_A;
        b = MOTOR4_B;
        break;
    default:
        break;
    }
    Serial.println("Current state of latch: " + String(latch_state));
    Serial.println("Moro num: " + String(motornum));
    Serial.println("A: " + String(a) + " B: " + String(b));

    switch (cmd)
    {
    case FORWARD:
        latch_state |= (1 << a);
        latch_state &= ~(1 << b);
        MC.latch_tx();
        Serial.println("Motor Forward Method Called" + String(latch_state));
        break;
    case BACKWARD:
        latch_state &= ~(1 << a);
        latch_state |= (1 << b);
        MC.latch_tx();
        Serial.println("Motor Backward Method Called" + String(latch_state));
        break;
    case RELEASE:
        latch_state &= ~(1 << a);
        latch_state &= ~(1 << b);
        MC.latch_tx();
        Serial.println("Motor Release Method Called" + String(latch_state));
        break;
    }
}

void Motor::setSpeed(uint8_t speed)
{
    switch (motornum)
    {
    case 1:
        Serial.println("Motor 1 Set Speed Method Called" + String(speed));
        ledcAnalogWrite1(LEDC_CHANNEL_0, speed);
        break;
    case 2:
        Serial.println("Motor 2 Set Speed Method Called" + String(speed));
        ledcAnalogWrite2(LEDC_CHANNEL_1, speed);
        break;
    case 3:
        Serial.println("Motor 3 Set Speed Method Called" + String(speed));
        ledcAnalogWrite3(LEDC_CHANNEL_2, speed);
        break;
    case 4:
        Serial.println("Motor 4 Set Speed Method Called" + String(speed));
        ledcAnalogWrite4(LEDC_CHANNEL_3, speed);
        break;
    default:
        break;
    }
}