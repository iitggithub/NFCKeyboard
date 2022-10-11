/**
 * led.ino contains functions that are used to setup
 * and control LED's. LED should be wired to ground with
 * power being provided by the digital IO pins. A suitable resistor
 * ie 150 ohm should be used according to LED requirements.
**/ 

#define LED_GREEN_PIN (7) // LED green trigger pin
#define LED_RED_PIN (8)   // LED red trigger pin

/**
 * Turns the bi-color LED green
 * 
 * @returns   void
 */
void ledSuccess()
{
  digitalWrite(LED_RED_PIN,LOW);
  digitalWrite(LED_GREEN_PIN,HIGH);
}

/**
 * Turns the bi-color LED red (default state)
 * 
 * @returns   void
 */
void ledReset()
{
  digitalWrite(LED_GREEN_PIN,LOW);
  digitalWrite(LED_RED_PIN,HIGH);
}

/**
 * Flashes the led red 3 times to indicate access denied.
 * 
 * @returns   void
 */
void ledFailure()
{
  digitalWrite(LED_GREEN_PIN,LOW);
  digitalWrite(LED_RED_PIN,HIGH);
  delay(200);
  digitalWrite(LED_RED_PIN,LOW);
  delay(100);
  digitalWrite(LED_RED_PIN,HIGH);
  delay(200);
  digitalWrite(LED_RED_PIN,LOW);
  delay(100);
  digitalWrite(LED_RED_PIN,HIGH);
  delay(200);
  digitalWrite(LED_RED_PIN,LOW);
  delay(100);
  ledReset();
}

/**
 * Turns the bi-color LED orange. Used to indicate permanent errors.
 * 
 * @returns   void
 */
void ledError()
{
  digitalWrite(LED_GREEN_PIN,LOW);
  digitalWrite(LED_RED_PIN,HIGH);
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
  pinMode(LED_GREEN_PIN,OUTPUT);
  pinMode(LED_RED_PIN,OUTPUT);
  
  // Set the default state of the LED
  digitalWrite(LED_RED_PIN,HIGH);
}
