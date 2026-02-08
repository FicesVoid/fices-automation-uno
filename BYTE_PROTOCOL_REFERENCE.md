# Byte Protocol Reference Guide

This document provides a complete reference for the byte-based serial communication protocol, organized by station.

---

## Protocol Format

### Incoming Commands (Pi → Arduino)

```
[ID] [VALUE] \n
```

- **ID:** 1 byte (variable identifier)
- **VALUE:** 1 byte (0 = OFF, 1 = ON, or 0-100 for recipe)
- **\n:** Newline delimiter (0x0A)

### Outgoing Events (Arduino → Pi)

```
0xFE [ID] [VALUE] \n
```

- **0xFE:** Event marker (differentiates from JSON)
- **ID:** 1 byte (variable identifier)
- **VALUE:** 1 byte (current state)
- **\n:** Newline delimiter (0x0A)

---

## System Control

| Byte ID  | Variable Name | Description          |
| :------- | :------------ | :------------------- |
| **0x01** | `SYSTEM_MODE` | 0 = Auto, 1 = Manual |

### Example Messages

**Turn to Manual Mode:**

- **Command:** `0x01 0x01 0x0A`
- **Event:** `0xFE 0x01 0x01 0x0A`

**Turn to Auto Mode:**

- **Command:** `0x01 0x00 0x0A`
- **Event:** `0xFE 0x01 0x00 0x0A`

---

## Control Room Station

| Byte ID  | Variable Name         | Description       | Pin |
| :------- | :-------------------- | :---------------- | :-- |
| **0x02** | `CR_MAIN_PUMP`        | Main Water Pump   | 40  |
| **0x03** | `CR_BUFFER_VALVE`     | Buffer Tank Valve | 44  |
| **0x04** | `CR_GREENHOUSE_VALVE` | Greenhouse Valve  | 45  |

### Example Messages

**Turn Main Pump ON:**

- **Command:** `0x02 0x01 0x0A`
- **Event:** `0xFE 0x02 0x01 0x0A`

**Turn Main Pump OFF:**

- **Command:** `0x02 0x00 0x0A`
- **Event:** `0xFE 0x02 0x00 0x0A`

**Turn Buffer Valve ON:**

- **Command:** `0x03 0x01 0x0A`
- **Event:** `0xFE 0x03 0x01 0x0A`

**Turn Greenhouse Valve ON:**

- **Command:** `0x04 0x01 0x0A`
- **Event:** `0xFE 0x04 0x01 0x0A`

---

## Buffer Mixer Station

| Byte ID  | Variable Name  | Description      | Pin |
| :------- | :------------- | :--------------- | :-- |
| **0x05** | `BM_VALVE_A`   | Solution A Valve | 46  |
| **0x06** | `BM_VALVE_B`   | Solution B Valve | 47  |
| **0x07** | `BM_VALVE_C`   | Solution C Valve | 48  |
| **0x08** | `BM_MIX_VALVE` | Mixer Valve      | 49  |
| **0x0A** | `BM_MIX_PUMP`  | Mixer Pump       | 42  |

### Example Messages

**Open Valve A:**

- **Command:** `0x05 0x01 0x0A`
- **Event:** `0xFE 0x05 0x01 0x0A`

**Close Valve B:**

- **Command:** `0x06 0x00 0x0A`
- **Event:** `0xFE 0x06 0x00 0x0A`

**Turn Mixer Pump ON:**

- **Command:** `0x0A 0x01 0x0A`
- **Event:** `0xFE 0x0A 0x01 0x0A`

---

## Mixer Module Station

| Byte ID  | Variable Name | Description         | Pin |
| :------- | :------------ | :------------------ | :-- |
| **0x0B** | `MM_SOL_PUMP` | Solution Pump       | 43  |
| **0x0C** | `MM_T1_IN`    | Tank 1 Input Valve  | 7   |
| **0x0D** | `MM_T1_OUT`   | Tank 1 Output Valve | 10  |
| **0x0E** | `MM_T2_IN`    | Tank 2 Input Valve  | 8   |
| **0x0F** | `MM_T2_OUT`   | Tank 2 Output Valve | 11  |
| **0x10** | `MM_T3_IN`    | Tank 3 Input Valve  | 9   |
| **0x11** | `MM_T3_OUT`   | Tank 3 Output Valve | 12  |

### Example Messages

**Open Tank 1 Input:**

- **Command:** `0x0C 0x01 0x0A`
- **Event:** `0xFE 0x0C 0x01 0x0A`

**Close Tank 1 Output:**

- **Command:** `0x0D 0x00 0x0A`
- **Event:** `0xFE 0x0D 0x00 0x0A`

**Open Tank 3 Input:**

- **Command:** `0x10 0x01 0x0A`
- **Event:** `0xFE 0x10 0x01 0x0A`

---

## Substation

| Byte ID  | Variable Name | Code Name     | Description        | Pin |
| :------- | :------------ | :------------ | :----------------- | :-- |
| **0x12** | `SS_PUMP`     | -             | Substation Pump    | 49  |
| **0x13** | `SUB1_VALVE`  | `SS1_VALVE_1` | Substation 1 Valve | 5   |
| **0x14** | `SUB2_VALVE`  | `SS2_VALVE_1` | Substation 2 Valve | 6   |

### Example Messages

**Turn Substation Pump ON:**

- **Command:** `0x12 0x01 0x0A`
- **Event:** `0xFE 0x12 0x01 0x0A`

**Open Substation 1 Valve:**

- **Command:** `0x13 0x01 0x0A`
- **Event:** `0xFE 0x13 0x01 0x0A`

**Close Substation 2 Valve:**

- **Command:** `0x14 0x00 0x0A`
- **Event:** `0xFE 0x14 0x00 0x0A`

---

## Recipe System

| Byte ID  | Variable Name   | Description        | Value Range              |
| :------- | :-------------- | :----------------- | :----------------------- |
| **0x20** | `RECIPE_TANK`   | Target Tank Number | 1, 2, or 3               |
| **0x21** | `RECIPE_A`      | Solution A Ratio   | 0 - 100 (%)              |
| **0x22** | `RECIPE_B`      | Solution B Ratio   | 0 - 100 (%)              |
| **0x23** | `RECIPE_C`      | Solution C Ratio   | 0 - 100 (%)              |
| **0x24** | `RECIPE_EC`     | Target EC × 10     | 0 - 255 (e.g., 15 = 1.5) |
| **0x25** | `RECIPE_WATER`  | Target Water Level | 0 - 100 (%)              |
| **0x26** | `RECIPE_START`  | Start Mixing       | 1 = Start                |
| **0x27** | `RECIPE_CANCEL` | Cancel/Stop        | 1 = Stop                 |

### Recipe Message Sequence

To send a complete recipe (e.g., Tank 1, A:40%, B:35%, C:25%, EC:1.5, Water:75%):

```
0x20 0x01 0x0A    // Set target tank to 1
0x21 0x28 0x0A    // Set A ratio to 40 (0x28 = 40)
0x22 0x23 0x0A    // Set B ratio to 35 (0x23 = 35)
0x23 0x19 0x0A    // Set C ratio to 25 (0x19 = 25)
0x24 0x0F 0x0A    // Set EC to 1.5 (0x0F = 15, ÷10 = 1.5)
0x25 0x4B 0x0A    // Set water level to 75% (0x4B = 75)
0x26 0x01 0x0A    // START mixing
```

**Note:** Recipe commands do **NOT** send event feedback. Only control variables (0x01-0x14) send events.

### Cancel Recipe

```
0x27 0x01 0x0A    // CANCEL all mixing
```

---

## Python Code Examples

### Sending Commands

```python
import serial

ser = serial.Serial('/dev/ttyUSB0', 115200)

# Turn Main Pump ON
ser.write(bytes([0x02, 0x01, 0x0A]))

# Turn Main Pump OFF
ser.write(bytes([0x02, 0x00, 0x0A]))

# Send Recipe
recipe_commands = [
    [0x20, 0x01, 0x0A],  # Tank 1
    [0x21, 40, 0x0A],    # A: 40%
    [0x22, 35, 0x0A],    # B: 35%
    [0x23, 25, 0x0A],    # C: 25%
    [0x24, 15, 0x0A],    # EC: 1.5
    [0x25, 75, 0x0A],    # Water: 75%
    [0x26, 0x01, 0x0A],  # START
]

for cmd in recipe_commands:
    ser.write(bytes(cmd))
```

### Receiving Events

```python
def read_serial():
    while True:
        byte = ser.read(1)[0]

        if byte == 0xFE:  # Event
            id = ser.read(1)[0]
            value = ser.read(1)[0]
            ser.read(1)  # Consume newline

            print(f"Event: ID={hex(id)}, Value={value}")

        elif byte == 0x7B:  # JSON (starts with '{')
            json_str = chr(byte) + ser.read_until(b'\n').decode()
            print(f"JSON: {json_str}")
```

---

## Quick Reference Table (All IDs)

| ID   | Variable            | ID   | Variable    | ID   | Variable      |
| :--- | :------------------ | :--- | :---------- | :--- | :------------ |
| 0x01 | SYSTEM_MODE         | 0x0B | MM_SOL_PUMP | 0x21 | RECIPE_A      |
| 0x02 | CR_MAIN_PUMP        | 0x0C | MM_T1_IN    | 0x22 | RECIPE_B      |
| 0x03 | CR_BUFFER_VALVE     | 0x0D | MM_T1_OUT   | 0x23 | RECIPE_C      |
| 0x04 | CR_GREENHOUSE_VALVE | 0x0E | MM_T2_IN    | 0x24 | RECIPE_EC     |
| 0x05 | BM_VALVE_A          | 0x0F | MM_T2_OUT   | 0x25 | RECIPE_WATER  |
| 0x06 | BM_VALVE_B          | 0x10 | MM_T3_IN    | 0x26 | RECIPE_START  |
| 0x07 | BM_VALVE_C          | 0x11 | MM_T3_OUT   | 0x27 | RECIPE_CANCEL |
| 0x08 | BM_MIX_VALVE        | 0x12 | SS_PUMP     |      |               |
| 0x0A | BM_MIX_PUMP         | 0x13 | SUB1_VALVE  |      |               |
|      |                     | 0x14 | SUB2_VALVE  |      |               |
|      |                     | 0x20 | RECIPE_TANK |      |               |
