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

// Bit positions in the 74HCT595 shift register output
#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR3_A 5
#define MOTOR3_B 7
#define MOTOR4_A 0
#define MOTOR4_B 6

// Constants that the user passes in to the motor calls
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

// Constants that the user passes in to the stepper calls
#define SINGLE 1
#define DOUBLE 2
#define INTERLEAVE 3
#define MICROSTEP 4

// Arduino pin names for interface to 74HCT595 latch (Modified for JRC Board)
#define MOTORLATCH GPIO_NUM_12
#define MOTORCLK GPIO_NUM_4
#define MOTORENABLE GPIO_NUM_7
#define MOTORDATA GPIO_NUM_8

class Controller
{
public:
    Controller(void);
    void enable(void);
    void latch_tx(void);
    friend class DCMotor;
};

class Motor
{
public:
    Motor(uint8_t cmd);
    void start(void);
    void run(uint8_t cmd);
    void setSpeed(uint8_t speed);
private:
    uint8_t motornum, pwm;
};