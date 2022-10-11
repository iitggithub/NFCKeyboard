#include <EEPROM.h>  // Store data in permanent EEPROM memory

// EEPROM related global variables

/**
 * Returns the byte data stored at a given EEPROM memory address
 * 
 * @param int a The EEPROM memory address to read
 * 
 * @returns   byte
 */
byte erGetValue(int a) {
  return EEPROM.read(a);
}

/**
 * Stores a given value in EEPROM
 * 
 * @param int  a The EEPROM memory address to store the value
 * @param byte b The byte value to store in EEPROM
 * 
 * @returns   void
 */
void erSetValue(int a, byte b) {
  Serial.print("eeprom: Writing value '");
  Serial.print(b);
  Serial.print("' to address ");
  Serial.println(a);
  EEPROM.update(a, b);
}

// https://roboticsbackend.com/arduino-write-string-in-eeprom/#:~:text=To%20save%20a%20String%20into,have%20to%20read%20from%20EEPROM%3F
/**
 * Stores a given string and its length in EEPROM memory so it can be return after powering on.
 * The length is stored in the same position in memory with the string
 * being stored at an offset equal to the length of the string which
 * ensures that the string never overwrites the value that contains its
 * length.
 * 
 * @returns   void
 */
void erWriteStringToEEPROM(const String &strToWrite)
{
  byte len = strToWrite.length();
  erSetValue(254, len);
  for (int i = 0; i < len; i++)
  {
    erSetValue((253 - len) + i, strToWrite[i]);
  }
}
/**
 * Reads an entire string from EEPROM memory.
 * 
 * @returns   String
 */
String erReadStringFromEEPROM()
{
  int newStrLen = erGetValue(254); // The location of the length of the string
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = erGetValue((253 - newStrLen) + i);
  }
  data[newStrLen] = '\0';
  return String(data);
}

/**
 * EEPROM related setup tasks go here
 * 
 * @returns   bool
 */
bool erSetup() {
  int    erRandomStringLen = erGetValue(254);
  String erRandomString    = erReadStringFromEEPROM();
  if (erRandomString.length() == erRandomStringLen)
  {
    return true;
  }

  Serial.print("eeprom: EEPROM String mismatch. value: '");
  Serial.print(erRandomString);
  Serial.println("'.");
  return false;
}
