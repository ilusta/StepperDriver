class StepperDriver {
  public:
    StepperDriver(){}
    
    void init(int dirPin, int stepPin, int enablePin, uint32_t stepsPerRotation) {
      this->dirPin = dirPin;
      this->stepPin = stepPin;
      this->enablePin = enablePin;
      this->stepsPerRotation = 2 * stepsPerRotation;

      goalPos = 0;
      currentPos = 0;
      stepState = 0;
      
      pinMode(dirPin, OUTPUT);
      pinMode(stepPin, OUTPUT);
      pinMode(enablePin, OUTPUT);

      digitalWrite(enablePin, HIGH);
      digitalWrite(dirPin, LOW);
      digitalWrite(stepPin, stepState);

      cli();
      TCCR2A = 0;
      TCCR2B = 0;
      TIMSK2 = (1 << TOIE1);
      TCCR2B |= (1 << CS11);
      sei();
    }

    void enable(bool enable = true) {
      digitalWrite(enablePin, !enable);
    }

    void disable() {
      digitalWrite(enablePin, HIGH);
    }

    void setAngleDegrees(double angle) {
      goalPos = angle * stepsPerRotation / 360.0;
      updateDirection();
    }

    void setAngleRadians(double angle) {
      goalPos = angle * stepsPerRotation / 6.2832;
      updateDirection();
    }

    void update() {
      if (goalPos != currentPos)
      {
        //Make steps
        stepState = !stepState;
        digitalWrite(stepPin, stepState);
    
        //Update current position
        currentPos += sign(goalPos - currentPos);
      }
    }

    void monitor(){
      Serial.println(String(uint32_t(goalPos)) + " " + String(uint32_t(currentPos)) + " " + String(stepState));
    }

  private:
    int dirPin, stepPin, enablePin;
    uint32_t stepsPerRotation;
    int64_t currentPos;
    int64_t goalPos;
    bool stepState;

    void updateDirection(){
      if(goalPos > currentPos) digitalWrite(dirPin, LOW);
      else if(goalPos < currentPos) digitalWrite(dirPin, HIGH);
    }
    
    int8_t sign(float x) {
      if (x > 0.0) return 1.0;
      if (x < 0.0) return -1.0;
      return 0.0;
    }

};

StepperDriver stepper = StepperDriver();

ISR(TIMER2_OVF_vect)
{
  stepper.update();
}
