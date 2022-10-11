#include <Adafruit_PN532.h> // Required to connect to NFC/RFID shield over i2c

#define PN532_IRQ   (4) // Cut trace between digital pin 2 and IRQ and wire IRQ to digital pin 4
#define PN532_RESET (3)  // Not connected by default on the NFC Shield

// Or use this line for a breakout or shield with an I2C connection:
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

String randomString = "CHANGE ME"; // Add a semi colon to the end of the statement. This line is intentionally broken to stop people from uploading a password that they didn't choose.

void setup() {
  Serial.begin(115200);

  // Wait for serial connection
  int fail = 0;
  while (!Serial)
  {
    delay(10); // for Leonardo/Micro/Zero which takes longer to start
    fail++;
    if (fail>100)
    {
      // We spent more than 1 second waiting for serial output. Break the loop
      // because it's not needed in production.
      break;
    }
  }

  Serial.println("==========================");
  Serial.println("NFC/RFID Setup begins now!");
  Serial.println("==========================");

  erSetup(); // Zero EEPROM and write randomString to EEPROM

  Serial.print("Starting NFC... ");
  nfc.begin();
  Serial.println("done!");

  Serial.print("Checking NFC board firmware... ");
  uint32_t versiondata = nfc.getFirmwareVersion();

  if (! versiondata)
  {
    Serial.println("failed!");
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  Serial.println("done!");
  // Got ok data, print it out!
  
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);
  
  // configure board to read RFID tags
  nfc.SAMConfig();

  Serial.print("Buzzing user to let them know we're ready... ");
  buzz(buzzerSuccess);
  Serial.println("done!");
  
  Serial.println("Waiting for an ISO14443A card");
}

void loop() {
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
 
  if (success)
  {
    Serial.println("Found a card!");
    Serial.print("UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("UID Value: ");
    for (uint8_t i=0; i < uidLength; i++) 
    {
      Serial.print(" 0x");Serial.print(uid[i], HEX);
    }
    Serial.println("");

    for (uint8_t i=0; i < uidLength; i++) 
    {
      erSetValue(i,uid[i]); // save the value to EEPROM
    }

    delay(3000);
  }
  else
  {
    // PN532 probably timed out waiting for a card
    Serial.println("Timed out waiting for a card data. success variable is false.");
  }
}
