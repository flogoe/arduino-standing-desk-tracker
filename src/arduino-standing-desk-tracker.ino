// PINS
const int trigger = 7;
const int echo = 6;
const int piezo = 9;
const int redLED = 4;
const int yellowLED = 3;
const int greenLED = 2;

// Variables
const int standingMIN = 90;           // in cm
const int standingMAX = 110;          // in cm
const int maxSittingTime = 2700;      // in seconds
const int standingWarningTime = 3600; // in seconds

bool alarm = false;
long duration = 0;
long distance = 0;

// Sitting

long sittingTimer = 0;
long standingTimer = 0;

int runningInterval = 1000;
bool isStanding = false;

void setup() {
  Serial.begin(9600);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(piezo, OUTPUT);
}

void loop() {

  measureDistance();

  if (isStanding) {
    trackStandingTime();
    resetSittingTimer();
  } else {
    checkSittingTime();
    resetStandingTimer();
  }

  delay(runningInterval);
}

void measureDistance() {
  digitalWrite(trigger, LOW);
  delay(5);
  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration / 2) * 0.03432;

  if (distance >= standingMIN && distance <= standingMAX) {
    isStanding = true;
  } else {
    isStanding = false;
  }
}

bool checkSittingTime() {
  if (sittingTimer < maxSittingTime) {
    yellowOn();
    sittingTimer += 1;
    return true;
  } else {
    alarmOn();
    return false;
  }
}

void trackStandingTime() {
  if (standingTimer <= standingWarningTime) {
    greenOn();
    standingTimer++;
  } else {
    greenBlink();
    standingTimer++;
  }
}

void greenOn() {
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, HIGH);
}

void yellowOn() {
  digitalWrite(yellowLED, HIGH);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
}

void redOn() {
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);
}

void allLEDoff() {
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
}

void alarmOn() {
  redOn();
  tone(piezo, 400);
  delay(1000);
  allLEDoff();
  noTone(piezo);
}

void greenBlink() {
  greenOn();
  delay(2000);
  allLEDoff();
  delay(2000);
}

void resetSittingTimer() {
  if (standingTimer >= 3) {
    sittingTimer = 0;
  }
}

void resetStandingTimer() {
  if (sittingTimer >= 3) {
    standingTimer = 0;
  }
}

void debug() {
  Serial.println("=== Dubugging ===");
  Serial.print("Desk height: ");
  Serial.print(distance);
  Serial.println("cm");

  Serial.print("isStanding: ");
  Serial.println(isStanding);

  Serial.print("StandingTimer: ");
  Serial.println(standingTimer);

  Serial.print("SittingTimer: ");
  Serial.println(sittingTimer);

  Serial.println("");
  Serial.println("");
}
