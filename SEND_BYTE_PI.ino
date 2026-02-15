// ==================================================
// SEND_BYTE_PI.ino
// SENSOR DATA -> RASPBERRY PI (BYTE PROTOCOL, 2s)
// ==================================================

unsigned long lastSensorSend = 0;
const unsigned long SENSOR_INTERVAL = 2000; // 2 seconds

// Overload for backward compatibility (defaults to not forced)
void moduleSendBytePI(bool force = false);

// ==================================================
// SEND SINGLE SENSOR AS BYTE PACKET
// ==================================================
// Format: 0xFD [SENSOR_ID] [VALUE_HIGH] [VALUE_LOW] [CHECKSUM] \n
// ==================================================

void sendSensorByte(byte sensorID, float value, int scale) {
  // Scale the value (e.g., 27.5 * 100 = 2750)
  unsigned int scaledValue = (unsigned int)(value * scale);
  
  // Split into high and low bytes
  byte valueHigh = (scaledValue >> 8) & 0xFF;
  byte valueLow = scaledValue & 0xFF;
  
  // Calculate checksum (XOR of all bytes)
  byte checksum = 0xFD ^ sensorID ^ valueHigh ^ valueLow;
  
  // Send packet
  Serial.write(0xFD);        // Sensor marker
  Serial.write(sensorID);    // Sensor ID
  Serial.write(valueHigh);   // Value high byte
  Serial.write(valueLow);    // Value low byte
  Serial.write(checksum);    // Checksum
  Serial.write('\n');        // Delimiter
}

// ==================================================
// SEND ALL SENSORS
// ==================================================

void moduleSendBytePI(bool force) {
  
  if (!force && millis() - lastSensorSend < SENSOR_INTERVAL) return;
  lastSensorSend = millis();
  
  // ===== CONTROL ROOM STATION (0x10-0x1F) =====
  sendSensorByte(0x10, mtl, 100);   // Main tank level
  sendSensorByte(0x11, btl, 100);   // Buffer tank level
  
  // ===== BUFFER MIXER STATION (0x20-0x2F) =====
  sendSensorByte(0x20, temp, 100);  // Temperature
  sendSensorByte(0x21, ph, 100);    // pH
  sendSensorByte(0x22, ec, 100);    // EC
  sendSensorByte(0x23, tds, 1);     // TDS
  sendSensorByte(0x24, va, 100);    // Fertilizer Tank A
  sendSensorByte(0x25, vb, 100);    // Fertilizer Tank B
  sendSensorByte(0x26, vc, 100);    // Fertilizer Tank C
  
  // ===== MIXER MODULE - TANK 1 (0x30-0x3F) =====
  sendSensorByte(0x30, t1l, 100);   // Tank 1 level
  sendSensorByte(0x31, t1t, 100);   // Tank 1 temperature
  sendSensorByte(0x32, t1p, 100);   // Tank 1 pH
  sendSensorByte(0x33, t1tds, 1);   // Tank 1 TDS
  
  // ===== MIXER MODULE - TANK 2 (0x40-0x4F) =====
  sendSensorByte(0x40, t2l, 100);   // Tank 2 level
  sendSensorByte(0x41, t2t, 100);   // Tank 2 temperature
  sendSensorByte(0x42, t2p, 100);   // Tank 2 pH
  sendSensorByte(0x43, t2tds, 1);   // Tank 2 TDS
  
  // ===== MIXER MODULE - TANK 3 (0x50-0x5F) =====
  sendSensorByte(0x50, t3l, 100);   // Tank 3 level
  sendSensorByte(0x51, t3t, 100);   // Tank 3 temperature
  sendSensorByte(0x52, t3p, 100);   // Tank 3 pH
  sendSensorByte(0x53, t3tds, 1);   // Tank 3 TDS
}

// ==================================================
// EVENT FEEDBACK -> RASPBERRY PI (BYTE PROTOCOL)
// Format: 0xFE [ID] [VALUE] \n
// ==================================================
// NOTE: This stays the same as before!

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
  sendByteEvent(0x09, BM_SOL_PUMP, last_BM_SOL_PUMP); // New Pump event
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

void sendByteEvent(byte id, int current, int last) {
  if (current != last) {
    Serial.write(0xFE);        // Event marker
    Serial.write(id);          // Variable ID
    Serial.write((byte)current); // Value (0 or 1)
    Serial.write('\n');        // Newline delimiter
  }
}
