<<<<<<< HEAD
#define M1a 9
#define M1b 6
#define M1m 5

#define LED 13

#define Sa A2
#define Sb A3

#define SL A1

#define Gyro A0

#define Photo A6

#define Intvl  10000L  // =10ms (センサの応答50Hzの1/2)
#define DPV    -1.0    // センサ出力電圧→角速度変換係数
#define Rmp1  200.0    // モータ電圧変換係数一次
#define Rmp2    3.5    // モータ電圧変換係数二次
#define DrL   100      // 傾き角度リセット間隔
#define DrR0    0.12   // 傾き角度リセットオフセット
#define DrR1    0.005  // 傾き角度リセット係数
#define Rdv     0.03   // 位置修正用角速度係数

unsigned long offset = 0;

void LEDInit()
{
  pinMode(LED, OUTPUT);
}

void LEDOnOff(bool on)
{
  digitalWrite(LED, on ? HIGH : LOW);
}

void LEDBlink(int n)
{
  for (int i = 0; i < n; i++) {
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
    delay(300);
  }
}

void SlantSensorInit()
{
  pinMode(SL, INPUT_PULLUP);
}

int getSlant()
{
  return analogRead(SL) / 900;
}

void MotorInit()
{
  pinMode(M1a, OUTPUT);
  pinMode(M1b, OUTPUT);
  // pinMode(M1m, OUTPUT);
}

void Motor(int power)
{
  static int lastPower = 0;

  if (power != lastPower) {
    if (power == 0) {
        digitalWrite(M1a, HIGH);
        digitalWrite(M1b, HIGH);
        analogWrite(M1m, 0);
    } else if (power > 0) {
      if (lastPower <= 0) {
        digitalWrite(M1a, LOW);
        digitalWrite(M1b, HIGH);
      }
      analogWrite(M1m, power);
    } else {
      if (lastPower >= 0) {
        digitalWrite(M1b, LOW);
        digitalWrite(M1a, HIGH);
      }
      analogWrite(M1m, -power);
    }
    lastPower = power;
  }
}

void calib()
{
  delay(1000);
  LEDBlink(3);
  LEDOnOff(true);
  offset = 0;
  int i;
  for (i = 0; i < 30; i++) {
    offset += analogRead(Gyro);
    delay(100);
  }
  offset /= i;
  LEDBlink(10);
}

float getGyroVal()
{
  int val = analogRead(Gyro) - offset;
  return (float)val;
}

void setup() {
  LEDInit();
  SlantSensorInit();
  MotorInit();
  calib();
}

void interval()
{
  static unsigned long lastTime = 0;
  static unsigned long lastDelay = Intvl - 3500;

  if (lastTime == 0) {
    lastTime = micros();
    delayMicroseconds(lastDelay);
  } else {
    unsigned long now = micros();
    unsigned long lastInterval = now - lastTime;
    lastTime = now;
    long delayTime = lastDelay + (Intvl - lastInterval);
    if (delayTime > 0) {
      delayMicroseconds(delayTime);
    } else {
      delayTime = 0;
    }
    lastDelay = delayTime;
  }
}

int getpos(int pos)
{
  static bool phA = 0, phB = 0;
  static int pvalA[3] = {0,0,0}, pvalB[3] = {0,0,0};
  pvalA[2] = pvalA[1]; pvalA[1] = pvalA[0]; pvalA[0] = analogRead(Sa);
  pvalB[2] = pvalB[1]; pvalB[1] = pvalB[0]; pvalB[0] = analogRead(Sb);
  bool nowphA = pvalA[0] + pvalA[1] + pvalA[2] > (phA ? 660 : 1320) ? 1 : 0;
  bool nowphB = pvalB[0] + pvalB[1] + pvalB[2] > (phB ? 660 : 1320) ? 1 : 0;
  if (nowphA != phA) {
    pos += phB ? nowphA - phA : phA - nowphA;
    phA = nowphA;
  }
  if (nowphB != phB) {
    pos += phA ? phB - nowphB : nowphB - phB;
    phB = nowphB;
  }
  return pos;
}

int calcPower(float dps, float degree)
{
  float fpower = degree * Rmp1 + dps * Rmp2;
  int power = constrain((int)fpower, -255, 255);
  return power;
}

void work() {
  static float degree = 0.0;
  static long sumPower = 0;
  static int pos = 0;

  pos = getpos(pos);
  float gyroVal = getGyroVal();
  float dps = gyroVal * DPV + pos * Rdv;
  degree += dps * 0.01; // 0.01 = 1 / (1s / 10ms)  // 積分

  if (sumPower > DrL) {
    degree += dps * DrR1 + DrR0;
    sumPower = 0;
  } else if (sumPower < -DrL) {
    degree += dps * DrR1 - DrR0;
    sumPower = 0;
  }

  int power = calcPower(dps, degree);
  sumPower += power;
  Motor(power);
}

void sleep()
{
    digitalWrite(M1a, LOW);
    digitalWrite(M1b, LOW);
    analogWrite(M1m, 0);
    while (true) {
      LEDOnOff(true);
      delay(50);
      LEDOnOff(false);
      delay(2950);
    }
}

void loop() {
  if (getSlant() == 0) {
    sleep();
  }
  work();
  interval();
}

=======
#define M1a 9
#define M1b 6
#define M1m 5

#define LED 13

#define Sa A2
#define Sb A3

#define SL A1

#define Gyro A0

#define Photo A6

#define Intvl  10000L  // =10ms (センサの応答50Hzの1/2)
#define DPV    -1.0    // センサ出力電圧→角速度変換係数
#define Rmp1  200.0    // モータ電圧変換係数一次
#define Rmp2    3.5    // モータ電圧変換係数二次
#define DrL   100      // 傾き角度リセット間隔
#define DrR0    0.12   // 傾き角度リセットオフセット
#define DrR1    0.005  // 傾き角度リセット係数
#define Rdv     0.03   // 位置修正用角速度係数

unsigned long offset = 0;

void LEDInit()
{
  pinMode(LED, OUTPUT);
}

void LEDOnOff(bool on)
{
  digitalWrite(LED, on ? HIGH : LOW);
}

void LEDBlink(int n)
{
  for (int i = 0; i < n; i++) {
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
    delay(300);
  }
}

void SlantSensorInit()
{
  pinMode(SL, INPUT_PULLUP);
}

int getSlant()
{
  return analogRead(SL) / 900;
}

void MotorInit()
{
  pinMode(M1a, OUTPUT);
  pinMode(M1b, OUTPUT);
  // pinMode(M1m, OUTPUT);
}

void Motor(int power)
{
  static int lastPower = 0;

  if (power != lastPower) {
    if (power == 0) {
        digitalWrite(M1a, HIGH);
        digitalWrite(M1b, HIGH);
        analogWrite(M1m, 0);
    } else if (power > 0) {
      if (lastPower <= 0) {
        digitalWrite(M1a, LOW);
        digitalWrite(M1b, HIGH);
      }
      analogWrite(M1m, power);
    } else {
      if (lastPower >= 0) {
        digitalWrite(M1b, LOW);
        digitalWrite(M1a, HIGH);
      }
      analogWrite(M1m, -power);
    }
    lastPower = power;
  }
}

void calib()
{
  delay(1000);
  LEDBlink(3);
  LEDOnOff(true);
  offset = 0;
  int i;
  for (i = 0; i < 30; i++) {
    offset += analogRead(Gyro);
    delay(100);
  }
  offset /= i;
  LEDBlink(10);
}

float getGyroVal()
{
  int val = analogRead(Gyro) - offset;
  return (float)val;
}

void setup() {
  LEDInit();
  SlantSensorInit();
  MotorInit();
  calib();
}

void interval()
{
  static unsigned long lastTime = 0;
  static unsigned long lastDelay = Intvl - 3500;

  if (lastTime == 0) {
    lastTime = micros();
    delayMicroseconds(lastDelay);
  } else {
    unsigned long now = micros();
    unsigned long lastInterval = now - lastTime;
    lastTime = now;
    long delayTime = lastDelay + (Intvl - lastInterval);
    if (delayTime > 0) {
      delayMicroseconds(delayTime);
    } else {
      delayTime = 0;
    }
    lastDelay = delayTime;
  }
}

int getpos(int pos)
{
  static bool phA = 0, phB = 0;
  static int pvalA[3] = {0,0,0}, pvalB[3] = {0,0,0};
  pvalA[2] = pvalA[1]; pvalA[1] = pvalA[0]; pvalA[0] = analogRead(Sa);
  pvalB[2] = pvalB[1]; pvalB[1] = pvalB[0]; pvalB[0] = analogRead(Sb);
  bool nowphA = pvalA[0] + pvalA[1] + pvalA[2] > (phA ? 660 : 1320) ? 1 : 0;
  bool nowphB = pvalB[0] + pvalB[1] + pvalB[2] > (phB ? 660 : 1320) ? 1 : 0;
  if (nowphA != phA) {
    pos += phB ? nowphA - phA : phA - nowphA;
    phA = nowphA;
  }
  if (nowphB != phB) {
    pos += phA ? phB - nowphB : nowphB - phB;
    phB = nowphB;
  }
  return pos;
}

int calcPower(float dps, float degree)
{
  float fpower = degree * Rmp1 + dps * Rmp2;
  int power = constrain((int)fpower, -255, 255);
  return power;
}

void work() {
  static float degree = 0.0;
  static long sumPower = 0;
  static int pos = 0;

  pos = getpos(pos);
  float gyroVal = getGyroVal();
  float dps = gyroVal * DPV + pos * Rdv;
  degree += dps * 0.01; // 0.01 = 1 / (1s / 10ms)  // 積分

  if (sumPower > DrL) {
    degree += dps * DrR1 + DrR0;
    sumPower = 0;
  } else if (sumPower < -DrL) {
    degree += dps * DrR1 - DrR0;
    sumPower = 0;
  }

  int power = calcPower(dps, degree);
  sumPower += power;
  Motor(power);
}

void sleep()
{
    digitalWrite(M1a, LOW);
    digitalWrite(M1b, LOW);
    analogWrite(M1m, 0);
    while (true) {
      LEDOnOff(true);
      delay(50);
      LEDOnOff(false);
      delay(2950);
    }
}

void loop() {
  if (getSlant() == 0) {
    sleep();
  }
  work();
  interval();
}

>>>>>>> 4ec730e042cd83a222f6fb1a7c607e425648b319
