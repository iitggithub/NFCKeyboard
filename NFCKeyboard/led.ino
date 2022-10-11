/**
 * led.ino contains functions that are used to setup
 * and control LED's.
 * Current LED in use is a Clear RGB LED with a common
 * anode which is wired to 5v.
 * https://core-electronics.com.au/led-rgb-clear-common-anode.html
 * Resistor requirements:
 * RED: 1 x 150 Ohm 0.6w resistor
 * GREEN: 1 x 90 Ohm 0.36w resistor
 * BLUE: 1 x 90 Ohm 0.36w resistor
 * Datasheet:
 * https://core-electronics.com.au/attachments/localcontent/YSL-R596AR3G4B5C-C10_53869014ee8.pdf
 * 
**/ 
#define LED_BLUE_PIN (6) // LED blue trigger pin
#define LED_GREEN_PIN (7) // LED green trigger pin
#define LED_RED_PIN (8)   // LED red trigger pin

boolean stealthMode = true; // 1: Enable Stealth Mode (turn off Red LED standby)

/**
 * Turns the bi-color LED green
 * 
 * @returns   void
 */
void ledSuccess()
{
  digitalWrite(LED_RED_PIN,HIGH);
  digitalWrite(LED_GREEN_PIN,LOW);
}

/**
 * Turns the bi-color LED red (default state)
 * If stealth mode is enabled, turn all LED's
 * off by default.
 * 
 * @returns   void
 */
void ledReset()
{
  digitalWrite(LED_BLUE_PIN,HIGH);
  digitalWrite(LED_GREEN_PIN,HIGH);
  if (stealthMode) {
    digitalWrite(LED_RED_PIN,HIGH);  
  } else {
    digitalWrite(LED_RED_PIN,LOW);
  }
  
}

/**
 * Flashes the led red 3 times to indicate access denied.
 * 
 * @returns   void
 */
void ledFailure()
{
  digitalWrite(LED_BLUE_PIN,HIGH);
  digitalWrite(LED_GREEN_PIN,HIGH);
  digitalWrite(LED_RED_PIN,LOW);
  delay(200);
  digitalWrite(LED_RED_PIN,HIGH);
  delay(100);
  digitalWrite(LED_RED_PIN,LOW);
  delay(200);
  digitalWrite(LED_RED_PIN,HIGH);
  delay(100);
  digitalWrite(LED_RED_PIN,LOW);
  delay(200);
  ledReset();
}

/**
 * Turns the LED yellow. Used to indicate permanent errors.
 * 
 * @returns   void
 */
void ledError()
{
  digitalWrite(LED_GREEN_PIN,LOW);
  digitalWrite(LED_RED_PIN,LOW);
}

/**
 * Function pointer to change the state of the LED's
 * 
 * @returns   void
 */
void ledToggler(void (*f)())
{
  f();
}

/**
 * Setup function for LED's
 * 
 * @returns   void
 */

void ledSetup()
{
  pinMode(LED_BLUE_PIN,OUTPUT);
  pinMode(LED_GREEN_PIN,OUTPUT);
  pinMode(LED_RED_PIN,OUTPUT);
  
  // Set the default state of the LED
  ledReset();
}
