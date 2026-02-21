#include <OneWire.h>
#include <DallasTemperature.h>
#include <avr/wdt.h>  // reset jika hang >10s
#include <EEPROM.h>
#include "DFRobot_PH.h"
#include "DFRobot_EC.h"

/* =================================================
   [1] PIN DEFINITION 
   ================================================= */

/* ===== CONTROL ROOM PINS ===== */
#define PIN_CR_MAIN_PUMP        40
#define PIN_CR_BUFFER_VALVE     44
#define PIN_CR_GREENHOUSE_VALVE 45

/* ===== BUFFER MIXER PINS ===== */
#define PIN_BM_MIX_PUMP    42  // Shared Pin
#define PIN_BM_VALVE_A     46
#define PIN_BM_VALVE_B     47
#define PIN_BM_VALVE_C     48
#define PIN_BM_MIX_VALVE   49

/* ===== MIXER MODULE PINS ===== */
#define PIN_MM_SOL_PUMP    39  // Dedicated Pin (previously shared with BM_MIX_PUMP on 42)
#define PIN_MM_T1_IN       7
#define PIN_MM_T1_OUT      10
#define PIN_MM_T2_IN       8
#define PIN_MM_T2_OUT      11
#define PIN_MM_T3_IN       9
#define PIN_MM_T3_OUT      12

/* ===== SUBSTATION / GH ===== */
#define PIN_SS_PUMP        43
#define PIN_SS1_VALVE_1    5
#define PIN_SS2_VALVE_1    6


// -------- Master Sensor --------1
#define TEMP_PIN           38
#define PH1_PIN            A0
#define EC1_PIN            A8
#define TDS1_PIN           A7
#define PIN_EC_PWR         4  // Power control for EC sensor (Interference Mitigation)

#define VA_TRIG_PIN        32
#define VA_ECHO_PIN        33
#define VB_TRIG_PIN        34
#define VB_ECHO_PIN        35
#define VC_TRIG_PIN        36
#define VC_ECHO_PIN        37

//main tank and buffer tank 1

#define MTL_TRIG_PIN         22
#define MTL_ECHO_PIN         23
#define BTL_TRIG_PIN         50
#define BTL_ECHO_PIN         25

#define ONE_WIRE_BUS 38

DFRobot_PH ph_master, ph_t1, ph_t2, ph_t3;
DFRobot_EC ec_master;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);

// -------- Mixer Module Sensor --------
#define T1_TRIG_PIN        26
#define T1_ECHO_PIN        27
#define T1_TDS_PIN         A9
#define T1_PH_PIN          A1

#define T2_TRIG_PIN        28
#define T2_ECHO_PIN        29
#define T2_TDS_PIN         A10
#define T2_PH_PIN          A2

#define T3_TRIG_PIN        30
#define T3_ECHO_PIN        31
#define T3_TDS_PIN         A11
#define T3_PH_PIN          A3

// struct SlaveData removed
/* =================================================
   [2] GLOBAL VARIABLE
   ================================================= */

/* ===== MODE ===== */
bool SYSTEM_MODE = 1; // 0=Auto, 1=Manual

/* ===== CONTROL VARIABLE ===== */
// Control Room
// Control Room
bool CR_BUFFER_VALVE = 0;
bool CR_GREENHOUSE_VALVE = 0;
bool CR_MAIN_PUMP = 0;

// Buffer Mixer
bool BM_VALVE_A = 0;
bool BM_VALVE_B = 0;
bool BM_VALVE_C = 0;
bool BM_MIX_VALVE = 0;
bool BM_MIX_PUMP = 0;

// Mixer Module
bool MM_SOL_PUMP = 0;
bool MM_T1_IN = 0;
bool MM_T1_OUT = 0;
bool MM_T2_IN = 0;
bool MM_T2_OUT = 0;
bool MM_T3_IN = 0;
bool MM_T3_OUT = 0;

// Others
bool SS_PUMP = 0;
bool SS1_VALVE_1 = 0;
bool SS2_VALVE_1 = 0;


// -------- MASTER SENSOR --------
float temp, ph, ec, tds, level;
float PH_OFFSET_MASTER = -0.18; 
float PH_OFFSET_T1     = -0.12;
float PH_OFFSET_T2     =  0.36;
float PH_OFFSET_T3     =  0.12; 
float EC_K_VALUE       =  1.00; // Software K-value for EC calibration (default 1.00)
int va, vb, vc; // Ultrasonic Volumes

// -------- STATUS / FEEDBACK --------
/* ===== EVENT LAST STATE (GLOBAL) ===== */
bool last_SYSTEM_MODE = 1;


// Control Room
bool last_CR_BUFFER_VALVE = 0;
bool last_CR_GREENHOUSE_VALVE = 0;
bool last_CR_MAIN_PUMP = 0;

// Buffer Mixer
bool last_BM_VALVE_A = 0;
bool last_BM_VALVE_B = 0;
bool last_BM_VALVE_C = 0;
bool last_BM_MIX_VALVE = 0;
bool last_BM_MIX_PUMP = 0;

// Mixer Module
bool last_MM_SOL_PUMP = 0;
bool last_MM_T1_IN = 0;
bool last_MM_T1_OUT = 0;
bool last_MM_T2_IN = 0;
bool last_MM_T2_OUT = 0;
bool last_MM_T3_IN = 0;
bool last_MM_T3_OUT = 0;

// Substation
bool last_SS_PUMP = 0;
bool last_SS1_VALVE_1 = 0;
bool last_SS2_VALVE_1 = 0;


// Control Room
int state_CR_BUFFER_VALVE = 0;
int state_CR_GREENHOUSE_VALVE = 0;
int state_CR_MAIN_PUMP = 0;

// Buffer Mixer
int state_BM_VALVE_A = 0;
int state_BM_VALVE_B = 0;
int state_BM_VALVE_C = 0;
int state_BM_MIX_VALVE = 0;
int state_BM_MIX_PUMP = 0;

// Mixer Module
int state_MM_SOL_PUMP = 0;
int state_MM_T1_IN = 0;
int state_MM_T1_OUT = 0;
int state_MM_T2_IN = 0;
int state_MM_T2_OUT = 0;
int state_MM_T3_IN = 0;
int state_MM_T3_OUT = 0;

// Substation
int state_SS_PUMP = 0;
int state_SS1_VALVE_1 = 0;
int state_SS2_VALVE_1 = 0;


// -------- MIXER MODULE SENSOR --------
float t1l, t1tds, t1t, t1p;
float t2l, t2tds, t2t, t2p;
float t3l, t3tds, t3t, t3p;
float mtl, btl; 


// Slave sensor variables removed

// -------- RECIPE VARIABLES --------
byte RECIPE_TANK = 0;    // Target tank (1, 2, or 3)
byte RECIPE_A = 0;       // Solution A ratio (0-100%)
byte RECIPE_B = 0;       // Solution B ratio (0-100%)
byte RECIPE_C = 0;       // Solution C ratio (0-100%)
byte RECIPE_EC = 0;      // Target EC * 10 (e.g., 15 = 1.5)
byte RECIPE_WATER = 0;   // Target water level (0-100%)


/* =================================================
   [3] SETUP
   ================================================= */
void setup() {

  Serial.begin(115200);
  pinMode(PIN_CR_BUFFER_VALVE, OUTPUT);
  pinMode(PIN_CR_GREENHOUSE_VALVE, OUTPUT);
  pinMode(PIN_CR_MAIN_PUMP, OUTPUT);

  /* ===== BUFFER MIXER ===== */
  pinMode(PIN_BM_VALVE_A, OUTPUT);
  pinMode(PIN_BM_VALVE_B, OUTPUT);
  pinMode(PIN_BM_VALVE_C, OUTPUT);
  pinMode(PIN_BM_MIX_VALVE, OUTPUT);
  pinMode(PIN_BM_MIX_PUMP, OUTPUT);

  /* ===== MIXER MODULE ===== */
  pinMode(PIN_MM_SOL_PUMP, OUTPUT);
  pinMode(PIN_MM_T1_IN, OUTPUT);
  pinMode(PIN_MM_T1_OUT, OUTPUT);
  pinMode(PIN_MM_T2_IN, OUTPUT);
  pinMode(PIN_MM_T2_OUT, OUTPUT);
  pinMode(PIN_MM_T3_IN, OUTPUT);
  pinMode(PIN_MM_T3_OUT, OUTPUT);

  pinMode(VA_TRIG_PIN, OUTPUT);
  pinMode(VA_ECHO_PIN, INPUT);
  pinMode(VB_TRIG_PIN, OUTPUT);
  pinMode(VB_ECHO_PIN, INPUT);
  pinMode(VC_TRIG_PIN, OUTPUT);
  pinMode(VC_ECHO_PIN, INPUT);
  pinMode(MTL_TRIG_PIN, OUTPUT);
  pinMode(MTL_ECHO_PIN, INPUT);
  pinMode(BTL_TRIG_PIN, OUTPUT);
  pinMode(BTL_ECHO_PIN, INPUT);
  pinMode(T1_TRIG_PIN, OUTPUT);
  pinMode(T1_ECHO_PIN, INPUT);
  pinMode(T2_TRIG_PIN, OUTPUT);
  pinMode(T2_ECHO_PIN, INPUT);
  pinMode(T3_TRIG_PIN, OUTPUT);
  pinMode(T3_ECHO_PIN, INPUT);

    /* ===== SUBSTATION ===== */
  pinMode(PIN_SS_PUMP,     OUTPUT);
  pinMode(PIN_SS1_VALVE_1, OUTPUT);
  pinMode(PIN_SS2_VALVE_1, OUTPUT);

allOutputOff();

  // LoRa initialization removed
  ds18b20.begin(); 
  ph_master.begin();
  ph_t1.begin();
  ph_t2.begin();
  ph_t3.begin();
  ec_master.begin();

  pinMode(PIN_EC_PWR, OUTPUT);
  digitalWrite(PIN_EC_PWR, HIGH); // Default to ON

  // --- ARDUINO BOOT SYNC ---
  // Invert all `last_` state trackers. On the first `loop()`, `sendEventStatusPI()` 
  // will see the difference and forcefully broadcast the entire true OFF state back to Python, 
  // instantly curing any DB desynchronization caused by reboot/power loss.
  last_SYSTEM_MODE = !SYSTEM_MODE;
  last_CR_MAIN_PUMP = !state_CR_MAIN_PUMP;
  last_CR_BUFFER_VALVE = !state_CR_BUFFER_VALVE;
  last_CR_GREENHOUSE_VALVE = !state_CR_GREENHOUSE_VALVE;
  last_BM_VALVE_A = !state_BM_VALVE_A;
  last_BM_VALVE_B = !state_BM_VALVE_B;
  last_BM_VALVE_C = !state_BM_VALVE_C;
  last_BM_MIX_VALVE = !state_BM_MIX_VALVE;
  last_BM_MIX_PUMP = !state_BM_MIX_PUMP;
  last_MM_SOL_PUMP = !state_MM_SOL_PUMP;
  last_MM_T1_IN = !state_MM_T1_IN;
  last_MM_T1_OUT = !state_MM_T1_OUT;
  last_MM_T2_IN = !state_MM_T2_IN;
  last_MM_T2_OUT = !state_MM_T2_OUT;
  last_MM_T3_IN = !state_MM_T3_IN;
  last_MM_T3_OUT = !state_MM_T3_OUT;
  last_SS_PUMP = !state_SS_PUMP;
  last_SS1_VALVE_1 = !state_SS1_VALVE_1;
  last_SS2_VALVE_1 = !state_SS2_VALVE_1;
}

/* =================================================
   [3.5] YIELD DELAY - NON-BLOCKING WAIT
   ================================================= */
void yieldDelay(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    moduleSerialPI(); // Keep checking for commands!
  }
}


/* =================================================
   [4] LOOP – MAIN CALL
   ================================================= */
void loop() {

  moduleSerialPI();       // command dari Pi
  
  // Non-blocking sensor state machine
  // Throttling is handled inside the module (SENSOR_INTERVAL)
  moduleSensorMaster();

  if (SYSTEM_MODE) moduleManual();
  else moduleAuto();      // kosong dulu

  moduleStatusCheck(); // Moved here to read ACTUAL pin state AFTER writing
  
  moduleSendBytePI(false);     // ENABLED: Auto-send every 2s for WebSocket stream
  sendEventStatusPI();
  initEventState();
}

// Function updateSharedPump42 removed used unused variables and conflicted with MANUAL_MODULE logic.
void initEventState() 
{
  last_SYSTEM_MODE = SYSTEM_MODE;

  last_CR_BUFFER_VALVE     = state_CR_BUFFER_VALVE;
  last_CR_GREENHOUSE_VALVE = state_CR_GREENHOUSE_VALVE;
  last_CR_MAIN_PUMP        = state_CR_MAIN_PUMP;

  last_BM_VALVE_A   = state_BM_VALVE_A;
  last_BM_VALVE_B   = state_BM_VALVE_B;
  last_BM_VALVE_C   = state_BM_VALVE_C;
  last_BM_MIX_VALVE = state_BM_MIX_VALVE;
  last_BM_MIX_PUMP  = state_BM_MIX_PUMP;
  last_BM_SOL_PUMP  = BM_SOL_PUMP; // Update tracker

  last_MM_SOL_PUMP = state_MM_SOL_PUMP;

  last_MM_T1_IN  = state_MM_T1_IN;
  last_MM_T1_OUT = state_MM_T1_OUT;
  last_MM_T2_IN  = state_MM_T2_IN;
  last_MM_T2_OUT = state_MM_T2_OUT;
  last_MM_T3_IN  = state_MM_T3_IN;
  last_MM_T3_OUT = state_MM_T3_OUT;

  last_SS_PUMP = state_SS_PUMP;
  last_SS1_VALVE_1 = state_SS1_VALVE_1;
  last_SS2_VALVE_1 = state_SS2_VALVE_1;
}


