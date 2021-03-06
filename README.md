# Button Array
Microswitch array for use in Arduino using multiplexing

## Hardware
If you want to build a button array, follow this diagram.
![Button Array Diagram](diagram/ButtonArray.png)

The main idea here is to connect each button to a diode in a row by column fashion, and read all the rows column by column. Rows works as inputs by setting each column as an output, so only the diodes in the enabled column get open and thus only a single row of buttons can be read at a time without interference from the other columns.

This is commonly known as multiplexing.

## Software

You'll need to add code in three different places:

### Header

Declare pin definitions and variables.

```c++
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
```

The REMAPPED_BUTTONS array works as a shorthand to remap (and reorder) buttons in your array. If you're building an array to use a musical keyboard, you'll likely need to make sure that all your buttons are in order (either to use as MIDI notes or directly in a synth). It needs some trial and error to setup, but it's usually faster than to redo cables and connectors.


### setup()

Set rows as inputs and columns as output.

```c++
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
```

Serial is used here for the initial configuration of the REMAPPED_BUTTONS array and for general diagnose and debug.


### loop()

Read buttons, column by column, row by row. For each column, set the column HIGH and read all rows. Using the `prevButtonState` and `buttonState` arrays you can detect button down and button up events. If you only use the `buttonState` array, you'll get instant values each loop.

```c++

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
```


# Original concept

Inspired by Code Tinker Hack.

http://www.codetinkerhack.com/2012/11/how-to-turn-piano-toy-into-midi.html
