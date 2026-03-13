// ==================================================
// BUTTON_MODULE.ino
// HARDWARE PHYSICAL BUTTONS (START, STOP, RESET)
// ==================================================

// Button state tracking (for edge detection)
bool last_btn_start = HIGH;
bool last_btn_stop = HIGH;
bool last_btn_reset = HIGH;

void moduleButtons() {
  // Read current states
  bool current_start = digitalRead(PIN_BTN_START);
  bool current_stop = digitalRead(PIN_BTN_STOP);
  bool current_reset = digitalRead(PIN_BTN_RESET);

  // 1. START BUTTON (Auto Mode) (Pressed = LOW)
  if (current_start == LOW && last_btn_start == HIGH) {
    Serial.println("BTN: START PRESSED -> AUTO MODE");
    SYSTEM_MODE = 0; // 0 = Auto Mode
    saveStateToEEPROM();
  }

  // 2. STOP BUTTON (Manual Mode) (Pressed = LOW)
  if (current_stop == LOW && last_btn_stop == HIGH) {
    Serial.println("BTN: STOP PRESSED -> MANUAL MODE");
    SYSTEM_MODE = 1; // 1 = Manual Mode
    saveStateToEEPROM();
  }

  // 3. RESET BUTTON (Pressed = LOW)
  if (current_reset == LOW && last_btn_reset == HIGH) {
    Serial.println("BTN: RESET PRESSED -> KILL OUTPUTS");
    
    // First safely turn off all physical pins
    allOutputOff();
    
    // Second, force SYSTEM_MODE to manual so it doesn't immediately turn back on
    SYSTEM_MODE = 1;
    
    // Third, zero out all tracking states so sendEventStatusPI() broadcasts the OFF state to Pi
    state_CR_MAIN_PUMP = 0;
    state_CR_BUFFER_VALVE = 0;
    state_CR_GREENHOUSE_VALVE = 0;
    
    state_BM_DOSING_A = 0;
    state_BM_DOSING_B = 0;
    state_BM_DOSING_C = 0;
    state_BM_MIX_VALVE = 0;
    state_BM_MIX_PUMP = 0;
    
    state_MM_SUB_PUMP = 0;
    state_MM_T1_IN = 0;
    state_MM_T1_OUT = 0;
    state_MM_T2_IN = 0;
    state_MM_T2_OUT = 0;
    state_MM_T3_IN = 0;
    state_MM_T3_OUT = 0;
    
    state_SS1_VALVE_1 = 0;
    state_SS2_VALVE_1 = 0;
    
    // Save all-OFF state to EEPROM
    saveStateToEEPROM();

    // Send explicit hardware reset byte to PI (0x15)
    Serial.write(0xFE);
    Serial.write(0x15);
    Serial.write(0x01);
    Serial.write('\n');
  }

  // Store states for next loop (debouncing/edge detection)
  last_btn_start = current_start;
  last_btn_stop = current_stop;
  last_btn_reset = current_reset;
}
