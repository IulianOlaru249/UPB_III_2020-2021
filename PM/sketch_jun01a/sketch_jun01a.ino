#include <Servo.h>
#include <Wire.h>

const int start_pos =   5;
int end_pos[4];

/* Servos for each finger */
Servo servos[4];

///* Listen for commands from the glove */
//void receiveEvent(int howMany)
//{
//  /* Receive byte as an integer */
//  int i = Wire.read();
//  Serial.print(i);
//}

void flex_finger(Servo finger_servo, int start_pos, int end_pos) {
  int pos;

  for (pos = start_pos; pos <= end_pos; pos += 1) {
    Serial.println(pos);
    finger_servo.write(pos);
    delay(10);
  }
}

void relax_finger(Servo finger_servo, int start_pos, int end_pos) {
  int pos;

  for (pos = end_pos; pos >= start_pos; pos -= 1) {
    Serial.println(pos);
    finger_servo.write(pos);
    delay(15);
  }
}

void setup() {
  /* Set up end positions of servers */
  end_pos[0] = 167;
  end_pos[1] = 180;
  end_pos[2] = 167;
  end_pos[3] = 180;
  
  /* Attach servos to pins */
  servos[0].attach(9);
  servos[1].attach(6);
  servos[2].attach(5);
  servos[3].attach(3);
  delay(15);

  /* Put servos in starting position */
  for(int i = 0; i < 4; i++) {
    servos[i].write(start_pos);
    delay(25);
  }
  delay(1000);

  /* Join i2c bus with address #4 */
  Wire.begin(4);
  Wire.onReceive(receiveEvent);
}

/* Listen for commands from the glove */
void receiveEvent(int howMany)
{
  /* Receive byte as an integer */
  Serial.println("!!!");
  int i = Wire.read();
  Serial.println(i);
}

void loop() {
  delay(100);
//  for(int i = 0; i < 4; i++) {
//    flex_finger(servos[i], start_pos, end_pos[i]);
//  }
//  delay(5000);
//
//  for(int i = 0; i < 4; i++) {
//    relax_finger(servos[i], start_pos, end_pos[i]);
//  }
//  delay(5000);
//  servos[0].write(0);
//  delay(15);
//  for (pos = 0; pos <= 300; pos += 1) { // goes from 0 degrees to 180 degrees
//    for (int i = 0; i < 4; i++) {
//      /* Move servos according to values */
//      servos[i].write(pos);
//      delay(5);
//    }
//  }
//  for (pos = 300; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//    for (int i = 0; i < 4; i++) {
//      /* Move servos according to values */
//      servos[i].write(pos);
//      delay(5);
//    }
//  }

}
