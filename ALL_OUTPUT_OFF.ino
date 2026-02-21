void allOutputOff() {

  digitalWrite(PIN_CR_BUFFER_VALVE,     HIGH);
  digitalWrite(PIN_CR_GREENHOUSE_VALVE, HIGH);
  digitalWrite(PIN_CR_MAIN_PUMP,        HIGH);

  digitalWrite(PIN_BM_VALVE_A,     HIGH);
  digitalWrite(PIN_BM_VALVE_B,     HIGH);
  digitalWrite(PIN_BM_VALVE_C,     HIGH);
  digitalWrite(PIN_BM_MIX_VALVE,   HIGH);
  digitalWrite(PIN_BM_SOL_PUMP,    HIGH);
  digitalWrite(42,                 HIGH);  // BM_MIX_PUMP

  digitalWrite(PIN_MM_SOL_PUMP, HIGH);
  digitalWrite(PIN_MM_T1_IN,    HIGH);
  digitalWrite(PIN_MM_T1_OUT,   HIGH);
  digitalWrite(PIN_MM_T2_IN,    HIGH);
  digitalWrite(PIN_MM_T2_OUT,   HIGH);
  digitalWrite(PIN_MM_T3_IN,    HIGH);
  digitalWrite(PIN_MM_T3_OUT,   HIGH);

  digitalWrite(PIN_SS_PUMP,     HIGH);
  digitalWrite(PIN_SS1_VALVE_1, HIGH);
  digitalWrite(PIN_SS2_VALVE_1, HIGH);
}
