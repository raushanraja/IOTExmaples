#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#if defined(__AVR__)
#include <avr/io.h>
#endif
#include "WProgram.h"
#endif


#include "Motor.h" 
static uint8_t latch_state;

#if (MICROSTEPS == 8)
uint8_t microstepcurve[] = {0, 50, 98, 142, 180, 212, 236, 250, 255};
#elif (MICROSTEPS == 16)
uint8_t microstepcurve[] = {0, 25, 50, 74, 98, 120, 141, 162, 180, 197, 212, 225, 236, 244, 250, 253, 255};
#endif




void ledcAnalogWrite1(uint32_t value) {
  uint32_t maxValue = 255;
  uint32_t duty = (4095 / maxValue) * min(value, maxValue);
  Serial.printf("duty: %d\n", duty);
  ledcWrite(LEDC_CHANNEL_0, duty);
}



void ledcAnalogWrite2(uint32_t value) {
  uint32_t maxValue = 255;
  uint32_t duty = (4095 / maxValue) * min(value, maxValue);
  Serial.printf("duty: %d\n", duty);
  ledcWrite(LEDC_CHANNEL_1, duty);
}


void ledcAnalogWrite3(uint32_t value) {
  uint32_t maxValue = 255;
  uint32_t duty = (4095 / maxValue) * min(value, maxValue);
  Serial.printf("duty: %d\n", duty);
  ledcWrite(LEDC_CHANNEL_2, duty);
}


void ledcAnalogWrite4(uint32_t value) {
  uint32_t maxValue = 255;
  uint32_t duty = (4095 / maxValue) * min(value, maxValue);
  Serial.printf("duty: %d\n", duty);
  ledcWrite(LEDC_CHANNEL_3, duty);
}

AFMotorController::AFMotorController(void)
{
  TimerInitalized = false;
}

void AFMotorController::enable(void)
{
  // setup the latch
  /*
  LATCH_DDR |= _BV(LATCH);
  ENABLE_DDR |= _BV(ENABLE);
  CLK_DDR |= _BV(CLK);
  SER_DDR |= _BV(SER);
  */
  pinMode(MOTORLATCH, OUTPUT);
  pinMode(MOTORENABLE, OUTPUT);
  pinMode(MOTORDATA, OUTPUT);
  pinMode(MOTORCLK, OUTPUT);

  latch_state = 0;

  latch_tx(); // "reset"

  // ENABLE_PORT &= ~_BV(ENABLE); // enable the chip outputs!
  digitalWrite(MOTORENABLE, LOW);
}

void AFMotorController::latch_tx(void)
{
  uint8_t i;

  // LATCH_PORT &= ~_BV(LATCH);
  digitalWrite(MOTORLATCH, LOW);

  // SER_PORT &= ~_BV(SER);
  digitalWrite(MOTORDATA, LOW);

  for (i = 0; i < 8; i++)
  {
    // CLK_PORT &= ~_BV(CLK);
    digitalWrite(MOTORCLK, LOW);

    if (latch_state & _BV(7 - i))
    {
      // SER_PORT |= _BV(SER);
      digitalWrite(MOTORDATA, HIGH);
    }
    else
    {
      // SER_PORT &= ~_BV(SER);
      digitalWrite(MOTORDATA, LOW);
    }
    // CLK_PORT |= _BV(CLK);
    digitalWrite(MOTORCLK, HIGH);
  }
  // LATCH_PORT |= _BV(LATCH);
  digitalWrite(MOTORLATCH, HIGH);
}

static AFMotorController MC;

/******************************************
               MOTORS
******************************************/
AF_DCMotor::AF_DCMotor(uint8_t num, uint8_t freq)
{
  motornum = num;
  pwmfreq = freq;

  MC.enable();

  switch (num)
  {
  case 1:
    latch_state &= ~_BV(MOTOR1_A) & ~_BV(MOTOR1_B); // set both motor pins to 0
    MC.latch_tx();
    break;
  case 2:
    latch_state &= ~_BV(MOTOR2_A) & ~_BV(MOTOR2_B); // set both motor pins to 0
    MC.latch_tx();
    break;
  case 3:
    latch_state &= ~_BV(MOTOR3_A) & ~_BV(MOTOR3_B); // set both motor pins to 0
    MC.latch_tx();
    break;
  case 4:
    latch_state &= ~_BV(MOTOR4_A) & ~_BV(MOTOR4_B); // set both motor pins to 0
    MC.latch_tx();
    break;
  }
}

void AF_DCMotor::run(uint8_t cmd)
{
  uint8_t a, b;
  switch (motornum)
  {
  case 1:
    a = MOTOR1_A;
    b = MOTOR1_B;
    break;
  case 2:
    a = MOTOR2_A;
    b = MOTOR2_B;
    break;
  case 3:
    a = MOTOR3_A;
    b = MOTOR3_B;
    break;
  case 4:
    a = MOTOR4_A;
    b = MOTOR4_B;
    break;
  default:
    return;
  }

  switch (cmd)
  {
  case FORWARD:
    latch_state |= _BV(a);
    latch_state &= ~_BV(b);
    MC.latch_tx();
    break;
  case BACKWARD:
    latch_state &= ~_BV(a);
    latch_state |= _BV(b);
    MC.latch_tx();
    break;
  case RELEASE:
    latch_state &= ~_BV(a); // A and B both low
    latch_state &= ~_BV(b);
    MC.latch_tx();
    break;
  }
}

void AF_DCMotor::setSpeed(uint8_t speed)
{
  switch (motornum)
  {
  case 1:
  ledcAnalogWrite1(speed);
    break;
  case 2:
  ledcAnalogWrite2(speed);
    break;
  case 3:
  ledcAnalogWrite3(speed);
    break;
  case 4:
  ledcAnalogWrite4(speed);
    break;
  }
}

/******************************************
               STEPPERS
******************************************/

AF_Stepper::AF_Stepper(uint16_t steps, uint8_t num)
{
  MC.enable();

  revsteps = steps;
  steppernum = num;
  currentstep = 0;

  if (steppernum == 1)
  {
    latch_state &= ~_BV(MOTOR1_A) & ~_BV(MOTOR1_B) &
                   ~_BV(MOTOR2_A) & ~_BV(MOTOR2_B); // all motor pins to 0
    MC.latch_tx();

    // enable both H bridges
    pinMode(11, OUTPUT);
    pinMode(3, OUTPUT);
    digitalWrite(11, HIGH);
    digitalWrite(3, HIGH);

    // use PWM for microstepping support
  }
  else if (steppernum == 2)
  {
    latch_state &= ~_BV(MOTOR3_A) & ~_BV(MOTOR3_B) &
                   ~_BV(MOTOR4_A) & ~_BV(MOTOR4_B); // all motor pins to 0
    MC.latch_tx();

    // enable both H bridges
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);

    // use PWM for microstepping support
    // use PWM for microstepping support
  }
}

void AF_Stepper::setSpeed(uint16_t rpm)
{
  usperstep = 60000000 / ((uint32_t)revsteps * (uint32_t)rpm);
  steppingcounter = 0;
}

void AF_Stepper::release(void)
{
  if (steppernum == 1)
  {
    latch_state &= ~_BV(MOTOR1_A) & ~_BV(MOTOR1_B) &
                   ~_BV(MOTOR2_A) & ~_BV(MOTOR2_B); // all motor pins to 0
    MC.latch_tx();
  }
  else if (steppernum == 2)
  {
    latch_state &= ~_BV(MOTOR3_A) & ~_BV(MOTOR3_B) &
                   ~_BV(MOTOR4_A) & ~_BV(MOTOR4_B); // all motor pins to 0
    MC.latch_tx();
  }
}

void AF_Stepper::step(uint16_t steps, uint8_t dir, uint8_t style)
{
  uint32_t uspers = usperstep;
  uint8_t ret = 0;

  if (style == INTERLEAVE)
  {
    uspers /= 2;
  }
  else if (style == MICROSTEP)
  {
    uspers /= MICROSTEPS;
    steps *= MICROSTEPS;
#ifdef MOTORDEBUG
    Serial.print("steps = ");
    Serial.println(steps, DEC);
#endif
  }

  while (steps--)
  {
    ret = onestep(dir, style);
    delay(uspers / 1000); // in ms
    steppingcounter += (uspers % 1000);
    if (steppingcounter >= 1000)
    {
      delay(1);
      steppingcounter -= 1000;
    }
  }
  if (style == MICROSTEP)
  {
    while ((ret != 0) && (ret != MICROSTEPS))
    {
      ret = onestep(dir, style);
      delay(uspers / 1000); // in ms
      steppingcounter += (uspers % 1000);
      if (steppingcounter >= 1000)
      {
        delay(1);
        steppingcounter -= 1000;
      }
    }
  }
}

uint8_t AF_Stepper::onestep(uint8_t dir, uint8_t style)
{
  uint8_t a, b, c, d;
  uint8_t ocrb, ocra;

  ocra = ocrb = 255;

  if (steppernum == 1)
  {
    a = _BV(MOTOR1_A);
    b = _BV(MOTOR2_A);
    c = _BV(MOTOR1_B);
    d = _BV(MOTOR2_B);
  }
  else if (steppernum == 2)
  {
    a = _BV(MOTOR3_A);
    b = _BV(MOTOR4_A);
    c = _BV(MOTOR3_B);
    d = _BV(MOTOR4_B);
  }
  else
  {
    return 0;
  }

  // next determine what sort of stepping procedure we're up to
  if (style == SINGLE)
  {
    if ((currentstep / (MICROSTEPS / 2)) % 2)
    { // we're at an odd step, weird
      if (dir == FORWARD)
      {
        currentstep += MICROSTEPS / 2;
      }
      else
      {
        currentstep -= MICROSTEPS / 2;
      }
    }
    else
    { // go to the next even step
      if (dir == FORWARD)
      {
        currentstep += MICROSTEPS;
      }
      else
      {
        currentstep -= MICROSTEPS;
      }
    }
  }
  else if (style == DOUBLE)
  {
    if (!(currentstep / (MICROSTEPS / 2) % 2))
    { // we're at an even step, weird
      if (dir == FORWARD)
      {
        currentstep += MICROSTEPS / 2;
      }
      else
      {
        currentstep -= MICROSTEPS / 2;
      }
    }
    else
    { // go to the next odd step
      if (dir == FORWARD)
      {
        currentstep += MICROSTEPS;
      }
      else
      {
        currentstep -= MICROSTEPS;
      }
    }
  }
  else if (style == INTERLEAVE)
  {
    if (dir == FORWARD)
    {
      currentstep += MICROSTEPS / 2;
    }
    else
    {
      currentstep -= MICROSTEPS / 2;
    }
  }

  if (style == MICROSTEP)
  {
    if (dir == FORWARD)
    {
      currentstep++;
    }
    else
    {
      // BACKWARDS
      currentstep--;
    }

    currentstep += MICROSTEPS * 4;
    currentstep %= MICROSTEPS * 4;

    ocra = ocrb = 0;
    if ((currentstep >= 0) && (currentstep < MICROSTEPS))
    {
      ocra = microstepcurve[MICROSTEPS - currentstep];
      ocrb = microstepcurve[currentstep];
    }
    else if ((currentstep >= MICROSTEPS) && (currentstep < MICROSTEPS * 2))
    {
      ocra = microstepcurve[currentstep - MICROSTEPS];
      ocrb = microstepcurve[MICROSTEPS * 2 - currentstep];
    }
    else if ((currentstep >= MICROSTEPS * 2) && (currentstep < MICROSTEPS * 3))
    {
      ocra = microstepcurve[MICROSTEPS * 3 - currentstep];
      ocrb = microstepcurve[currentstep - MICROSTEPS * 2];
    }
    else if ((currentstep >= MICROSTEPS * 3) && (currentstep < MICROSTEPS * 4))
    {
      ocra = microstepcurve[currentstep - MICROSTEPS * 3];
      ocrb = microstepcurve[MICROSTEPS * 4 - currentstep];
    }
  }

  currentstep += MICROSTEPS * 4;
  currentstep %= MICROSTEPS * 4;

#ifdef MOTORDEBUG
  Serial.print("current step: ");
  Serial.println(currentstep, DEC);
  Serial.print(" pwmA = ");
  Serial.print(ocra, DEC);
  Serial.print(" pwmB = ");
  Serial.println(ocrb, DEC);
#endif

  // release all
  latch_state &= ~a & ~b & ~c & ~d; // all motor pins to 0

  // Serial.println(step, DEC);
  if (style == MICROSTEP)
  {
    if ((currentstep >= 0) && (currentstep < MICROSTEPS))
      latch_state |= a | b;
    if ((currentstep >= MICROSTEPS) && (currentstep < MICROSTEPS * 2))
      latch_state |= b | c;
    if ((currentstep >= MICROSTEPS * 2) && (currentstep < MICROSTEPS * 3))
      latch_state |= c | d;
    if ((currentstep >= MICROSTEPS * 3) && (currentstep < MICROSTEPS * 4))
      latch_state |= d | a;
  }
  else
  {
    switch (currentstep / (MICROSTEPS / 2))
    {
    case 0:
      latch_state |= a; // energize coil 1 only
      break;
    case 1:
      latch_state |= a | b; // energize coil 1+2
      break;
    case 2:
      latch_state |= b; // energize coil 2 only
      break;
    case 3:
      latch_state |= b | c; // energize coil 2+3
      break;
    case 4:
      latch_state |= c; // energize coil 3 only
      break;
    case 5:
      latch_state |= c | d; // energize coil 3+4
      break;
    case 6:
      latch_state |= d; // energize coil 4 only
      break;
    case 7:
      latch_state |= d | a; // energize coil 1+4
      break;
    }
  }

  MC.latch_tx();
  return currentstep;
}
