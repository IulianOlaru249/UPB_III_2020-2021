#include <Wire.h>

const int FLEX_PINS_NO = 4;

/* Store values of the flex sensors */
int flexPinValues[FLEX_PINS_NO];
/* Store pins of the flex sensors*/
int flexPins[FLEX_PINS_NO];

void setup()
{
    /* Map flex pins to physical pins */
    flexPins[0] = A0;
    flexPins[1] = A1;
    flexPins[2] = A2;
    flexPins[3] = A3;
  
    /* Initiate flex values to 0 */
    for (int i = 0; i < FLEX_PINS_NO; i++)
        flexPinValues[i] = 120;

    /* For debug */
    Serial.begin(9600);
  
    /* join i2c bus (address optional for master) */
    Wire.begin();
}

void loop()
{
  /* transmit to device #4 */
  Wire.beginTransmission(4);
  
  for (int i = 0; i < FLEX_PINS_NO; i++) {
    /* Read and save analog value from potentiometer */
    //flexPinValues[i] = analogRead(flexPins[i]);
    /* Map value 0-1023 to 0-255 (PWM) */
//    flexPinValues[i] = map(flexPinValues[i], 700, 900, 0, 255);
    
    /* Print info for debug */
    Serial.print(i);
    Serial.print(": ");
    Serial.println(flexPinValues[i]);
    
    /* Send values to slave via I2C */
    Wire.write(flexPinValues[i]);

  }
 
  Serial.println();
  /* stop transmitting */
  Wire.endTransmission();


  delay(500);
}
