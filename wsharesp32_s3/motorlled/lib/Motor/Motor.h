// Adafruit Motor shield library for JRC Board

#define MOTORDEBUG 1
#ifndef _JRC_AFMotor_h_
#define _JRC_AFMotor_h_


#include <inttypes.h>

#if defined(__AVR__)
#include <avr/io.h>

// #define MOTORDEBUG 1

#define MICROSTEPS 16 // 8 or 16

#define MOTOR12_64KHZ _BV(CS20)            // no prescale
#define MOTOR12_8KHZ _BV(CS21)             // divide by 8
#define MOTOR12_2KHZ _BV(CS21) | _BV(CS20) // divide by 32
#define MOTOR12_1KHZ _BV(CS22)             // divide by 64

#define MOTOR34_64KHZ _BV(CS00)            // no prescale
#define MOTOR34_8KHZ _BV(CS01)             // divide by 8
#define MOTOR34_1KHZ _BV(CS01) | _BV(CS00) // divide by 64

#define DC_MOTOR_PWM_RATE MOTOR34_8KHZ  // PWM rate for DC motors
#define STEPPER1_PWM_RATE MOTOR12_64KHZ // PWM rate for stepper 1
#define STEPPER2_PWM_RATE MOTOR34_64KHZ // PWM rate for stepper 2

#elif defined(ESP32)

#define MICROSTEPS 16 // 8 or 16

#define MOTOR12_64KHZ 64
#define MOTOR12_8KHZ 8
#define MOTOR12_2KHZ 2
#define MOTOR12_1KHZ 1

#define MOTOR34_64KHZ 64
#define MOTOR34_8KHZ 8
#define MOTOR34_1KHZ 1

#define DC_MOTOR_PWM_RATE 5000  // PWM rate for DC motors
#define STEPPER1_PWM_RATE MOTOR12_64KHZ // PWM rate for stepper 1
#define STEPPER2_PWM_RATE MOTOR34_64KHZ // PWM rate for stepper 2


// use 12 bit precission for LEDC timer
#define LEDC_TIMER_12_BIT  12
// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     5000
// 255 Max Duty for LEDC
#define LEDC_MAX_DUTY      255

#endif


// logic pins for "PWM"
#define MOTOR1_EN GPIO_NUM_11
#define MOTOR2_EN GPIO_NUM_3
#define MOTOR3_EN GPIO_NUM_6
#define MOTOR4_EN GPIO_NUM_5


#define LEDC_CHANNEL_0 0
#define LEDC_CHANNEL_1 1
#define LEDC_CHANNEL_2 2
#define LEDC_CHANNEL_3 3
#define LEDC_TIMER_12_BIT 12
#define LEDC_BASE_FREQ 5000

// Bit positions in the 74HCT595 shift register output
#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR4_A 0
#define MOTOR4_B 6
#define MOTOR3_A 5
#define MOTOR3_B 7

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

/*
#define LATCH 4
#define LATCH_DDR DDRB
#define LATCH_PORT PORTB

#define CLK_PORT PORTD
#define CLK_DDR DDRD
#define CLK 4

#define ENABLE_PORT PORTD
#define ENABLE_DDR DDRD
#define ENABLE 7

#define SER 0
#define SER_DDR DDRB
#define SER_PORT PORTB
*/

// Arduino pin names for interface to 74HCT595 latch (Modified for JRC Board)
#define MOTORLATCH GPIO_NUM_12
#define MOTORCLK GPIO_NUM_4
#define MOTORENABLE GPIO_NUM_7 
#define MOTORDATA GPIO_NUM_8

class AFMotorController
{
public:
  AFMotorController(void);
  void enable(void);
  friend class AF_DCMotor;
  void latch_tx(void);
  uint8_t TimerInitalized;
};

class AF_DCMotor
{
public:
  AF_DCMotor(uint8_t motornum, uint8_t freq = DC_MOTOR_PWM_RATE);
  void run(uint8_t);
  void setSpeed(uint8_t);

private:
  uint8_t motornum, pwmfreq;
};

class AF_Stepper
{
public:
  AF_Stepper(uint16_t, uint8_t);
  void step(uint16_t steps, uint8_t dir, uint8_t style = SINGLE);
  void setSpeed(uint16_t);
  uint8_t onestep(uint8_t dir, uint8_t style);
  void release(void);
  uint16_t revsteps; // # steps per revolution
  uint8_t steppernum;
  uint32_t usperstep, steppingcounter;

private:
  uint8_t currentstep;
};

uint8_t getlatchstate(void);

#endif
