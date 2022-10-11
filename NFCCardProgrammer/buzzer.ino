const int BUZZER_PIN = 10; // The trigger pin for the buzzer

/**
 * Turns the buzzer on/off if successful
 * 
 * @returns   void
 */
void buzzerSuccess() {
  tone(BUZZER_PIN, 1500);
  delay(500);
  noTone(BUZZER_PIN);
}

/**
 * Turns the buzzer on/off if not successful
 * 
 * @returns   void
 */
void buzzerFailure() {
  tone(BUZZER_PIN, 1500);
  delay(200);
  noTone(BUZZER_PIN);
  delay(50);
  tone(BUZZER_PIN, 1500);
  delay(200);
  noTone(BUZZER_PIN);
  delay(50);
  tone(BUZZER_PIN, 1500);
  delay(200);
  noTone(BUZZER_PIN);
  delay(50);
}

/**
 * Generic wrapper function pointer for different calls
 * 
 * @returns   void
 */
void buzz(void (*f) ()) {
  f();
}

void buzzerSetup() {
  // No setup required for the buzzer
}

