void moduleSerialPI() {
  while (Serial.available() >= 2) {
    byte id = Serial.read();
    byte val = Serial.read();

    if (Serial.available() > 0 && Serial.peek() == '\n') {
      Serial.read();
    }
    
    // Process command based on ID
    switch (id) {
      // ===== SYSTEM MODE =====
      case 0x01:
        if (SYSTEM_MODE != val) {
          SYSTEM_MODE = val;
          last_SYSTEM_MODE = !SYSTEM_MODE; // Force event send
        }
        break;
      
      // ===== CONTROL ROOM =====
      case 0x02: CR_MAIN_PUMP = val; break;
      case 0x03: CR_BUFFER_VALVE = val; break;
      case 0x04: CR_GREENHOUSE_VALVE = val; break;
      
      // ===== BUFFER MIXER =====
      case 0x05: BM_VALVE_A = val; break;
      case 0x06: BM_VALVE_B = val; break;
      case 0x07: BM_VALVE_C = val; break;
      case 0x08: BM_MIX_VALVE = val; break;
      case 0x0A: BM_MIX_PUMP = val; break;
      
      // ===== MIXER MODULE =====
      case 0x0B: MM_SOL_PUMP = val; break;
      case 0x0C: MM_T1_IN = val; break;
      case 0x0D: MM_T1_OUT = val; break;
      case 0x0E: MM_T2_IN = val; break;
      case 0x0F: MM_T2_OUT = val; break;
      case 0x10: MM_T3_IN = val; break;
      case 0x11: MM_T3_OUT = val; break;
      
      // ===== SUBSTATION =====
      case 0x12: SS_PUMP = val; break;
      case 0x13: SS1_VALVE_1 = val; break;  // SUB1_VALVE maps to SS1_VALVE_1
      case 0x14: SS2_VALVE_1 = val; break;  // SUB2_VALVE maps to SS2_VALVE_1
      
      // ===== RECIPE SYSTEM =====
      case 0x20: RECIPE_TANK = val; break;
      case 0x21: RECIPE_A = val; break;
      case 0x22: RECIPE_B = val; break;
      case 0x23: RECIPE_C = val; break;
      case 0x24: RECIPE_EC = val; break;
      case 0x25: RECIPE_WATER = val; break;
      case 0x26: 
        if (val == 1) startMixingRecipe();  // RECIPE_START
        break;
      case 0x27:
        if (val == 1) cancelMixingRecipe(); // RECIPE_CANCEL
        break;
      
      default:
        // Unknown ID, ignore
        break;
    }
  }
}
