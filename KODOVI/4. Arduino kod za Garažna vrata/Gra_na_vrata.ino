#include <IRremote.hpp>
#define IR_RECEIVE_PIN 2
# define IN1 5
# define IN2 6
int prekidac_zatvorena=3;
int prekidac_otvorena=4;
void setup()
{
  Serial.begin(9600); // // Establish serial communication
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver
  pinMode (IN1,OUTPUT);
  pinMode (IN2,OUTPUT);
  pinMode (prekidac_zatvorena,INPUT);
  pinMode (prekidac_otvorena,INPUT);
}
void otvoriVrata (){  
        Serial.println("Vrata se otvaraju");
        analogWrite (IN1,500);
        digitalWrite (IN2,LOW);
  while (digitalRead(prekidac_otvorena) == HIGH);
    {
      zaustaviMotor();
    }
;

  zaustaviMotor();
  Serial.println("Vrata potpuno otvorena.");

     }
      void zatvoriVrata ()
     {  
        Serial.println("Vrata se zatvaraju");
        digitalWrite (IN1,LOW);
        analogWrite (IN2,500);
  while (digitalRead(prekidac_zatvorena) == HIGH);

  zaustaviMotor();
  Serial.println("Vrata potpuno zatvorena.");

     }
     void zaustaviMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}
void loop() {
  if (IrReceiver.decode()) {
      Serial.println(IrReceiver.decodedIRData.decodedRawData,HEX); // Print "old" raw data
      IrReceiver.printIRResultShort(&Serial); // Print complete received data in one line
      IrReceiver.printIRSendUsage(&Serial);   // Print the statement required to send this data
      IrReceiver.resume(); // Enable receiving of the next value
      }
    if (( IrReceiver.decodedIRData.decodedRawData == 0XF30CFF00) && (digitalRead(prekidac_zatvorena)==LOW))
    {
      otvoriVrata();
     
      }
       if (( IrReceiver.decodedIRData.decodedRawData == 0XE718FF00)&&(digitalRead(prekidac_otvorena)==LOW))
    {
      zatvoriVrata();
    
      }

}
