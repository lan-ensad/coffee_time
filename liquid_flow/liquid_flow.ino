const int AS = 10;  //array size
const int THRESHOLD = 2;
const int BUFFERDEL = 5;  //delay befor fill each element in buff[]
const int SENS = 2;
int state, prevstate;
int buff[AS];

void setup() {
  pinMode(SENS, INPUT);
  Serial.begin(115200);
  Serial.println("GOOD");
}

void loop() {
  FillBuffer();
  Serial.println(IsTurning());
  delay(100);
}

void FillBuffer() {
  for (int i = 0; i < AS; i++) {
    buff[i] = digitalRead(SENS);
    delay(BUFFERDEL);
  }
}

int IsTurning() {
  int val = 0;
  int sum = 0;
  for (int i = 0; i < AS; i++) {
    sum += buff[i];
  }
  if (sum > 0 + THRESHOLD && sum < AS - THRESHOLD) {
    val = 1;
  } else {
    val = 0;
  }
  return val;
}
