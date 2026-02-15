// Sensor State Machine Definitions
enum SensorState {
  SENSOR_IDLE,
  SENSOR_TEMP_REQ,
  SENSOR_WAIT_TEMP,
  SENSOR_READ_TEMP,
  SENSOR_WAIT_PH_STABLE,
  SENSOR_READ_PH,
  SENSOR_WAIT_EC_STABLE,
  SENSOR_READ_EC
};

SensorState sensorState = SENSOR_IDLE;
unsigned long sensorTimer = 0;
const unsigned long SENSOR_INTERVAL = 1000; // Read every 1s (agile)

void moduleSensorMaster() {
  unsigned long currentMillis = millis();

  switch (sensorState) {
    case SENSOR_IDLE:
      if (currentMillis - sensorTimer >= SENSOR_INTERVAL) {
        sensorState = SENSOR_TEMP_REQ;
      }
      break;

    case SENSOR_TEMP_REQ:
      ds18b20.requestTemperatures(); // Async request (takes ~750ms for 12-bit)
      sensorTimer = currentMillis;   // Start wait timer
      sensorState = SENSOR_WAIT_TEMP;
      break;

    case SENSOR_WAIT_TEMP:
      if (currentMillis - sensorTimer >= 800) { // Wait 800ms for conversion
        sensorState = SENSOR_READ_TEMP;
      }
      break;

    case SENSOR_READ_TEMP:
      /* ===== WATER TEMP ===== */
      temp = ds18b20.getTempCByIndex(0);
      t1t  = ds18b20.getTempCByIndex(1);
      t2t  = ds18b20.getTempCByIndex(2);
      t3t  = ds18b20.getTempCByIndex(3);

      // Temperature Guard
      if (temp < -50 || temp > 100) temp = 25.0;
      if (t1t < -50  || t1t > 100)  t1t  = 25.0;
      if (t2t < -50  || t2t > 100)  t2t  = 25.0;
      if (t3t < -50  || t3t > 100)  t3t  = 25.0;

      /* ===== PREPARE PH (EC Power OFF) ===== */
      digitalWrite(PIN_EC_PWR, LOW); 
      sensorTimer = currentMillis;
      sensorState = SENSOR_WAIT_PH_STABLE;
      break;

    case SENSOR_WAIT_PH_STABLE:
      if (currentMillis - sensorTimer >= 50) { // Short settle time
        sensorState = SENSOR_READ_PH;
      }
      break;

    case SENSOR_READ_PH:
      ph  = readPH(ph_master, PH1_PIN,   temp, PH_OFFSET_MASTER);
      t1p = readPH(ph_t1,     T1_PH_PIN, t1t,  PH_OFFSET_T1);
      t2p = readPH(ph_t2,     T2_PH_PIN, t2t,  PH_OFFSET_T2);
      t3p = readPH(ph_t3,     T3_PH_PIN, t3t,  PH_OFFSET_T3);

      /* ===== PREPARE EC (EC Power ON) ===== */
      digitalWrite(PIN_EC_PWR, HIGH);
      sensorTimer = currentMillis;
      sensorState = SENSOR_WAIT_EC_STABLE;
      break;

    case SENSOR_WAIT_EC_STABLE:
      if (currentMillis - sensorTimer >= 500) { // Stabilization time
        sensorState = SENSOR_READ_EC;
      }
      break;

    case SENSOR_READ_EC:
      /* ===== EC & TDS ===== */
      ec  = readEC(EC1_PIN, temp);       
      tds = readTDS(TDS1_PIN, temp);     

      t1tds = readTDS(T1_TDS_PIN, t1t);
      t2tds = readTDS(T2_TDS_PIN, t2t);
      t3tds = readTDS(T3_TDS_PIN, t3t);

      /* ===== ULTRASONIC ===== */
      // Reads are fast enough to keep inline or could split further
      va  = readUltrasonicCM(VA_TRIG_PIN, VA_ECHO_PIN); 
      vb  = readUltrasonicCM(VB_TRIG_PIN, VB_ECHO_PIN); 
      vc  = readUltrasonicCM(VC_TRIG_PIN, VC_ECHO_PIN); 
      mtl = readUltrasonicCM(MTL_TRIG_PIN, MTL_ECHO_PIN); 
      btl = readUltrasonicCM(BTL_TRIG_PIN, BTL_ECHO_PIN); 
      t1l = readUltrasonicCM(T1_TRIG_PIN, T1_ECHO_PIN); 
      t2l = readUltrasonicCM(T2_TRIG_PIN, T2_ECHO_PIN); 
      t3l = readUltrasonicCM(T3_TRIG_PIN, T3_ECHO_PIN);

      /* ===== CONVERT TO % ===== */
      mtl = levelToPercent(mtl, 10, 150);
      btl = levelToPercent(btl, 10, 120);
      va  = levelToPercent(va, 10, 80);
      vb  = levelToPercent(vb, 10, 80);
      vc  = levelToPercent(vc, 10, 80);
      t1l = levelToPercent(t1l, 10, 100);
      t2l = levelToPercent(t2l, 10, 100);
      t3l = levelToPercent(t3l, 10, 100);

      // Done!
      sensorTimer = currentMillis; // Reset idle timer base
      sensorState = SENSOR_IDLE;
      break;
  }
}
