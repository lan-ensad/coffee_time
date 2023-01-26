#if defined(ESP32)
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32"
#elif defined(ESP8266)
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#define DEVICE "ESP8266"
#endif

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASS";
String hostname = "coffeTime";
uint8_t newMac[] = { 0xE8, 0x62, 0xE7, 0x23, 0x0A, 0x27 };

#define INFLUXDB_URL "IP_ADDRESS:8086"
#define INFLUXDB_TOKEN "LONG_STRING_TOKEN"
#define INFLUXDB_ORG "ID_ORGA"
#define INFLUXDB_BUCKET "BUCKET"
#define TZ_INFO "CET-1CEST,M3.5.0,M10.5.0/3"

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

Point sensor("coffee_machine");

const int ASS = 36;  //Watter Sensor
const int AS = 20;   //array size
const int THRESHOLD = 3;
const int BUFFERDEL = 2;  //delay befor fill each element in buff[]
int state, prevstate;
int buff1[AS];
int buff2[AS];

//————————————————————————————————
//           FUNCTIONS

//Fill 2 arrays
void FillBuffer() {
  for (int i = 0; i < AS; i++) {
    buff2[i] = buff1[i];
  }
  for (int i = 0; i < AS; i++) {
    buff1[i] = digitalRead(ASS);
    delay(BUFFERDEL);
  }
}
//Compare the 2 arrays
int IsTurning() {
  int val = 0;
  //Compare 1 arrays
  for (int i = 0; i < AS; i++) {
    if (buff1[i] != buff2[i]) {
      val++;
    }
  }
  //returning
  if (val > THRESHOLD) {
    return true;
  } else {
    return false;
  }
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(hostname.c_str());  //define hostname
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("WiFi Connected !");
}

void setup() {
  Serial.begin(115200);
  pinMode(ASS, INPUT);

  initWiFi();

  // Add tags
  sensor.addTag("device", DEVICE);

  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}


void loop() {
  sensor.clearFields();
  FillBuffer();

  sensor.addField("coffee_machine", IsTurning());

  Serial.print("Writing: ");
  Serial.println(sensor.toLineProtocol());
  // Serial.print("Value : ");
  // Serial.println(IsTurning());

  if (!client.writePoint(sensor)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  Serial.println("Wait 10s");
  delay(10000);
}
