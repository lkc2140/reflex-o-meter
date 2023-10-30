#undef LED_BUILTIN
#define LED_BUILTIN   (32)
#define SWITCH_PIN    2   // Define the switch (button) pin

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);    // Initialize the LED pin as an output
  pinMode(SWITCH_PIN, INPUT_PULLUP);  // Initialize the switch pin as an input with a pull-up resistor
}

int longButtonState = HIGH;
int lastButtonState = HIGH;
int lastDebounceTime = 0;
const int DEBOUNCE_DELAY = 50;
bool LED_STATE = LOW;

void loop() {
  int reading = digitalRead(SWITCH_PIN);
  if (reading != lastButtonState)
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) >  DEBOUNCE_DELAY)
  {
    if(reading != longButtonState)
    {
      longButtonState = reading;
      if(longButtonState == LOW)
      {
        LED_STATE = !LED_STATE;
        digitalWrite(LED_BUILTIN, LED_STATE);

      }
    }
  }
  lastButtonState = reading;
}

// 1 normal state - no button pressed
// reading = high
// lastButtonState = high
// millis() - lastDebounceTime > 50
//   do nothing

// 2 as soon as button pressed 
// reading = low
// lastButtonState = high
// lastDebounceTime = 1000
// millis(1001) - lastDebounceTime < 50
// do nothing
// lastButtonState = low

// 3 button pressed 51 ms
// reading = low
// lastButtonState = high
// lastDebounceTime = 1000
// millis(1100) - lastDebounceTime(1000) > 50
// toggle led state
// lastButtonState = low

