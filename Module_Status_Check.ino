void moduleStatusCheck() {

  state_CR_BUFFER_VALVE     = !digitalRead(PIN_CR_BUFFER_VALVE);
  state_CR_GREENHOUSE_VALVE = !digitalRead(PIN_CR_GREENHOUSE_VALVE);
  state_CR_MAIN_PUMP        = !digitalRead(PIN_CR_MAIN_PUMP);

  // MM Solution Pump (Pin 43)
  state_MM_SOL_PUMP = !digitalRead(PIN_MM_SOL_PUMP);

  // BM Mix Pump (Pin 42)
  state_BM_MIX_PUMP = !digitalRead(42);

     // Often redundant if shared

  state_BM_MIX_VALVE = !digitalRead(PIN_BM_MIX_VALVE);

  state_BM_VALVE_A = !digitalRead(PIN_BM_VALVE_A);
  state_BM_VALVE_B = !digitalRead(PIN_BM_VALVE_B);
  state_BM_VALVE_C = !digitalRead(PIN_BM_VALVE_C);

  state_MM_T1_IN  = !digitalRead(PIN_MM_T1_IN);
  state_MM_T1_OUT = !digitalRead(PIN_MM_T1_OUT);
  state_MM_T2_IN  = !digitalRead(PIN_MM_T2_IN);
  state_MM_T2_OUT = !digitalRead(PIN_MM_T2_OUT);
  state_MM_T3_IN  = !digitalRead(PIN_MM_T3_IN);
  state_MM_T3_OUT = !digitalRead(PIN_MM_T3_OUT);

  state_SS1_VALVE_1 = !digitalRead(PIN_SS1_VALVE_1);
  state_SS2_VALVE_1 = !digitalRead(PIN_SS2_VALVE_1);
  state_SS_PUMP     = !digitalRead(PIN_SS_PUMP);
}
