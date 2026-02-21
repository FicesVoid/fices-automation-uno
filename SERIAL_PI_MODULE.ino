void moduleSerialPI() {
  // NASA Grade Strict 2-Byte Protocol
  while (Serial.available() >= 2) {
    byte id = Serial.read();
    byte val = Serial.read();

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
      case 0x09: BM_SOL_PUMP = val; break;
      case 0x0A: BM_MIX_PUMP = val; break;
      
    // --- Mixer Module ---
    case 0x0B: MM_SOL_PUMP = val; break;
    case 0x0C: MM_T1_IN    = val; break;
    case 0x0D: MM_T1_OUT   = val; break;
    case 0x0E: MM_T2_IN    = val; break;
    case 0x0F: MM_T2_OUT   = val; break;
    case 0x10: MM_T3_IN    = val; break;
    case 0x11: MM_T3_OUT   = val; break;
    
    // --- Others ---
    case 0x12: SS_PUMP     = val; break;
    case 0x13: SS1_VALVE_1 = val; break;
    case 0x14: SS2_VALVE_1 = val; break;

    // --- Master Control ---
    case 0xFF: 
    // ...

      
      default:
        // Unknown ID, ignore
        break;
    }
  }
}
