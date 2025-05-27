#include <Keypad.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definicija OLED displeja
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1  // Reset pin (nije potreban, spojen na GND)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define ZakljucajVani 10
#define ZakljucajUnutra 11
#define OtkljucajUnutra 12
#define Brava 13
// Definicija veličine tastature
const byte ROWS = 4; // Broj redova
const byte COLS = 4; // Broj kolona
// Mapiranje tipki na tastaturi
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Povezivanje redova i kolona sa Arduino pinovima
byte rowPins[ROWS] = {9, 8, 7, 6}; // Arduino pinovi za redove
byte colPins[COLS] = {5, 4, 3, 2}; // Arduino pinovi za kolone

// Kreiranje objekta tastature
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String enteredCode = "";          // Trenutno unesena šifra
String enteredCode1 = "";
String correctCode = "2468A";     // Tačna šifra
void prikaziSTS() {
  int finalX = (SCREEN_WIDTH - 12 * 10) / 2; // Centrirana pozicija
  for (int x = -135; x <=finalX-8; x++) { // Animacija ulaska s leve strane
    display.clearDisplay();
    display.setTextSize(2); // Veći font za tekst
    display.setCursor(x, 2); // Vertikalna pozicija teksta
    display.print("STS Bugojno");
    display.display();
    delay(2); // Brzina animacije
  }
  delay(2000); // Prikaz centriranog teksta na 2 sekunde
}

void prikaziIVE() {
  for (int x = SCREEN_WIDTH-60; x >= -40; x--) { // Animacija
      display.clearDisplay();
    display.setTextSize(2); // Veći font za animaciju
    display.setCursor(x, 10); // Tekst "IV E" centriran vertikalno
    display.print("IV E");
    display.display();
    delay(2); // Brzina animacije
  }
}
void prikaziGodinu() {
  for (int x =14 ; x >= -40; x--) { // Animacija s desna na levo
    display.clearDisplay();
    display.setTextSize(2); // Veličina fonta
    display.setCursor(SCREEN_WIDTH+60, x); // Vertikalna pozicija teksta
    display.print("2024/2025");
    display.display();
    delay(20); // Brzina animacije
  }
}
void setup() {
  pinMode(ZakljucajVani,INPUT_PULLUP);
  pinMode(ZakljucajUnutra,INPUT_PULLUP);
  pinMode(OtkljucajUnutra,INPUT_PULLUP);
  pinMode(Brava,OUTPUT);
  Serial.begin(9600); // Pokretanje serijske komunikacije
  Serial.println("Pokretanje sistema...");
if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
 
  display.clearDisplay();
  display.setTextSize(2);      // Veličina fonta
  display.setTextColor(SSD1306_WHITE); // Boja teksta
  display.setCursor(0, 0);
  display.println("Pokretanje\nsistema...");
  display.display();
  delay(1000);
  prikaziSTS();
  prikaziIVE();
  prikaziGodinu();
  display.clearDisplay();
  display.setTextSize(2);      // Veličina fonta
  display.setTextColor(SSD1306_WHITE); // Boja teksta
  display.setCursor(0, 0);
  display.println("Unesite\nsifru:");
  display.display();
  delay(500);
  Serial.println("Unesite šifru:");
  display.clearDisplay();
}

void loop() {
  char key;
   bool otkljucana;
  unsigned long startTime = millis(); // Početno vrijeme
  while (millis() - startTime < 3000) {
    key = keypad.getKey();
  if (key) { // Ako je tipka pritisnuta
    if (key == '#') { // Ako je potvrda šifre
      if (enteredCode == correctCode) {
        Serial.println("Šifra tačna!");
        display.clearDisplay();
        display.setTextSize(2);       
        display.setCursor(0, 0);
        display.println("Sifra\ntacna!");
        display.display();
        delay(1000);
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Unesite\nsifru:");
        display.display();
        enteredCode1 = enteredCode + key;
        enteredCode = ""; // Resetovanje unosa
      } else {
        Serial.println("Pogrešna šifra. Pokušajte ponovo:");
        display.clearDisplay();
        display.setCursor(0, 0);
        display.setTextSize(2); 
        display.println("Pogresna\nsifra.");
        display.clearDisplay();
        display.setCursor(0, 0);        
        display.println("Pokusajte\nponovo:");
        display.display();
        delay(1000);
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Unesite\nsifru:");
        display.display();
        delay(500);
        display.clearDisplay();
        enteredCode = ""; // Resetovanje unosa
      }
    } else if (key == '*') { // Ako je brisanje unosa
      if (enteredCode.length() > 0) {
        enteredCode.remove(enteredCode.length() - 1); // Briše posljednji karakter
        Serial.print("Trenutni unos: ");
        Serial.println(enteredCode);

        // Prikaz trenutnog unosa bez brisanja celog ekrana
        display.clearDisplay();
        display.setCursor(0, 0);
        display.setTextSize(2);
        display.println(enteredCode);
        display.display();
      }
    } else { // Dodavanje unosa
      if (enteredCode.length() < 10) { // Maksimalna dužina šifre (po želji)
        enteredCode += key; // Dodajemo pritisnuti taster na uneseni tekst
        Serial.print("Trenutni unos: ");
        Serial.println(enteredCode);
        display.setCursor(0, 0);
        display.println(enteredCode);
        display.display();
      }
    }
  }
  }
  if (((enteredCode1 == "2468A#") || digitalRead(OtkljucajUnutra) == LOW) && otkljucana == false)
  {
    digitalWrite(Brava,HIGH);
    enteredCode1 = "";
    otkljucana = true;
    Serial.print("Vrata otključana! ");
    display.clearDisplay();
  }
  if ((digitalRead(ZakljucajUnutra) == LOW || digitalRead(ZakljucajVani) == LOW) && otkljucana == true) {
    digitalWrite(Brava,LOW);
    otkljucana = false;
    enteredCode = "";
    Serial.print("Vrata zaključana! ");
    
  }
}
