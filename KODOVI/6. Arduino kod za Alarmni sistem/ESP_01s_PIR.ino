#define BLYNK_TEMPLATE_ID "TMPL4bkC0Urwt"
#define BLYNK_TEMPLATE_NAME "Detektor"
#define BLYNK_AUTH_TOKEN "n8ywpFle_Q_qGkZOlvy6rkipkpc2eehZ"
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>  // Promjena iz ESP32 na ESP8266

// WiFi podaci
char ssid[] = "WiFi-6C4E34-2.4G";
char pass[] = "aaaaaaaa";

// Pinovi
int relay1 = 4;  // GPIO0 (ESP-01S) ili GPIO2, zavisi koji pin koristiš za relej
const int motionsensor = 5;  // GPIO2 (ESP-01S) za PIR senzor

int pinStateCurrent = LOW;   // Trenutno stanje pina
int pinStatePrevious = LOW;  // Prethodno stanje pina
int status1 = LOW;
 
BlynkTimer timer;

// Ova funkcija se poziva svaki put kada se uređaj poveže na Blynk Cloud
BLYNK_CONNECTED() {
  Blynk.syncVirtual(V0);  // Sinhronizuje V0
  Blynk.syncVirtual(V1);  // Sinhronizuje V1
}

void setup() {
  // Debug konzola
  Serial.begin(115200);  // Inicijalizacija serijske veze

  // Povezivanje sa Blynk platformom
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(motionsensor, INPUT);  // Postavi PIR pin kao ulazni
  pinMode(relay1, OUTPUT);       // Postavi GPIO pin za relej kao izlazni
}

void loop() {
  Blynk.run();
  timer.run();

  pinStatePrevious = pinStateCurrent;  // Spremi prethodno stanje
  pinStateCurrent = digitalRead(motionsensor);  // Čitaj trenutno stanje PIR senzora

  if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {  // Promjena stanja: LOW -> HIGH (detektovan pokret)
    Serial.println("Motion detected!");
    digitalWrite(relay1, HIGH);  // Aktiviraj relej
    Blynk.virtualWrite(V0, 1);   // Pošaljite 1 na V0 (aktivacija)
    Blynk.virtualWrite(V1, "Motion Detected");  // Pošaljite obavijest na V1
    Blynk.logEvent("detekcija_pokreta","Detekcija pokreta");
    delay(3000);
  }
  else if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {  // Promjena stanja: HIGH -> LOW (pokret prestao)
    Serial.println("Motion stopped!");
    digitalWrite(relay1, LOW);  // Deaktiviraj relej
    Blynk.virtualWrite(V0, 0);   // Pošaljite 0 na V0 (deaktivacija)
    Blynk.virtualWrite(V1, "Motion Stopped");  // Pošaljite obavijest na V1
  }
}