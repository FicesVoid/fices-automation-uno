// Helper function to sort array for median filter (long)
void sortArray(long a[], int size) {
  for(int i=0; i<(size-1); i++) {
    for(int o=0; o<(size-(i+1)); o++) {
      if(a[o] > a[o+1]) {
        long t = a[o];
        a[o] = a[o+1];
        a[o+1] = t;
      }
    }
  }
}

// Helper function to sort array for median filter (int)
void sortArrayInt(int a[], int size) {
  for(int i=0; i<(size-1); i++) {
    for(int o=0; o<(size-(i+1)); o++) {
      if(a[o] > a[o+1]) {
        int t = a[o];
        a[o] = a[o+1];
        a[o+1] = t;
      }
    }
  }
}

int readTankPercentage(int trigPin, int echoPin, float tankHeightCm) {
  const int numSamples = 5;
  long raw_echo_us[numSamples];
  int validSamples = 0;

  // STEP 1 - Read 5 samples
  for (int i = 0; i < numSamples; i++) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Timeout ~25000us (max ~400cm distance)
    long duration = pulseIn(echoPin, HIGH, 25000); 

    // Clean data: Ignore values < 1000us (~17cm min distance to ignore deadzone) or > 25000us
    // We'll relax the minimum slightly in case tank is totally full to 500us (~8.5cm).
    if (duration >= 500 && duration <= 25000) {
      raw_echo_us[validSamples] = duration;
      validSamples++;
    }
    delay(5); // Small delay between pings
  }

  if (validSamples == 0) return -1; // Sensor error / timeout

  // STEP 2 - Remove outliers by finding the median
  sortArray(raw_echo_us, validSamples);
  long median_echo_us;
  if (validSamples % 2 == 0) {
    median_echo_us = (raw_echo_us[(validSamples / 2) - 1] + raw_echo_us[validSamples / 2]) / 2;
  } else {
    median_echo_us = raw_echo_us[validSamples / 2];
  }

  // STEP 3 - Convert time to distance
  float distance_cm = median_echo_us / 58.0;

  // STEP 4 - Calculate water level
  float water_level_cm = tankHeightCm - distance_cm;
  if (water_level_cm < 0) water_level_cm = 0;

  // STEP 5 - Calculate tank percentage
  // We use tankHeightCm as the max possible depth capacity.
  float tank_percentage = (water_level_cm / tankHeightCm) * 100.0;
  
  // Clamp heavily
  if (tank_percentage > 100.0) tank_percentage = 100.0;
  if (tank_percentage < 0.0) tank_percentage = 0.0;

  // STEP 6 - Return rounded integer
  return round(tank_percentage);
}
float readPH(DFRobot_PH &ph_obj, int pin, float temperature, float offset) {
  const int SCOUNT = 30; // 30 samples for better stability
  int analogBuffer[SCOUNT];
  
  // Read samples with delay
  for (int i = 0; i < SCOUNT; i++) {
    analogBuffer[i] = analogRead(pin);
    yieldDelay(10); // 10ms between samples => 300ms total read time
  }
  
  // Sort for median filter
  sortArrayInt(analogBuffer, SCOUNT);
  
  // Average the middle 10 samples (ignore extremes)
  long sum = 0;
  for (int i = 10; i < 20; i++) {
    sum += analogBuffer[i];
  }
  
  float voltage = (sum / 10.0) / 1024.0 * 5000.0; // millivolts
  return ph_obj.readPH(voltage, temperature) + offset;
}

float readEC(int pin, float temperature) {
  const int SCOUNT = 30;
  int analogBuffer[SCOUNT];
  
  for (int i = 0; i < SCOUNT; i++) {
    analogBuffer[i] = analogRead(pin);
    yieldDelay(10);
  }
  
  sortArrayInt(analogBuffer, SCOUNT);
  
  long sum = 0;
  for (int i = 10; i < 20; i++) {
    sum += analogBuffer[i];
  }

  // Convert to voltage millivolts (DFRobot Standard)
  float voltage = (sum / 10.0) / 1024.0 * 5000.0; 

  // Use the official DFRobot_EC library math
  float ecValue = ec_master.readEC(voltage, temperature);

  // Apply software K-value for fine-tuning
  return ecValue * EC_K_VALUE;
}

float readTDS(int pin, float temperature) {
  const int SCOUNT = 30;
  int analogBuffer[SCOUNT];
  
  for (int i = 0; i < SCOUNT; i++) {
    analogBuffer[i] = analogRead(pin);
    yieldDelay(10);
  }
  
  sortArrayInt(analogBuffer, SCOUNT);
  
  long sum = 0;
  for (int i = 10; i < 20; i++) {
    sum += analogBuffer[i];
  }
  
  float voltage = (sum / 10.0) / 1024.0 * 5.0; // voltage in Volts

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

