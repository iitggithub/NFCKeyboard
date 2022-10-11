#include <Keyboard.h> // Writes out our random string

void kbWriteString()
{
  Keyboard.print(erReadStringFromEEPROM());
  Keyboard.press(KEY_RETURN);
  delay(30);
  Keyboard.release(KEY_RETURN);
  Keyboard.end();
}

void kbSetup() {
  // initialize control over the keyboard:
  Serial.print("keyboard: Initiating keyboard control... ");
  Keyboard.begin();
  Serial.println("done!");
}
