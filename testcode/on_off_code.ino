// ===== SIMPLE LINE FOLLOWER (NO PID) =====

#define NUM_SENSORS 6

int sPins[NUM_SENSORS] = {A0, A1, A2, A3, A6, A7};

#define IN1 11
#define IN2 10
#define IN3 9
#define IN4 6

int sVal[NUM_SENSORS];

int threshold = 500;   // adjust if needed

int speedL = 255;
int speedR = 255;

// ================== SETUP ==================
void setup() {
  for (int i = 0; i < NUM_SENSORS; i++)
    pinMode(sPins[i], INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

// ================== LOOP ==================
void loop() {

  // Read sensors
  for (int i = 0; i < NUM_SENSORS; i++) {
    sVal[i] = analogRead(sPins[i]);
  }

  // Convert to digital (line detected or not)
  int s[6];
  for (int i = 0; i < 6; i++) {
    if (sVal[i] > threshold)
      s[i] = 1;
    else
      s[i] = 0;
  }

  // ===== LOGIC =====

  // CENTER → GO STRAIGHT
  if (s[2] == 1 && s[3] == 1) {
    forward();
  }

  // LEFT SIDE → TURN LEFT
  else if (s[0] == 1 || s[1] == 1) {
    left();
  }

  // RIGHT SIDE → TURN RIGHT
  else if (s[4] == 1 || s[5] == 1) {
    right();
  }

  // LOST LINE → SEARCH
  else {
    forward();
  }
}

// ================== MOTOR FUNCTIONS ==================

void forward() {
  analogWrite(IN1, speedL);
  analogWrite(IN2, 0);

  analogWrite(IN3, speedR);
  analogWrite(IN4, 0);
}

void left() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 120);

  analogWrite(IN3, 150);
  analogWrite(IN4, 0);
}

void right() {
  analogWrite(IN1, 150);
  analogWrite(IN2, 0);

  analogWrite(IN3, 0);
  analogWrite(IN4, 120);
}



