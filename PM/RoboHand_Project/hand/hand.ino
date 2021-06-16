#include <Wire.h>
#include <Servo.h>

const int start_pos =   5;
int end_pos[4];

/* Servos for each finger */
Servo servos[4];

void flex_finger(Servo finger_servo, int start_pos, int end_pos) {
  int pos;

  for (pos = start_pos; pos <= end_pos; pos += 1) {
//    Serial.println(pos);
    finger_servo.write(pos);
    delay(10);
  }
}

void relax_finger(Servo finger_servo, int start_pos, int end_pos) {
  int pos;

  for (pos = end_pos; pos >= start_pos; pos -= 1) {
//    Serial.println(pos);
    finger_servo.write(pos);
    delay(10);
  }
}

/**
 * Function that executes whenever data is received from master
 * this function is registered as an event, see setup()
 */
void receiveEvent(int howMany)
{
      /**
       * Extract button id and state from glove message
       * code = ab (10 * a + b) where a = button id + 1; b = on(1)/off(0) state
       */
      int index = -1;
      int state = -1;
      int i = Wire.read();

      state = i % 10;
      index = i / 10 - 1;
      
      /* Print info for debug */
      Serial.print(index);
      Serial.print(" : ");
      Serial.println(state);

      /* Flex / Relax coresponding finger */
      if (state == 1)
        flex_finger(servos[index], start_pos, end_pos[index]);
      else
        relax_finger(servos[index], start_pos, end_pos[index]);
}

void setup()
{
  /* Set up end positions of servers */
  end_pos[0] = 167;
  end_pos[1] = 180;
  end_pos[2] = 167;
  end_pos[3] = 180;
  
  /* Attach servos to pins */
  servos[0].attach(5);
  servos[1].attach(6);
  servos[2].attach(3);
  servos[3].attach(9);
  delay(15);

  /* Put servos in starting position */
  for(int i = 0; i < 4; i++) {
    servos[i].write(start_pos);
    delay(25);
  }
  delay(1000);

  /* Join i2c bus with address #4 */
  Wire.begin(4);               
  
  /* Register event */
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}
 
void loop()
{
  delay(100);
}
