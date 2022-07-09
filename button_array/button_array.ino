// inspired by
// http://www.codetinkerhack.com/2012/11/how-to-turn-piano-toy-into-midi.html

// number of rows and cols
const int NUM_ROWS = 5;
const int NUM_COLS = 8;

// pin definition
const int ROWS_PIN[NUM_ROWS] = {A0, A1, A2, A3, A4};
const int COLS_PIN[NUM_COLS] = {0, 1, 2, 3, 4, 5, 6, 7};

// button state arrays
const int NUM_BUTTONS = NUM_COLS * NUM_ROWS;
int buttonState[NUM_BUTTONS];
int prevButtonState[NUM_BUTTONS];

// remapping of buttons, (for use i.e. a musical keyboard)
// ideally for reordering the buttons (3, 2, 1, 6, 5, 4, etc)
const int REMAPPED_BUTTONS[NUM_BUTTONS] = {
  0,   1,  2,  3,  4,  5,  6,  7,  8,  9,
  10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
  30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
};


void setup() {
  // set columns as output and rows as input
  for (int col = 0; col < NUM_COLS; col++) {
    pinMode(COLS_PIN[col], OUTPUT);
  }
  for (int row = 0; row < NUM_ROWS; row++) {
    pinMode(ROWS_PIN[row], INPUT);
  }

  Serial.begin(115200);
}

void loop() {
  int button;
  for (int col = 0; col < NUM_COLS; col++) {
    // put column high, read rows, put column low,
    // and compute "key down" and "key up" events
    digitalWrite(COLS_PIN[col], HIGH);
    for (int row = 0; row < NUM_ROWS; row++) {
      button = NUM_ROWS * col + row;
      prevButtonState[button] = buttonState[button];
      buttonState[button] = digitalRead(ROWS_PIN[row]);

      // detect button change
      if (buttonState[button] != prevButtonState[button]) {
        Serial.print(REMAPPED_BUTTONS[button]);
        Serial.print(" ");
        if (buttonState[button] == HIGH) {
          // key down
          Serial.print("on");
        } else {
          // key up
          Serial.print("off");
        }
        Serial.print("\n");
      }
    }
    digitalWrite(COLS_PIN[col], LOW);
  }
}
