// ==================================================
// EEPROM_MODULE.ino
// STATE PERSISTENCE + HEARTBEAT SYNC
// ==================================================
// EEPROM Layout:
//   0x00: Magic byte (0xAA = valid data exists)
//   0x01: SYSTEM_MODE
//   0x02: CR_MAIN_PUMP
//   0x03: CR_BUFFER_VALVE
//   0x04: CR_GREENHOUSE_VALVE
//   0x05: BM_DOSING_A
//   0x06: BM_DOSING_B
//   0x07: BM_DOSING_C
//   0x08: BM_MIX_VALVE
//   0x09: BM_MIX_PUMP
//   0x0A: MM_SUB_PUMP
//   0x0B: MM_T1_IN
//   0x0C: MM_T1_OUT
//   0x0D: MM_T2_IN
//   0x0E: MM_T2_OUT
//   0x0F: MM_T3_IN
//   0x10: MM_T3_OUT
//   0x11: SUB1_VALVE
//   0x12: SUB2_VALVE
// ==================================================

#define EEPROM_MAGIC_BYTE   0xAA
#define EEPROM_ADDR_MAGIC   0x00
#define EEPROM_ADDR_START   0x01

// Heartbeat interval (30 seconds)
#define HEARTBEAT_INTERVAL  30000UL
unsigned long lastHeartbeat = 0;

// ==================================================
// LOAD STATE FROM EEPROM (called in setup())
// ==================================================
void loadStateFromEEPROM() {
  byte magic = EEPROM.read(EEPROM_ADDR_MAGIC);
  
  if (magic != EEPROM_MAGIC_BYTE) {
    // First boot or corrupted EEPROM — initialize with all OFF
    Serial.println("EEPROM: first boot, initializing defaults");
    SYSTEM_MODE = 1;  // Default to Manual mode
    
    // Write magic byte + defaults
    EEPROM.update(EEPROM_ADDR_MAGIC, EEPROM_MAGIC_BYTE);
    saveStateToEEPROM();
    return;
  }
  
  // Valid EEPROM data — restore states
  byte addr = EEPROM_ADDR_START;
  
  SYSTEM_MODE          = EEPROM.read(addr++);
  CR_MAIN_PUMP         = EEPROM.read(addr++);
  CR_BUFFER_VALVE      = EEPROM.read(addr++);
  CR_GREENHOUSE_VALVE  = EEPROM.read(addr++);
  BM_DOSING_A          = EEPROM.read(addr++);
  BM_DOSING_B          = EEPROM.read(addr++);
  BM_DOSING_C          = EEPROM.read(addr++);
  BM_MIX_VALVE         = EEPROM.read(addr++);
  BM_MIX_PUMP          = EEPROM.read(addr++);
  MM_SUB_PUMP          = EEPROM.read(addr++);
  MM_T1_IN             = EEPROM.read(addr++);
  MM_T1_OUT            = EEPROM.read(addr++);
  MM_T2_IN             = EEPROM.read(addr++);
  MM_T2_OUT            = EEPROM.read(addr++);
  MM_T3_IN             = EEPROM.read(addr++);
  MM_T3_OUT            = EEPROM.read(addr++);
  SS1_VALVE_1          = EEPROM.read(addr++);
  SS2_VALVE_1          = EEPROM.read(addr++);
  
  Serial.println("EEPROM: loaded saved states");
  Serial.print("  SYSTEM_MODE="); Serial.println(SYSTEM_MODE);
}

// ==================================================
// SAVE STATE TO EEPROM (called on state changes)
// Uses EEPROM.update() — only writes if value changed
// ==================================================
void saveStateToEEPROM() {
  byte addr = EEPROM_ADDR_START;
  
  EEPROM.update(addr++, SYSTEM_MODE);
  EEPROM.update(addr++, CR_MAIN_PUMP);
  EEPROM.update(addr++, CR_BUFFER_VALVE);
  EEPROM.update(addr++, CR_GREENHOUSE_VALVE);
  EEPROM.update(addr++, BM_DOSING_A);
  EEPROM.update(addr++, BM_DOSING_B);
  EEPROM.update(addr++, BM_DOSING_C);
  EEPROM.update(addr++, BM_MIX_VALVE);
  EEPROM.update(addr++, BM_MIX_PUMP);
  EEPROM.update(addr++, MM_SUB_PUMP);
  EEPROM.update(addr++, MM_T1_IN);
  EEPROM.update(addr++, MM_T1_OUT);
  EEPROM.update(addr++, MM_T2_IN);
  EEPROM.update(addr++, MM_T2_OUT);
  EEPROM.update(addr++, MM_T3_IN);
  EEPROM.update(addr++, MM_T3_OUT);
  EEPROM.update(addr++, SS1_VALVE_1);
  EEPROM.update(addr++, SS2_VALVE_1);
}

// ==================================================
// HEARTBEAT — Send ALL current states to Pi (every 30s)
// Format: 0xFC [COUNT] [ID1 VAL1] [ID2 VAL2] ... [CHECKSUM] \n
// ==================================================
void sendStateHeartbeat() {
  if (millis() - lastHeartbeat < HEARTBEAT_INTERVAL) return;
  lastHeartbeat = millis();
  
  // 19 state entries (including SYSTEM_MODE)
  const byte count = 19;
  
  // Build pairs: [byte_id, state_value]
  // Using the same IDs as BYTE_ID_MAP in serial_manager.py
  byte pairs[] = {
    0x01, (byte)SYSTEM_MODE,
    0x02, (byte)state_CR_MAIN_PUMP,
    0x03, (byte)state_CR_BUFFER_VALVE,
    0x04, (byte)state_CR_GREENHOUSE_VALVE,
    0x05, (byte)state_BM_DOSING_A,
    0x06, (byte)state_BM_DOSING_B,
    0x07, (byte)state_BM_DOSING_C,
    0x08, (byte)state_BM_MIX_VALVE,
    0x0A, (byte)state_BM_MIX_PUMP,
    0x0B, (byte)state_MM_SUB_PUMP,
    0x0C, (byte)state_MM_T1_IN,
    0x0D, (byte)state_MM_T1_OUT,
    0x0E, (byte)state_MM_T2_IN,
    0x0F, (byte)state_MM_T2_OUT,
    0x10, (byte)state_MM_T3_IN,
    0x11, (byte)state_MM_T3_OUT,
    0x13, (byte)state_SS1_VALVE_1,
    0x14, (byte)state_SS2_VALVE_1
  };
  // Note: 18 devices in pairs array (SYSTEM_MODE + 17 outputs = 18 entries, 36 bytes)
  // But count = 19 was wrong, let's fix: we have 18 entries above
  // Actually counting: SYSTEM_MODE + CR(3) + BM(5) + MM(7) + SS(2) = 18
  
  byte actualCount = 18;
  byte pairBytes = actualCount * 2;
  
  // Calculate checksum (XOR of all pair bytes)
  byte checksum = 0xFC ^ actualCount;
  for (byte i = 0; i < pairBytes; i++) {
    checksum ^= pairs[i];
  }
  
  // Send packet
  Serial.write(0xFC);           // Heartbeat marker
  Serial.write(actualCount);    // Number of state entries
  Serial.write(pairs, pairBytes); // All ID+VALUE pairs
  Serial.write(checksum);       // Checksum
  Serial.write('\n');           // Delimiter
}
