#include <Wire.h>

const int BUTTONS_NO = 4;

int  buttonPins[BUTTONS_NO];
int buttonStates[BUTTONS_NO];
int lastButtonStates[BUTTONS_NO];

void setup()
{

    /* Initialize button pins */
    buttonPins[0] = 13;
    buttonPins[1] = 12;
    buttonPins[2] = 3;
    buttonPins[3] = 2;
  
    /* initialize button pins */
    for(int i = 0; i < BUTTONS_NO; i++) {
      buttonStates[i] = 0;
      lastButtonStates[i] = 0;
    }

    /* initialize the button pin as a input: */
    for(int i = 0; i < BUTTONS_NO; i++) {
      pinMode(buttonPins[i], INPUT);
    }
  
    /* For debug */
    Serial.begin(9600);
  
    /* join i2c bus (address optional for master) */
    Wire.begin();
}

void loop()
{
    /**
     * Code to identify the pressed button
     * code = ab (10 * a + b) where a = button id + 1; b = on(1)/off(0) state
     */
    int msg_code = 0;
   
    /* read the pushbutton input pin */
    for (int i = 0; i < BUTTONS_NO; i++) {
        buttonStates[i] = digitalRead(buttonPins[i]);
    }

    /* Detect changes in state */
    for (int i = 0; i < BUTTONS_NO; i++) {
       msg_code = 0;
      /* compare the buttonState to its previous state */
      if (buttonStates[i] != lastButtonStates[i]) {
        /* if the state has changed, increment the counter */
        if (buttonStates[i] == HIGH) {
            /* if the current state is HIGH then the button went from off to on */;
            msg_code = (i + 1) * 10 + 1;
            /* transmit to device #4 */
            Wire.beginTransmission(4);
            Wire.write(msg_code);
            delay(500);
            /* stop transmitting */
            Wire.endTransmission();

        } else {
            /* if the current state is LOW then the button went from on to off */
            /* transmit to device #4 */
            msg_code = (i + 1) * 10 + 0;
            Wire.beginTransmission(4);
            Wire.write(msg_code);
            delay(500);
            /* stop transmitting */
            Wire.endTransmission();

        }
        
        // Delay a little bit to avoid bouncing
        delay(50);
      }
      // save the current state as the last state, for next time through the loop
      lastButtonStates[i] = buttonStates[i];
    }
}
