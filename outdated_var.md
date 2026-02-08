# Arduino Serial Communication Variables Reference

This document lists all serial communication variables for each station. Use these variable names in your Arduino code to receive commands from the automation system.

## Communication Protocol

**Format:** `VARIABLE:VALUE\n`

- Commands are sent as plain text
- Each command ends with a newline character (`\n`)
- Boolean values are sent as `1` (true) or `0` (false)
- Example: `BM_VALVE_A:1\n` means "turn on Buffer Mixer Valve A"

## Sensor Data Format

Arduino should send sensor data as JSON strings:

- Format: `{"key1":value1,"key2":value2,...}\n`
- Send one JSON object per line
- Include both sensor readings and current control states

---

## 1. Buffer Mixer Station

**Station Name:** `buffer_mixer`  
**COM Port:** Configure in `init_serial.py`

### Control Variables (Receive from Automation)

| Variable       | Type       | Description                    |
| -------------- | ---------- | ------------------------------ |
| `BM_VALVE_A`   | bool (0/1) | Valve A control                |
| `BM_VALVE_B`   | bool (0/1) | Valve B control                |
| `BM_VALVE_C`   | bool (0/1) | Valve C control                |
| `BM_SOL_PUMP`  | bool (0/1) | Solution pump control          |
| `BM_MIX_PUMP`  | bool (0/1) | Mixer pump control             |
| `BM_MIX_VALVE` | bool (0/1) | Mixer valve control            |
| `MODE`         | bool (0/1) | System mode (1=manual, 0=auto) |

### Sensor Data (Send to Automation)

**JSON Format:**

```json
{
  "temp": 22.5,
  "ph": 6.5,
  "ec": 1.2,
  "level": 75.3,
  "va": 1,
  "vb": 0,
  "vc": 1,
  "sp": 1,
  "mp": 0,
  "mv": 1
}
```

**Keys:**

- `temp` - Temperature (°C)
- `ph` - pH value
- `ec` - Electrical conductivity
- `level` - Water level (%)
- `va` - Valve A state (0/1)
- `vb` - Valve B state (0/1)
- `vc` - Valve C state (0/1)
- `sp` - Solution pump state (0/1)
- `mp` - Mixer pump state (0/1)
- `mv` - Mixer valve state (0/1)

---

## 2. Mixer Module Station

**Station Name:** `mixer_module`  
**COM Port:** Configure in `init_serial.py`

### Control Variables (Receive from Automation)

| Variable      | Type       | Description                    |
| ------------- | ---------- | ------------------------------ |
| `MM_SOL_PUMP` | bool (0/1) | Solution pump control          |
| `MM_T1_IN`    | bool (0/1) | Tank 1 input valve             |
| `MM_T1_OUT`   | bool (0/1) | Tank 1 output valve            |
| `MM_T2_IN`    | bool (0/1) | Tank 2 input valve             |
| `MM_T2_OUT`   | bool (0/1) | Tank 2 output valve            |
| `MM_T3_IN`    | bool (0/1) | Tank 3 input valve             |
| `MM_T3_OUT`   | bool (0/1) | Tank 3 output valve            |
| `MODE`        | bool (0/1) | System mode (1=manual, 0=auto) |

### Sensor Data (Send to Automation)

**JSON Format:**

```json
{
  "t1l": 65.0,
  "t1e": 1.2,
  "t1t": 22.0,
  "t1p": 6.5,
  "t2l": 40.0,
  "t2e": 1.5,
  "t2t": 24.0,
  "t2p": 7.2,
  "t3l": 80.0,
  "t3e": 0.9,
  "t3t": 21.0,
  "t3p": 5.8,
  "sp": 1,
  "t1i": 0,
  "t1o": 1,
  "t2i": 0,
  "t2o": 0,
  "t3i": 1,
  "t3o": 0
}
```

**Keys:**

- `t1l` - Tank 1 level (%)
- `t1e` - Tank 1 EC
- `t1t` - Tank 1 temperature (°C)
- `t1p` - Tank 1 pH
- `t2l` - Tank 2 level (%)
- `t2e` - Tank 2 EC
- `t2t` - Tank 2 temperature (°C)
- `t2p` - Tank 2 pH
- `t3l` - Tank 3 level (%)
- `t3e` - Tank 3 EC
- `t3t` - Tank 3 temperature (°C)
- `t3p` - Tank 3 pH
- `sp` - Solution pump state (0/1)
- `t1i` - Tank 1 input valve state (0/1)
- `t1o` - Tank 1 output valve state (0/1)
- `t2i` - Tank 2 input valve state (0/1)
- `t2o` - Tank 2 output valve state (0/1)
- `t3i` - Tank 3 input valve state (0/1)
- `t3o` - Tank 3 output valve state (0/1)

---

## 3. Control Room Station

**Station Name:** `control_room`  
**COM Port:** Configure in `init_serial.py`

### Control Variables (Receive from Automation)

| Variable   | Type       | Description                    |
| ---------- | ---------- | ------------------------------ |
| `CR_BTN_1` | bool (0/1) | Button 1 control               |
| `CR_BTN_2` | bool (0/1) | Button 2 control               |
| `MODE`     | bool (0/1) | System mode (1=manual, 0=auto) |

### Sensor Data (Send to Automation)

**JSON Format:**

```json
{ "mtl": 88.5, "btl": 75.2, "gtl": 45.8, "bv": 1, "gv": 0, "mp": 1 }
```

**Keys:**

- `mtl` - Main tank level (%)
- `btl` - Buffer tank level (%)
- `gtl` - Greenhouse tank level (%)
- `bv` - Buffer valve state (0/1)
- `gv` - Greenhouse valve state (0/1)
- `mp` - Main pump state (0/1)

---

## 4. Substation 1

**Station Name:** `substation_1`  
**COM Port:** Configure in `init_serial.py`

### Control Variables (Receive from Automation)

| Variable      | Type       | Description                    |
| ------------- | ---------- | ------------------------------ |
| `SS1_VALVE_1` | bool (0/1) | Valve 1 control                |
| `SS1_PUMP_1`  | bool (0/1) | Pump 1 control                 |
| `MODE`        | bool (0/1) | System mode (1=manual, 0=auto) |

### Sensor Data (Send to Automation)

**JSON Format:**

```json
{
  "et": 28.5,
  "eh": 65.0,
  "st": 22.4,
  "sm": 42.0,
  "sp": 6.8,
  "sn": 45.0,
  "sph": 38.0,
  "sk": 52.0,
  "se": 1.4,
  "iv": 0,
  "fp": 0
}
```

**Keys:**

- `et` - Environment temperature (°C)
- `eh` - Environment humidity (%)
- `st` - Soil temperature (°C)
- `sm` - Soil moisture (%)
- `sp` - Soil pH
- `sn` - Soil nitrogen (ppm)
- `sph` - Soil phosphorus (ppm)
- `sk` - Soil potassium (ppm)
- `se` - Soil EC
- `iv` - Irrigation valve state (0/1)
- `fp` - Fertilizer pump state (0/1)

---

## 5. Substation 2

**Station Name:** `substation_2`  
**COM Port:** Configure in `init_serial.py`

### Control Variables (Receive from Automation)

| Variable      | Type       | Description                    |
| ------------- | ---------- | ------------------------------ |
| `SS2_VALVE_1` | bool (0/1) | Valve 1 control                |
| `SS2_PUMP_1`  | bool (0/1) | Pump 1 control                 |
| `MODE`        | bool (0/1) | System mode (1=manual, 0=auto) |

### Sensor Data (Send to Automation)

**JSON Format:**

```json
{
  "et": 26.8,
  "eh": 72.0,
  "st": 21.5,
  "sm": 55.0,
  "sp": 6.2,
  "sn": 48.0,
  "sph": 42.0,
  "sk": 55.0,
  "se": 1.1,
  "iv": 0,
  "fp": 0
}
```

**Keys:**

- `et` - Environment temperature (°C)
- `eh` - Environment humidity (%)
- `st` - Soil temperature (°C)
- `sm` - Soil moisture (%)
- `sp` - Soil pH
- `sn` - Soil nitrogen (ppm)
- `sph` - Soil phosphorus (ppm)
- `sk` - Soil potassium (ppm)
- `se` - Soil EC
- `iv` - Irrigation valve state (0/1)
- `fp` - Fertilizer pump state (0/1)

---

## Arduino Example Code

### Receiving Commands

```cpp
void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');

    // Parse command (format: VARIABLE:VALUE)
    int colonIndex = command.indexOf(':');
    if (colonIndex > 0) {
      String variable = command.substring(0, colonIndex);
      int value = command.substring(colonIndex + 1).toInt();

      // Handle commands
      if (variable == "BM_VALVE_A") {
        digitalWrite(VALVE_A_PIN, value);
      }
      else if (variable == "BM_VALVE_B") {
        digitalWrite(VALVE_B_PIN, value);
      }
      else if (variable == "MODE") {
        systemMode = value; // 1=manual, 0=auto
      }
      // Add more handlers...
    }
  }
}
```

### Sending Sensor Data

```cpp
void sendSensorData() {
  // Create JSON string
  String json = "{";
  json += "\"temp\":" + String(temperature, 1) + ",";
  json += "\"ph\":" + String(ph, 1) + ",";
  json += "\"ec\":" + String(ec, 1) + ",";
  json += "\"level\":" + String(waterLevel, 1) + ",";
  json += "\"va\":" + String(digitalRead(VALVE_A_PIN)) + ",";
  json += "\"vb\":" + String(digitalRead(VALVE_B_PIN)) + ",";
  json += "\"vc\":" + String(digitalRead(VALVE_C_PIN)) + ",";
  json += "\"sp\":" + String(digitalRead(SOL_PUMP_PIN)) + ",";
  json += "\"mp\":" + String(digitalRead(MIX_PUMP_PIN)) + ",";
  json += "\"mv\":" + String(digitalRead(MIX_VALVE_PIN));
  json += "}";

  Serial.println(json);
}
```

---

## Setup Instructions

1. **Configure COM Ports**: Edit `init_serial.py` and set the correct COM port for each station
2. **Upload Arduino Code**: Use the variable names from this document in your Arduino sketches
3. **Test Connection**: Run `python init_serial.py` to test serial connections
4. **Start Automation**: Run `python main.py` to start the automation system

## Notes

- The automation system will continue to work with dummy data if serial connections fail
- All serial communication is thread-safe
- Baud rate is set to 9600 by default (configurable in `init_serial.py`)
- Commands are sent immediately when received from the backend
- Sensor data should be sent periodically (e.g., every 1-5 seconds)
