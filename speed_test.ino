int v1;           // 첫 번째 센서의 순간 속도
int v2;           // 두 번째 센서의 순간 속도
int vmax1;        // 첫 번째 센서의 최대 속도
int vmax2;        // 두 번째 센서의 최대 속도
int averageSpeed; // 두 센서의 평균 속도
int maxSpeed;     // 최대 속도
int Sigpin1 = 11; // 첫 번째 센서의 신호 입력 핀
int Sigpin2 = 12; // 두 번째 센서의 신호 입력 핀

void setup() {
  Serial.begin(9600);
  pinMode(Sigpin1, INPUT);
  pinMode(Sigpin2, INPUT);
}

void loop() {
  unsigned long T1, T2;   // 주기
  double f1, f2;          // 주파수
  char s[20];             // Serial 출력 Length
  vmax1 = vmax2 = 0;

  // 첫 번째 센서 측정
  while (digitalRead(Sigpin1));
  while (!digitalRead(Sigpin1));
  T1 = pulseIn(Sigpin1, HIGH) + pulseIn(Sigpin1, LOW); // 주기 측정
  f1 = 1 / (double)T1;                                // 주파수 측정
  v1 = int((f1 * 1e6) / 44.0);                         // 속도 측정
  vmax1 = max(v1, vmax1);                              // 첫 번째 센서의 최대 속도 갱신

  // 두 번째 센서 측정
  while (digitalRead(Sigpin2));
  while (!digitalRead(Sigpin2));
  T2 = pulseIn(Sigpin2, HIGH) + pulseIn(Sigpin2, LOW); // 주기 측정
  f2 = 1 / (double)T2;                                // 주파수 측정
  v2 = int((f2 * 1e6) / 44.0);                         // 속도 측정
  vmax2 = max(v2, vmax2);                              // 두 번째 센서의 최대 속도 갱신

  // 두 센서의 평균 속도 계산
  averageSpeed = (vmax1 + vmax2) / 2;

  // 최대 속도 결정
  maxSpeed = max(vmax1, vmax2, averageSpeed);

  // 최대 속도 출력
  sprintf(s, "Max Speed: %3d km/h", maxSpeed);
  Serial.println(s); // Serial 출력
  delay(500);        // Delay 500m/s
}
