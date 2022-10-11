#include <Adafruit_PN532.h> // Required to connect to NFC/RFID shield over i2c

#define PN532_IRQ   (4) // Cut trace between digital pin 2 and IRQ and wire IRQ to digital pin 4
#define PN532_RESET (3)  // Not connected by default on the NFC Shield

// Or use this line for a breakout or shield with an I2C connection:
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
/**
 * General setup function
 * 
 * @returns   void
 */
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

  erSetup();     // Any EEPROM related setup tasks
  kbSetup();     // Keybaard related setup tasks
  ledSetup();    // LED setup tasks
  buzzerSetup(); // buzzer related setup tasks
  nfcSetup();    // nfc related setup tasks

  Serial.print("nfc: Starting NFC... ");
  nfc.begin();
  Serial.println("done!");

  Serial.print("nfc: Checking NFC board firmware... ");
  uint32_t versiondata = nfc.getFirmwareVersion();

  if (! versiondata)
  {
    Serial.println("failed!");
    Serial.print("nfc: Didn't find PN53x board");
    while (1); // halt
  }
  Serial.println("done!");
  // Got ok data, print it out!
  
  Serial.print("nfc: Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("nfc: Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);
  
  // configure board to read RFID tags
  nfc.SAMConfig();

  Serial.println("Waiting for an ISO14443A card");
}

/**
 * Main program loop
 * 
 * @returns   void
 */
void loop() {
  boolean success  = false;                    // Determines whether we've successfully read a card or not
  boolean is_valid = false;                    // Determines whether Card UID matches the one stored in EEPROM
  uint8_t uid[]    = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                           // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
 
  if (success)
  {
    // Show card information on the serial monitor
    Serial.println("Found a card!");
    Serial.print("UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("UID Value: ");
    for (uint8_t i=0; i < uidLength; i++) 
    {
      Serial.print(" 0x");Serial.print(uid[i], HEX);
    }
    Serial.println("");

    // Check if the card is valid
    uint8_t isValidCount = 0;
    for (uint8_t i=0; i < uidLength; i++) 
    {
      if (erGetValue(i) == uid[i])
      {
        isValidCount++;
      }
    }

    // If the UID on the card matches EEPROM memory
    // This is a valid card.
    if (isValidCount == uidLength) {
      is_valid = true;
    }

    // If it's valid, perform any necessary actions
    if (is_valid) {
      Serial.println("Valid UID. Access granted.");
      ledToggler(ledSuccess);
      buzz(buzzerSuccess);
      ledToggler(ledReset);
      kbWriteString();
    } else {
      Serial.println("Invalid UID. Access denied.");
      ledToggler(ledFailure);
      buzz(buzzerFailure);
      ledToggler(ledReset);
    }

    delay(3000); // Wait 3 seconds to avoid multiple card triggering events
  }
  else
  {
    // PN532 probably timed out waiting for a card
    Serial.println("Timed out waiting for a card data. success variable is false.");
  }
}
