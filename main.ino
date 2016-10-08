#include "notes.h";
#include "menu.h";

// Calibration configuration
const float ACCEPTABLE_PCT = .4;
const char CALIBRATE_SIZE = 10;
const char CALIBRATE_DELAY = 10;
const char NUM_SENSORS = 16; // sensors are assumed contiguous (i.e. mux1[0], mux1[1], etc..)
int expectations[NUM_SENSORS];
int statuses[NUM_SENSORS];

// PINS
// the first 4 numbers are the address pin
// the fifth is the drain pin
const char mux1[] = {5,4,3,2,A2};
const char distanceEnable = 6;
const char pcEnable = 7;

void setup() {
  //  Set MIDI baud rate:
  Serial.begin(31250);

  // initialize the LED pin as an output:
  for(int i = 0 ; i < 5 ; i++) {
    pinMode(mux1[i], OUTPUT);
  }
  pinMode(mux1[4], INPUT);

  pinMode(distanceEnable, OUTPUT);
  pinMode(pcEnable, OUTPUT);

  digitalWrite(pcEnable, HIGH);
  digitalWrite(distanceEnable, HIGH);

  Menu::init();
  Menu::startCalibrating();


  delay(200);
  calibrate();

  Menu::start();
}

void loop(){
  int noteIndex = NUM_STRINGS;
  for(int i = 0 ; i < NUM_SENSORS ; i++) {
    if(i == 0 || i == 1 || i == 6) continue ; // dont use these pins currently
    int status = readSensor(i);
    noteIndex--;
    if(status != statuses[i]) {
      statuses[i] = status;

      // Turn on the current note
      if(status == 1) {
        noteOn(0x90, Notes::getMidiNoteForString(noteIndex), 0x7F);
      } else {
        // turn off the current note
        noteOn(0x90, Notes::getMidiNoteForString(noteIndex), 0x00);
      }
    }
  }

  Menu::getKey();
}

/**
 * set the value at pos to val for mux1.
 **/
void setMux1(int pos, int val) {
  int x, marker;

  pinMode(mux1[4], OUTPUT);
  // 4 bits can only address 16 numbers.
  if(pos >= 16) {
    return;
  }

  // Compute the first value
  x = pos;
  for(int i = 0; i < 4; i++) {
    marker = x % 2;
    x = x / 2;
    if(marker == 0) {
      marker = LOW;
    } else {
      marker = HIGH;
    }

    digitalWrite(mux1[i], marker);
  }
  digitalWrite(mux1[4], val);
}

/**
 * read the value at pos to val for mux1.
 **/
int readMux1(int pos) {
  int x, marker;

  pinMode(mux1[4], INPUT);
  // 4 bits can only address 16 numbers.
  if(pos >= 16) {
    return -1;
  }

  // Compute the first value
  x = pos;
  for(int i = 0; i < 4; i++) {
    marker = x % 2;
    x = x / 2;
    if(marker == 0) {
      marker = LOW;
    } else {
      marker = HIGH;
    }

    digitalWrite(mux1[i], marker);
  }

  return analogRead(mux1[4]);
}

/**
 * calibrate the ambient light expectation
 **/
void calibrate() {
    // loop over the sensors and take 10 readings.
    for(int i = 0 ; i < NUM_SENSORS ; i++) {
      // take 10 reading and average them
      int total = 0;
      for(int r = 0; r < CALIBRATE_SIZE; r++) {
        total += readMux1(i);
        delay(CALIBRATE_DELAY);
      }

      expectations[i] = total / CALIBRATE_SIZE;
      statuses[i] = 0;
    }
}

/**
 * Read a given sensor and compare to its expected value
 * returns 1 if ON (dark) 0 if OFF (light), returns -1
 * if POS is outside the number of sensors.
 **/
int readSensor(int pos) {
  // take a reading
  int reading = readMux1(pos);

  // Compute the acceptable ranges
  // TODO: cache these in an array during calibration
  float windowSize = (expectations[pos] * ACCEPTABLE_PCT);
  float bottomLine = expectations[pos] - windowSize;
  float topLine = expectations[pos] + windowSize;

  // If the reading falls below the acceptable range of calibrated value
  // return that it is on.
  if(reading <= bottomLine) {
    return 1;
  }

  // If the reading falls above the range, recalibrate and return that it is off
  // TODO: only recalibrate this specific sensor since each full calibration
  // requires CALIBRATE_DELAY * CALIBRATE_SIZE * NUM_SENSORS ms to complete.
  // which would be an unacceptable delay while playing (10 * 10 * 2 = 200ms)
  if(reading >= topLine) {
    calibrate();
  }

  // Return that its off.
  return 0;
}

void noteOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}
