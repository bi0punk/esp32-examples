// Monitor cardiaco AD8232 para ESP32
// Unifica heart.ino y sketch_oct16b.ino corrigiendo inconsistencias de pines

#define LO_PLUS_PIN   8
#define LO_MINUS_PIN  9
#define ECG_PIN       A0
#define SHUTDOWN_PIN  10

#define THRESHOLD     100
#define WINDOW_MS     10000
#define MIN_PEAK_GAP  300

int count = 0;
bool flag = false;
long instance1 = 0;
double hr = 72;
double hrv = 0;
double interval = 0;
unsigned long timer = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LO_PLUS_PIN, INPUT);
  pinMode(LO_MINUS_PIN, INPUT);
  pinMode(SHUTDOWN_PIN, OUTPUT);
  digitalWrite(SHUTDOWN_PIN, LOW);
  Serial.println("[cardiaco] Monitor AD8232 listo");
}

void loop() {
  if (digitalRead(LO_PLUS_PIN) == 1 || digitalRead(LO_MINUS_PIN) == 1) {
    Serial.println("[cardiaco] Electrodos desconectados");
    digitalWrite(SHUTDOWN_PIN, LOW);
    delay(100);
    return;
  }

  digitalWrite(SHUTDOWN_PIN, HIGH);
  int raw = analogRead(ECG_PIN);
  int value = map(raw, 250, 400, 0, 100);
  value = constrain(value, 0, 100);

  if (value > THRESHOLD && !flag) {
    count++;
    flag = true;
    interval = micros() - instance1;
    instance1 = micros();
  } else if (value <= THRESHOLD) {
    flag = false;
  }

  if (millis() - timer >= WINDOW_MS) {
    hr = count * 6;
    timer = millis();
    count = 0;
  }

  hrv = (hr / 60.0) - (interval / 1000000.0);

  Serial.printf("ECG:%d HR:%.0f HRV:%.3f\n", value, hr, hrv);
  delay(1);
}
