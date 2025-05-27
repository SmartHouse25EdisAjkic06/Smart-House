#include <SPI.h>
#include <MFRC522.h>
// Definicija pinova
#define RST_PIN 9       // Pin za reset RFID modula
#define SS_PIN 10       // Pin za SDA (Slave Select)
#define IN1 5
#define IN2 6
#define otvorenaTaster 3
#define tasterOtvoriZatvori 2
#define zatvorenaTaster 4
#define ULTRAZVUCNI_TRIG 7
#define ULTRAZVUCNI_ECHO 8

// Vrijednosti udaljenosti
const int PREPREKA_UDALJENOST = 5;
bool stanje = false;
bool kartica = false;
MFRC522 rfid(SS_PIN, RST_PIN); // Kreiraj instancu RFID modula

// Očekivani UID kartice
byte expectedUID[] = {0xE3, 0x2B, 0xEF, 0x99};

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(otvorenaTaster, INPUT_PULLUP);
  pinMode(zatvorenaTaster, INPUT_PULLUP);
  pinMode(tasterOtvoriZatvori, INPUT_PULLUP);
  pinMode(ULTRAZVUCNI_TRIG, OUTPUT);
  pinMode(ULTRAZVUCNI_ECHO, INPUT);

  Serial.begin(9600);
  SPI.begin();           // Pokretanje SPI komunikacije
  rfid.PCD_Init();       // Inicijalizacija RFID čitača
  Serial.println("Priprema za čitanje RFID kartica...");
}

void loop() {
  if ((!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) && digitalRead(tasterOtvoriZatvori) != LOW) {
    return;
  }
  kartica = isUIDCorrect();
  if((kartica == true || digitalRead(tasterOtvoriZatvori) == LOW) && stanje == false) {
    otvoriKapiju();
    stanje = true;
    kartica = false;
  }
  if((kartica == true || digitalRead(tasterOtvoriZatvori) == LOW)  && stanje == true) {
    zatvoriKapiju();
    stanje = false;
    kartica = false;
  }
  // Stop rad s karticom
  rfid.PICC_HaltA();
}

void otvoriKapiju() {
  Serial.println("Otvaranje kapije...");
  digitalWrite(IN2, LOW);
  analogWrite(IN1, 200);

  // Čekaj dok taster za potpuno otvaranje ne bude aktiviran
  while (digitalRead(otvorenaTaster) == HIGH);

  zaustaviMotor();
  Serial.println("Kapija potpuno otvorena.");
}

void zatvoriKapiju() {
  Serial.println("Zatvaranje kapije...");
  digitalWrite(IN1, LOW);
  analogWrite(IN2, 200);

  // Čekaj dok taster za potpuno zatvaranje ne bude aktiviran
  while (digitalRead(zatvorenaTaster) == HIGH) ;
  /*{ 
    if (detekcijaPrepreke()) {
      otvoriPrisilno();
      return; }
  } */
  zaustaviMotor();
  Serial.println("Kapija potpuno zatvorena.");
}

void otvoriPrisilno() {
  Serial.println("Prisilno otvaranje kapije zbog prepreke...");
  digitalWrite(IN2, LOW);
  analogWrite(IN1, 100);

  // Čekaj dok taster za potpuno otvaranje ne bude aktiviran
  while (digitalRead(otvorenaTaster) == HIGH);

  zaustaviMotor();
  stanje = true;
  Serial.println("Kapija potpuno otvorena.");
}

bool detekcijaPrepreke() {
  digitalWrite(ULTRAZVUCNI_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRAZVUCNI_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRAZVUCNI_TRIG, LOW);

  long trajanje = pulseIn(ULTRAZVUCNI_ECHO, HIGH);
  int udaljenost = trajanje * 0.034 / 2;

  if (udaljenost < PREPREKA_UDALJENOST) {
    Serial.println("Prepreka detektovana! ");
    Serial.println(udaljenost);
    return true;
  }
  return false;
} 

void zaustaviMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}
bool isUIDCorrect() {
  if (rfid.uid.size != sizeof(expectedUID)) {
    return false;
  }

  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] != expectedUID[i]) {
      return false;
    }
  }
  return true;
}
