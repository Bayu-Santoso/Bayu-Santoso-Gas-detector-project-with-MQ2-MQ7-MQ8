#define BLYNK_PRINT Serial
#define BLYNK_MAX_SENDBYTES 128
#include <SoftwareSerial.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
BlynkTimer timer;

// ================= CONNECT TO WIFI ================
char auth[] = "TE7vkPcX8RaKQYjrdCKis5btmLkY2MRm";
char ssid[] = "WIFI";
char pass[] = "wadooooo";
int MQ2, MQ7, MQ8, mmm;

SoftwareSerial NodeMCU(D5, D6);

void a() {
  if (NodeMCU.available() > 0) {
    mmm = NodeMCU.parseInt();
    if (NodeMCU.read() == '\n') {
      Serial.println(mmm);
      if (mmm >= 2000 && mmm <= 3023) {
        int MQ2 = mmm;
        MQ2 = MQ2 - 2000;
        Serial.print(" MQ2 : ");
        Serial.println(MQ2);
        Blynk.virtualWrite(V1, MQ2);

      }
      if (mmm >= 4000 && mmm <= 5023) {
        int MQ7 = mmm;
        MQ7 = MQ7 - 4000;
        Serial.print(" MQ7 : ");
        Serial.println(MQ7);
        Blynk.virtualWrite(V2, MQ7);

      }
      if (mmm >= 6000 && mmm <= 7023) {
        int MQ8 = mmm;
        MQ8 = MQ8 - 6000;
        Serial.print(" MQ8 : ");
        Serial.println(MQ8);
        Blynk.virtualWrite(V3, MQ8);

      }
    }
  }
}

void setup() {
  Serial.begin  (115200);
  NodeMCU.begin(9600);
  Blynk.begin(auth, ssid, pass);    //Local server
  Blynk.notify("Alpha is online");
}

void loop() {
  Blynk.run();
  timer.run();
  timer.setInterval(400L, a);
}
