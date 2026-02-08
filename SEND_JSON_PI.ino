// ==================================================
// SEND_JSON_PI.ino
// SENSOR DATA -> RASPBERRY PI (JSON, 5s)
// ==================================================

unsigned long lastSensorSend = 0;
const unsigned long SENSOR_INTERVAL = 2000; // 2 saat

void moduleSendJsonPI() {

  if (millis() - lastSensorSend < SENSOR_INTERVAL) return;
  lastSensorSend = millis();

  Serial.print("{");
  
  // ===== MODE =====
  Serial.print("\"mode\":"); Serial.print(SYSTEM_MODE);

  // ===== TANK LEVEL =====
  Serial.print(",\"mtl\":"); Serial.print(mtl);
  Serial.print(",\"btl\":"); Serial.print(btl);

  // ===== BUFFER MIXER LEVEL =====
  Serial.print(",\"va\":"); Serial.print(va);
  Serial.print(",\"vb\":"); Serial.print(vb);
  Serial.print(",\"vc\":"); Serial.print(vc);

  // ===== MIXER MODULE LEVEL =====
  Serial.print(",\"t1l\":"); Serial.print(t1l);
  Serial.print(",\"t2l\":"); Serial.print(t2l);
  Serial.print(",\"t3l\":"); Serial.print(t3l);

  // ===== TEMPERATURE =====
  Serial.print(",\"temp\":"); Serial.print(temp);
  Serial.print(",\"t1t\":"); Serial.print(t1t);
  Serial.print(",\"t2t\":"); Serial.print(t2t);
  Serial.print(",\"t3t\":"); Serial.print(t3t);

  // ===== PH =====
  Serial.print(",\"ph\":");  Serial.print(ph);
  Serial.print(",\"t1p\":"); Serial.print(t1p);
  Serial.print(",\"t2p\":"); Serial.print(t2p);
  Serial.print(",\"t3p\":"); Serial.print(t3p);

  // ===== EC & TDS =====
  Serial.print(",\"ec\":");    Serial.print(ec);
  Serial.print(",\"tds\":");   Serial.print(tds);
  Serial.print(",\"t1tds\":"); Serial.print(t1tds);
  Serial.print(",\"t2tds\":"); Serial.print(t2tds);
  Serial.print(",\"t3tds\":"); Serial.print(t3tds);

  // ===== OUTPUTS (RELAYS) - Flat Structure =====
  
  // Control Room
  Serial.print(",\"CR_BUFFER_VALVE\":");     Serial.print(state_CR_BUFFER_VALVE);
  Serial.print(",\"CR_GREENHOUSE_VALVE\":"); Serial.print(state_CR_GREENHOUSE_VALVE);
  Serial.print(",\"CR_MAIN_PUMP\":");        Serial.print(state_CR_MAIN_PUMP);

  // Buffer Mixer
  Serial.print(",\"BM_VALVE_A\":");   Serial.print(state_BM_VALVE_A);
  Serial.print(",\"BM_VALVE_B\":");   Serial.print(state_BM_VALVE_B);
  Serial.print(",\"BM_VALVE_C\":");   Serial.print(state_BM_VALVE_C);
  Serial.print(",\"BM_MIX_VALVE\":"); Serial.print(state_BM_MIX_VALVE);
  Serial.print(",\"BM_MIX_PUMP\":");  Serial.print(state_BM_MIX_PUMP);

  // Mixer Module
  Serial.print(",\"MM_SOL_PUMP\":"); Serial.print(state_MM_SOL_PUMP);
  Serial.print(",\"MM_T1_IN\":");    Serial.print(state_MM_T1_IN);
  Serial.print(",\"MM_T1_OUT\":");   Serial.print(state_MM_T1_OUT);
  Serial.print(",\"MM_T2_IN\":");    Serial.print(state_MM_T2_IN);
  Serial.print(",\"MM_T2_OUT\":");   Serial.print(state_MM_T2_OUT);
  Serial.print(",\"MM_T3_IN\":");    Serial.print(state_MM_T3_IN);
  Serial.print(",\"MM_T3_OUT\":");   Serial.print(state_MM_T3_OUT);

  // Substation
  Serial.print(",\"SS_PUMP\":");     Serial.print(state_SS_PUMP);
  Serial.print(",\"SS1_VALVE_1\":"); Serial.print(state_SS1_VALVE_1);
  Serial.print(",\"SS2_VALVE_1\":"); Serial.print(state_SS2_VALVE_1);

  Serial.println("}"); // Close main object
}

// ==================================================
// EVENT FEEDBACK -> RASPBERRY PI (BYTE PROTOCOL)
// Format: 0xFE [ID] [VALUE] \n
// ==================================================

void sendEventStatusPI() {

  // ===== SYSTEM MODE =====
  sendByteEvent(0x01, SYSTEM_MODE, last_SYSTEM_MODE);

  // ===== CONTROL ROOM =====
  sendByteEvent(0x02, state_CR_MAIN_PUMP, last_CR_MAIN_PUMP);
  sendByteEvent(0x03, state_CR_BUFFER_VALVE, last_CR_BUFFER_VALVE);
  sendByteEvent(0x04, state_CR_GREENHOUSE_VALVE, last_CR_GREENHOUSE_VALVE);

  // ===== BUFFER MIXER =====
  sendByteEvent(0x05, state_BM_VALVE_A, last_BM_VALVE_A);
  sendByteEvent(0x06, state_BM_VALVE_B, last_BM_VALVE_B);
  sendByteEvent(0x07, state_BM_VALVE_C, last_BM_VALVE_C);
  sendByteEvent(0x08, state_BM_MIX_VALVE, last_BM_MIX_VALVE);
  sendByteEvent(0x0A, state_BM_MIX_PUMP, last_BM_MIX_PUMP);

  // ===== MIXER MODULE =====
  sendByteEvent(0x0B, state_MM_SOL_PUMP, last_MM_SOL_PUMP);
  sendByteEvent(0x0C, state_MM_T1_IN, last_MM_T1_IN);
  sendByteEvent(0x0D, state_MM_T1_OUT, last_MM_T1_OUT);
  sendByteEvent(0x0E, state_MM_T2_IN, last_MM_T2_IN);
  sendByteEvent(0x0F, state_MM_T2_OUT, last_MM_T2_OUT);
  sendByteEvent(0x10, state_MM_T3_IN, last_MM_T3_IN);
  sendByteEvent(0x11, state_MM_T3_OUT, last_MM_T3_OUT);

  // ===== SUBSTATION =====
  sendByteEvent(0x12, state_SS_PUMP, last_SS_PUMP);
  sendByteEvent(0x13, state_SS1_VALVE_1, last_SS1_VALVE_1);
  sendByteEvent(0x14, state_SS2_VALVE_1, last_SS2_VALVE_1);
}

// ==================================================

void sendByteEvent(byte id, int current, int last) {
  if (current != last) {
    Serial.write(0xFE);        // Event marker
    Serial.write(id);          // Variable ID
    Serial.write((byte)current); // Value (0 or 1)
    Serial.write('\n');        // Newline delimiter
  }
}
