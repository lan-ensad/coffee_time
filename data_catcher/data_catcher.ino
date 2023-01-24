/*
====================
  Reset datas.txt
====================
* -> arduino == 1.8.19
* -> open esp32_filesystem
* -> Tools => ESP32 Sketch Data Upload
*
* -> open data_catcher
* -> upload

RTC Mapping : 
  SDA => IO4
  SCL => IO5
*/

#include "SPIFFS.h"
#include "RTClib.h"

RTC_PCF8523 rtc;

char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

int t = 21;
const int ASS = 5;

const int AS = 10;  //array size
const int THRESHOLD = 2;
const int BUFFERDEL = 5;  //delay befor fill each element in buff[]
int state, prevstate;
int buff[AS];

String format(unsigned long t) {
  String r;
  if (t < 10) {
    r = "0" + String(t);
  } else {
    r = String(t);
  }
  return String(r);
}

void setup() {
  Serial.begin(115200);

  pinMode(ASS, INPUT);

  analogReadResolution(12);
  analogSetPinAttenuation(ASS, ADC_6db);

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (!rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  rtc.start();
  float drift = 43;
  float period_sec = (7 * 86400);
  float deviation_ppm = (drift / period_sec * 1000000);
  float drift_unit = 4.34;
  int offset = round(deviation_ppm / drift_unit);
  Serial.print("Offset is ");
  Serial.println(offset);
}

void loop() {
  //————————————————————————————————
  //          PREPARE LOG
  FillBuffer();
  DateTime now = rtc.now();

  String log = format(now.year()) + "/" + format(now.month()) + "/" + format(now.day()) + ";" + format(now.hour()) + ":" + format(now.minute()) + ":" + format(now.second()) + " ; " + String(IsTurning());

  //————————————————————————————————
  //           WRITE FILE
  if (IsTurning()) {
    File file = SPIFFS.open("/datas.txt", FILE_APPEND);
    if (!file) {
      Serial.println("There was an error opening the file for writing");
      return;
    }
    if (file.println() && file.print(log)) {
      Serial.println("File was written : " + log);
    } else {
      Serial.println("File write failed");
    }
    file.close();
    delay(5000);
  }
  delay(500);
}
//————————————————————————————————
//           FUNCTIONS
void FillBuffer() {
  for (int i = 0; i < AS; i++) {
    buff[i] = digitalRead(ASS);
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