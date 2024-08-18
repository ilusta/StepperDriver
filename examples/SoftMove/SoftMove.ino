#include "StepperDriver.h"

#define dirPin      12
#define stepPin     11
#define enablePin   10
#define stepPerRotation   1600


float sign(float x) {
  if (x > 0.0) return 1.0;
  if (x < 0.0) return -1.0;
  return 0.0;
}


void softMove(float y0, float y1, float E, float Wmax) {

  float t0 = millis() / 1000.0;
  float t2 = Wmax / E + t0;
  float t3 = abs(y1 - y0) / Wmax - Wmax / E + t2;
  float t1 = t3 + Wmax / E;
  float t4 = sqrt(abs(y1 - y0) / E) + t0;
  float t5 = t4 + sqrt(abs(y1 - y0) / E);

  float y = y0;

  float s = sign(y1 - y0);

  float t = millis() / 1000.0;

  if (t3 > t2)
  {
    while (t <= t1) {
      t = millis() / 1000.0;

      if (t <= t2)
        y = y0 + s * (E * (t - t0) * (t - t0) / 2);
      else if (t <= t3)
        y = y0 + s * (E * (t2 - t0) * (t2 - t0) / 2 + Wmax * (t - t2));
      else
        y = y0 + s * (E * (t2 - t0) * (t2 - t0) / 2 + Wmax * (t3 - t2) + Wmax * (t - t3) - E * (t - t3) * (t - t3) / 2);
  
      stepper.setAngleDegrees(y);
      //stepper.monitor();
    }
  }
  else {
    while (t <= t5) {
      t = millis() / 1000.0;

      if (t <= t4)
        y = y0 + s * (E * (t - t0) * (t - t0) / 2);
      else
        y = y0 + s * (E * (t4 - t0) * (t4 - t0) / 2 + sqrt(abs(y1 - y0) / E) * E * (t - t4) - E * (t - t4) * (t - t4) / 2);

      stepper.setAngleDegrees(y);
      //stepper.monitor();
    }
  }
}


void setup() {
  Serial.begin(115200);

  stepper.init(dirPin, stepPin, enablePin, stepPerRotation);

  stepper.enable();
  stepper.setAngleDegrees(0);
  delay(500);
}


void loop() {
  softMove(0, 720, 2000, 1000);
  delay(1000);
  softMove(720, 0, 2000, 1000);
  delay(1000);
}
