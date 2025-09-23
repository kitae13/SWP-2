int led = 7;            // LED 핀 번호
int period = 1000;      // 초기 PWM 주기 (us 단위, 100~10000)
int duty = 0;           // 듀티 (0~100 %)

// 주기 설정 함수
void set_period(int p) {
  if (p < 100) p = 100;
  if (p > 10000) p = 10000;
  period = p;
}

// 듀티 설정 함수
void set_duty(int d) {
  if (d < 0) d = 0;
  if (d > 100) d = 100;
  duty = d;
}

// 소프트웨어 PWM: duty%에 맞게 LED ON/OFF
void softwarePWM() {
  int onTime = (period * duty) / 100;
  int offTime = period - onTime;

  if (onTime > 0) {
    digitalWrite(led, HIGH);
    delayMicroseconds(onTime);
  }
  if (offTime > 0) {
    digitalWrite(led, LOW);
    delayMicroseconds(offTime);
  }
}

void setup() {
  pinMode(led, OUTPUT);
  set_period(1000); // 기본 주기: 1ms (1000us)
  set_duty(0);      // 시작은 꺼진 상태
}

void loop() {
  // LED 밝기 증가 (0 → 100%)
  for (int d = 0; d <= 100; d++) {
    set_duty(d);
    // 각 duty에서 PWM을 잠시 유지 (부드러운 변화)
    for (int i = 0; i < 50; i++) {
      softwarePWM();
    }
  }

  // LED 밝기 감소 (100 → 0%)
  for (int d = 100; d >= 0; d--) {
    set_duty(d);
    for (int i = 0; i < 50; i++) {
      softwarePWM();
    }
  }
}
