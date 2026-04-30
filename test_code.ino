// This code  moves the bot forward and backword for 5 rotations
// Remember to edit the CPR 

// ---------------- MOTOR PINS ----------------
const int L_IN1 = 11;
const int L_IN2 = 10;
const int R_IN1 = 9;
const int R_IN2 = 6;

// ---------------- ENCODER PINS ----------------
const int ENC_R_C1 = 3;  // INT1
const int ENC_R_C2 = 5;

const int ENC_L_C1 = 2;  // INT0 (only working)

// ---------------- VARIABLES ----------------
volatile long ticksRight = 0;
volatile long ticksLeft = 0;

// ---- SET THIS AFTER CALIBRATION ----
#define CPR 650   // adjust after testing

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(9600);

  pinMode(L_IN1, OUTPUT);
  pinMode(L_IN2, OUTPUT);
  pinMode(R_IN1, OUTPUT);
  pinMode(R_IN2, OUTPUT);

  pinMode(ENC_R_C1, INPUT);
  pinMode(ENC_R_C2, INPUT);
  pinMode(ENC_L_C1, INPUT);

  // Interrupts
  attachInterrupt(digitalPinToInterrupt(ENC_R_C1), rightEncoderISR, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_L_C1), leftEncoderISR, RISING);
  delay(3000);
}

// ---------------- ISR FUNCTIONS ----------------

// Right motor → direction + ticks
void rightEncoderISR() {
  int c2 = digitalRead(ENC_R_C2);

  if (c2 == HIGH) {
    ticksRight++;   // forward


  } else {
    ticksRight--;   // backward
  }
}

// Left motor → ticks only
void leftEncoderISR() {
  ticksLeft++;
}

// ---------------- MOTOR CONTROL ----------------
void moveForward(int speedVal) {
  analogWrite(L_IN1, speedVal);
  analogWrite(L_IN2, 0);

  analogWrite(R_IN1, speedVal);
  analogWrite(R_IN2, 0);
}

void moveBackward(int speedVal) {
  analogWrite(L_IN1, 0);
  analogWrite(L_IN2, speedVal);

  analogWrite(R_IN1, 0);
  analogWrite(R_IN2, speedVal);
}

void stopMotors() {
  analogWrite(L_IN1, 0);
  analogWrite(L_IN2, 0);
  analogWrite(R_IN1, 0);
  analogWrite(R_IN2, 0);
}

// ---------------- MOVE EXACT ROTATIONS ----------------
void moveRotations(float rotations, int speedVal, bool forward) {

  ticksRight = 0;
  ticksLeft = 0;

  long targetTicks = rotations * CPR;

  if (forward) moveForward(speedVal);
  else moveBackward(speedVal);

  while (abs(ticksRight) < targetTicks) {

    Serial.print("Right ticks: ");
    Serial.print(ticksRight);

    Serial.print(" | Left ticks: ");
    Serial.print(ticksLeft);

    Serial.print(" | Target: ");
    Serial.println(targetTicks);

    delay(10);
  }

  stopMotors();
}

// ---------------- LOOP ----------------
void loop() {

  moveRotations(5, 255, true);   // forward 5 rotations
  delay(2000);

  moveRotations(5, 255, false);  // backward 5 rotations
  delay(5000);
}
