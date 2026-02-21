void moduleManual() {
  if (SYSTEM_MODE != 1) return;

  /* ===== CONTROL ROOM ===== */
  /* ===== CONTROL ROOM ===== */
  // Serial.print("MANUAL_DEBUG: Mode="); Serial.print(SYSTEM_MODE); 
  // Serial.print(" PUMP="); Serial.println(CR_MAIN_PUMP);
  
  digitalWrite(PIN_CR_BUFFER_VALVE,     CR_BUFFER_VALVE     ? LOW : HIGH);
  digitalWrite(PIN_CR_GREENHOUSE_VALVE, CR_GREENHOUSE_VALVE ? LOW : HIGH);
  digitalWrite(PIN_CR_MAIN_PUMP,        CR_MAIN_PUMP        ? LOW : HIGH);

  /* ===== BUFFER MIXER ===== */
  digitalWrite(PIN_BM_VALVE_A,   BM_VALVE_A   ? LOW : HIGH);
  digitalWrite(PIN_BM_VALVE_B,   BM_VALVE_B   ? LOW : HIGH);
  digitalWrite(PIN_BM_VALVE_C,   BM_VALVE_C   ? LOW : HIGH);
  digitalWrite(PIN_BM_MIX_VALVE, BM_MIX_VALVE ? LOW : HIGH);


  /* ===== BM MIX PUMP (Pin 42) ===== */
  digitalWrite(PIN_BM_MIX_PUMP, BM_MIX_PUMP ? LOW : HIGH);

  /* ===== BM SOL PUMP (Pin 41) ===== */
  digitalWrite(PIN_BM_SOL_PUMP, BM_SOL_PUMP ? LOW : HIGH);

  /* ===== MM SOL PUMP (Pin 39) ===== */
  digitalWrite(PIN_MM_SUB_PUMP, MM_SUB_PUMP ? LOW : HIGH);

  
  digitalWrite(PIN_MM_T1_IN,    MM_T1_IN    ? LOW : HIGH);
  digitalWrite(PIN_MM_T1_OUT,   MM_T1_OUT   ? LOW : HIGH);
  digitalWrite(PIN_MM_T2_IN,    MM_T2_IN    ? LOW : HIGH);
  digitalWrite(PIN_MM_T2_OUT,   MM_T2_OUT   ? LOW : HIGH);
  digitalWrite(PIN_MM_T3_IN,    MM_T3_IN    ? LOW : HIGH);
  digitalWrite(PIN_MM_T3_OUT,   MM_T3_OUT   ? LOW : HIGH);

  /* ===== SUBSTATION ===== */
  digitalWrite(PIN_SS1_VALVE_1, SS1_VALVE_1 ? LOW : HIGH);
  digitalWrite(PIN_SS2_VALVE_1, SS2_VALVE_1 ? LOW : HIGH);

// updateSharedPump42(); // REMOVED: Redundant
}
