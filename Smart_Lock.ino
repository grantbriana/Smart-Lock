#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9

#define RELAY 3 //relay pin

#define ACCESS_DELAY 2000
#define DENIED_DELAY 1000
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance (RFID Reader)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  //115200 9600
  pinMode(RELAY, OUTPUT);
  
  digitalWrite(RELAY, HIGH);
  Serial.println("Put your card to the reader...");
  Serial.println();
}

void loop() {
  // Runs repeatedly
  // Looks for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  //Prints RFID UID (Tag ID)
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  
   //if valid card with valid UID is presented, authorized message returned
  if (content.substring(1) == "63 E2 4B 94") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
    digitalWrite(RELAY, LOW);
    //Solenoid Unlocked
    delay(ACCESS_DELAY);
    digitalWrite(RELAY, HIGH);
  }
 
 else   {
    //if invalid card is presented, rejection message returned
    Serial.println(" Access denied");
    delay(DENIED_DELAY);
    
  }
  //Presentation: https://docs.google.com/presentation/d/16TDbiAZifbw5ZtmMXuEj2Xs19wC0R37v/edit?usp=sharing&ouid=115498961157537223179&rtpof=true&sd=true
  //Reference : https://www.hackster.io/mertarduino/solenoid-door-lock-contol-rfid-527505
  
}
