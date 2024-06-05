#include <FreeRTOS_AVR.h>
#include <Arduino.h>
int v1;  // 첫 번째 센서의 순간 속도
int v2;  // 두 번째 센서의 순간 속도

int averageSpeed;  // 두 센서의 평균 속도
int maxSpeed;      // 최대 속도
int Sigpin1 = 12;  // 첫 번째 센서의 신호 입력 핀
int Sigpin2 = 8;   // 두 번째 센서의 신호 입력 핀
int speed = 9;
int In1 = 5;
int In2 = 6;
unsigned long T1, T2;  // 주기
double f1, f2;         // 주파수
char s[20];            // Serial 출력 Lengt
int overSpeed = 7;
SemaphoreHandle_t sem;
SemaphoreHandle_t sem2;

static void Thread1(void* arg) {
  pinMode(Sigpin1, INPUT);

  while (1) {

    xSemaphoreTake(sem2, portMAX_DELAY);  
   
    while (digitalRead(Sigpin1));
    while (!digitalRead(Sigpin1));
    T1 = pulseIn(Sigpin1, HIGH) + pulseIn(Sigpin1, LOW);  
    f1 = 1 / (double)T1;                                  
    v1 = int((f1 * 1e6) / 44.0);
    xSemaphoreGive(sem);
    
    }
}

static void Thread2(void* arg) {
  pinMode(Sigpin2, INPUT);

  pinMode(speed, OUTPUT);
  pinMode(In1, INPUT);
  pinMode(In2, INPUT);

  while (1) {
    xSemaphoreTake(sem, portMAX_DELAY);  //세마포어를 받을 때 까지 기다린다.
    // 두 번째 센서 측정
    while (digitalRead(Sigpin2));
    while (!digitalRead(Sigpin2));
    T2 = pulseIn(Sigpin2, HIGH) + pulseIn(Sigpin2, LOW);  // 주기 측정
    f2 = 1 / (double)T2;                                  // 주파수 측정
    v2 = int((f2 * 1e6) / 44.0);                          // 속도 측정


      // 두 센서의 평균 속도 계산
      averageSpeed = (v1 + v2) / 2;

    // 최대 속도 결정
    maxSpeed = max(v1, v2);
    maxSpeed = max(maxSpeed, averageSpeed);

    sprintf(s, "Max Speed: %3d km/h", maxSpeed);
    Serial.println(s);  // Serial 출력
    if (maxSpeed >= overSpeed) {
      sprintf(s, "OverSpeed!!");
      Serial.println(s);

      digitalWrite(In1, HIGH);
      digitalWrite(In2, LOW);
      analogWrite(speed, 255);

      digitalWrite(In1, LOW);
      digitalWrite(In2, HIGH);
      analogWrite(speed, 255);
    }
    xSemaphoreGive(sem2);  //세마포어를 주어서 Thread1의 동작을 수행할 수 있게한다.
  }
}
void setup() {
  Serial.begin(9600);
  portBASE_TYPE s1, s2;
  //카운팅 세마포어를 생성한다.

  sem = xSemaphoreCreateCounting(1, 1);  //세마포어를 주고 시작함
  sem2 = xSemaphoreCreateCounting(1, 0);

  s1 = xTaskCreate(Thread1, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  s2 = xTaskCreate(Thread2, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);

  //생성중 에러 발생했는지 체크

  if ((sem == NULL || sem2 == NULL) || (s1 != pdPASS || s2 != pdPASS)) {

    Serial.println(F("Creation problem"));

    while (1);
      
  }

  //스케줄러를 시작한다.
  vTaskStartScheduler();
    //여기가 실행된다면 메모리 부족 상태
    Serial.println(F("Insufficient RAM"));

  while (1);
    
}


//------------------------------------------------------------------------------

void loop()

{

// Not used.

}
