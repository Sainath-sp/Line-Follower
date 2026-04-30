// ===== FINAL COMPETITION CODE =====

#define NUM_SENSORS 6
int sPins[NUM_SENSORS] = {A0, A1, A2, A3, A6, A7};

#define IN1 11
#define IN2 10
#define IN3 9
#define IN4 6

#define ENC_R 3   // main encoder

float Kp = 28;
float Kd = 18;
float Kp_speed = 0.4;

int baseSpeed = 150;

int minV[NUM_SENSORS], maxV[NUM_SENSORS];
int sVal[NUM_SENSORS];

float error = 0, prev_error = 0;

volatile long countR = 0;
long prevCount = 0;
unsigned long lastTime = 0;

void ISR_R() { countR++; }

void setup() {
  for (int i = 0; i < NUM_SENSORS; i++) pinMode(sPins[i], INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENC_R, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_R), ISR_R, RISING);

  calibrate();
  lastTime = millis();
}

void loop() {

  int sum = 0;
  float wSum = 0;
  int weights[6] = {-5, -3, -1, 1, 3, 5};

  for (int i = 0; i < NUM_SENSORS; i++) {
    int raw = analogRead(sPins[i]);
    sVal[i] = map(raw, minV[i], maxV[i], 0, 1000);
    sVal[i] = constrain(sVal[i], 0, 1000);
  }

  int avg = 0;
  for (int i = 0; i < NUM_SENSORS; i++) avg += sVal[i];
  avg /= NUM_SENSORS;

  if (avg > 500) {
    for (int i = 0; i < NUM_SENSORS; i++) sVal[i] = 1000 - sVal[i];
  }

  for (int i = 0; i < NUM_SENSORS; i++) {
    wSum += sVal[i] * weights[i];
    sum += sVal[i];
  }

  if (sum == 0) return;

  float pos = wSum / sum;
  error = pos;

  float correction = Kp * error + Kd * (error - prev_error);
  prev_error = error;

  int targetL = baseSpeed + correction;
  int targetR = baseSpeed - correction;

  // ===== ENCODER SPEED ASSIST =====
  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;

  long dR = countR - prevCount;
  float speedR = dR / dt;

  prevCount = countR;
  lastTime = now;

  // simple correction
  float speedError = speedR - 100; // target reference

  targetL -= Kp_speed * speedError;
  targetR += Kp_speed * speedError;

  targetL = constrain(targetL, 0, 255);
  targetR = constrain(targetR, 0, 255);

  setMotor(targetL, targetR);
}

void setMotor(int L, int R) {
  analogWrite(IN1, L);
  analogWrite(IN2, 0);
  analogWrite(IN3, R);
  analogWrite(IN4, 0);
}

void calibrate() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    minV[i] = 1023;
    maxV[i] = 0;
  }

  for (int t = 0; t < 200; t++) {
    for (int i = 0; i < NUM_SENSORS; i++) {
      int v = analogRead(sPins[i]);
      if (v < minV[i]) minV[i] = v;
      if (v > maxV[i]) maxV[i] = v;
    }
    delay(5);
  }
}
