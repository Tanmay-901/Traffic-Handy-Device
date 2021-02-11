#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above


const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {A0,2,3,4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5,6,7,8}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
int a=4500, b=5500, c=6500;
int readsuccess;
byte readcard[4];
char str[32] = "", key;
String StrUID, charge, penalty;

void setup() {
  Serial.begin(9600);                                           // Initialize serial communications with the PC
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();                                              // Init MFRC522 card
//  Serial.println(F("Read personal data on a MIFARE PICC:"));    //shows in serial that it is ready to read 
}

void loop() {

   readsuccess = getid();
  if(readsuccess) {
   Serial.println("");
    if(StrUID=="DB692E0B"){
      Serial.println("Vehicle no: RJ14-LC-1996");
      Serial.print("Select Charge: ");
      while(penalty=="" or charge==""){
        penalty, charge = fine();
      }
      Serial.println("Mr. ADITYA is fined Rs. ");
      Serial.print(penalty);
      Serial.print(" for ");
      Serial.print(charge);
      Serial.print(" !!!");
      Serial.println("");
      Serial.println("");
      penalty = "";
      charge="";
      }
    if(StrUID=="E63C27F8"){
    Serial.println("Vehicle no: RJ14-CG-3542");
    Serial.println("Select Charge: ");
    while(penalty=="" or charge==""){
      penalty, charge = fine();
    }
    Serial.print("Mr. Tanmay is fined Rs. ");
    Serial.print(penalty);
    Serial.print(" for ");
    Serial.print(charge);
    Serial.print(" !!!");
    Serial.println("");
    Serial.println("");
    penalty="";
    charge="";
  }
  if(StrUID=="FE9EC983"){
    Serial.println("Vehicle no: RJ20-AS-9817");
    Serial.println("Select Charge: ");
    while(penalty=="" or charge==""){
      penalty, charge = fine();
    }
    Serial.print("Miss. Shivani is fined Rs. ");
    Serial.print(penalty);
    Serial.print(" for ");
    Serial.print(charge);
    Serial.print(" !!!");
    Serial.println("");
    Serial.println("");
    penalty="";
    charge="";
  }
  delay(1000);
      }
}
// ------------------------------------------------------------------
int getid() {  
  if(!mfrc522.PICC_IsNewCardPresent()) {
    return 0;
  }
  if(!mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }
 
  for(int i=0;i<4;i++){
    readcard[i]=mfrc522.uid.uidByte[i]; //storing the UID of the tag in readcard
    array_to_string(readcard, 4, str);
    StrUID = str;
  }
  mfrc522.PICC_HaltA();
  return 1;
}
// -----------------------------------------------------------------
String fine(){
  char key = keypad.getKey();
  if (key){
  if (key=='1'){
    penalty = "200";
    charge = "Not wearing Helmet" ;
  }
  if (key=='2'){
    penalty = "1000";
    charge = "lack of Documents" ;
  }
  if (key=='3'){
    penalty = "1500";
    charge = "Rash Driving" ;
  }
  if (key=='4'){
    penalty = "500";
    charge = "Not having Insurance" ;
  }
  }
  else{
    penalty="";
    charge="";
  }
  return penalty, charge;
}
// --------------------------------------------------------------------
void array_to_string(byte array[], unsigned int len, char buffer[]) {
    for (unsigned int i = 0; i < len; i++)
    {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
        buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
    }
    buffer[len*2] = '\0';
}
