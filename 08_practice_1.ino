// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12   // sonar sensor TRIGGER
#define PIN_ECHO 13   // sonar sensor ECHO

// configurable parameters
#define SND_VEL 346.0     
#define INTERVAL 25       // 25ms 주기
#define PULSE_DURATION 10 
#define _DIST_MIN 100.0   // 100mm
#define _DIST_MAX 300.0   // 300mm

#define TIMEOUT ((INTERVAL / 2) * 1000.0) 
#define SCALE (0.001 * 0.5 * SND_VEL) 

unsigned long last_sampling_time;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);
  
  Serial.begin(115200);
}

void loop() {
  float distance;
  if (millis() < (last_sampling_time + INTERVAL))
    return;

  distance = USS_measure(PIN_TRIG, PIN_ECHO);

  int duty = 255; // 기본: 꺼짐

  if ((distance >= _DIST_MIN) && (distance <= _DIST_MAX)) {
    float ratio;
    if (distance <= 200.0) {
      ratio = (distance - 100.0) / 100.0;   // 100→0, 200→1
    } else {
      ratio = (300.0 - distance) / 100.0;   // 200→1, 300→0
    }
    duty = (int)(255 * (1.0 - ratio)); // active low
  }

  analogWrite(PIN_LED, duty);

  // Debug 출력
  Serial.print("distance: "); Serial.print(distance);
  Serial.print(" mm, duty: "); Serial.println(duty);

  last_sampling_time += INTERVAL;
}

float USS_measure(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}
