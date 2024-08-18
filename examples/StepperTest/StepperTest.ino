#include "StepperDriver.h"

#define dirPin      12
#define stepPin     11
#define enablePin   10
#define stepPerRotation   1600


void setup() {
  stepper.init(dirPin, stepPin, enablePin, stepPerRotation);

  stepper.enable();
  stepper.setAngleDegrees(0);
  delay(500);
}


void loop() {
  stepper.setAngleDegrees(90);
  delay(1000);

  stepper.setAngleDegrees(-90);
  delay(1000);

  stepper.setAngleDegrees(720);
  delay(2000);

  for (double i = 720.0; i > 360.0; i -= 0.1) {
    stepper.setAngleDegrees(i);
    delay(1);
  }

  delay(1000);
}
