float readUltrasonicCM(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reduced timeout to 23529us (~4m max distance) to prevent long blocks
  long duration = pulseIn(echoPin, HIGH, 23529); 
  if (duration == 0) return -1;

  float distance = duration * 0.034 / 2;
  return distance; // cm
}
float readPH(DFRobot_PH &ph_obj, int pin, float temperature, float offset) {
  long sum = 0;
  // Reduce to 5 samples, faster interval, use yieldDelay
  for (int i = 0; i < 5; i++) {
    sum += analogRead(pin);
    yieldDelay(2); 
  }
  float voltage = (sum / 5.0) / 1024.0 * 5000.0; // millivolts
  return ph_obj.readPH(voltage, temperature) + offset;
}

float readEC(int pin, float temperature) {
  long sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += analogRead(pin);
    yieldDelay(2);
  }
  // Convert to voltage millivolts (DFRobot Standard)
  float voltage = (sum / 5.0) / 1024.0 * 5000.0; 

  // Use the official DFRobot_EC library math
  float ecValue = ec_master.readEC(voltage, temperature);

  // Apply software K-value for fine-tuning
  return ecValue * EC_K_VALUE;
}

float readTDS(int pin, float temperature) {
  long sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += analogRead(pin);
    yieldDelay(2);
  }
  float voltage = (sum / 5.0) / 1024.0 * 5.0; // voltage in Volts

  // DFRobot TDS Temperature Compensation
  float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
  if (compensationCoefficient <= 0.1) compensationCoefficient = 0.1; // Safety guard

  float compensationVoltage = voltage / compensationCoefficient;

  // DFRobot TDS Calculation Formula
  float tdsValue = (133.42 * pow(compensationVoltage, 3) - 255.86 * pow(compensationVoltage, 2) + 857.39 * compensationVoltage);
  if (tdsValue < 0) tdsValue = 0; // Prevent negative TDS
  return tdsValue;
}
byte simpleCRC(const String &s) {
  byte crc = 0;
  for (unsigned int i = 0; i < s.length(); i++) {
    crc ^= s[i];
  }
  return crc;
}
float levelToPercent(float cm, float minCm, float maxCm) {
  if (cm < 0) return -1; // sensor error

  if (cm <= minCm) return 100.0;
  if (cm >= maxCm) return 0.0;

  float pct = (maxCm - cm) * 100.0 / (maxCm - minCm);
  return constrain(pct, 0, 100);
}

