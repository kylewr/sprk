#include "AsyncStepper.hpp";
#include <Array.h>;

#define SIM

#define STEPPER1_DIR_PIN      2
#define STEPPER1_STEP_PIN     3
#define STEPPER1_DISABLE_PIN  4
#define STEPPER2_DIR_PIN      5
#define STEPPER2_STEP_PIN     6
#define STEPPER2_DISABLE_PIN  7
#define STEPPER3_DIR_PIN      8
#define STEPPER3_STEP_PIN     9
#define STEPPER3_DISABLE_PIN  10

#define STEPS_PER_ROT         200

AsyncStepper stepper1 = AsyncStepper(STEPS_PER_ROT, STEPPER1_DIR_PIN, STEPPER1_STEP_PIN, STEPPER1_DISABLE_PIN);
#ifdef SIM
AsyncStepper stepper2 = AsyncStepper(STEPS_PER_ROT, STEPPER2_DIR_PIN, STEPPER2_STEP_PIN, STEPPER2_DISABLE_PIN);
AsyncStepper stepper3 = AsyncStepper(STEPS_PER_ROT, STEPPER3_DIR_PIN, STEPPER3_STEP_PIN, STEPPER3_DISABLE_PIN);
#endif

AsyncStepper* steppers[] = {
  &stepper1,
#ifdef SIM
  &stepper2,
  &stepper3,
#else
  &stepper1,
  &stepper1
#endif
};

void setup() {
  stepper1.setRPM(120);
#ifdef SIM
  stepper2.setRPM(120);
  stepper3.setRPM(120);
#endif

  stepper1.setAccelerationMode(AsyncStepper::CONSTANT);

  Serial.begin(9600);
  Serial.setTimeout(100);
  Serial.println("Init");
}

String lastCommand = "";

void loop() {
  if (Serial.available() > 0) {
    String original = Serial.readString();
    if (original.startsWith("l")) {
      original = lastCommand;
    }

    if (original.startsWith(",")) { // predef macros
      switch (original.substring(1).toInt()) {
        case 0:
          original = ";0e1.1e1.2e1\n";
          break;
        case 1:
          original = ";0e0.1e0.2e0\n";
          break;
        case 2:
          original = ";0s.1s.2s\n";
          break;
        case 30:
          original = ";0srcw.1srcw.2srcw\n";
          break;
        case 31:
          original = ";0srccw.1srccw.2srccw\n";
          break;
        case 32:
          original = ";0rcw.1rcw.2rcw\n";
          break;
        case 33:
          original = ";0rccw.1rccw.2rccw\n";
          break;
        default:
          break;
      }
    }

    update();

    Array<String, 20> messages;
    uint8_t numTimes = 0;
    if (original.startsWith(";")) { // multiline
      String prefixAdded = " ." + original.substring(1, original.length() - 1);
      char* token = strtok(prefixAdded.c_str(), ".");
      
      uint8_t i = 0; // todo: delete
      while (token != NULL) {
        if (i > 0 && token != "\n" && token != " ") {
          messages.push_back(token);
          numTimes++;
        }
        token = strtok(NULL, ".");
        i++;
      }
      update();
    }
    else {
      messages[0] = original;
      numTimes++;
    }

    uint8_t stepperIndex;
    String currentMessage;
    for (uint8_t i = 0; i < numTimes; i++) {
      if (messages[i] == "") {
        continue;
      }
      stepperIndex = messages[i].substring(0, 1).toInt();
      currentMessage = messages[i].substring(1);

      update();

      if (currentMessage.startsWith("srcw")) { // [#]srcw
        steppers[stepperIndex]->rotateSteps(STEPS_PER_ROT);
      }
      else if (currentMessage.startsWith("srccw")) { // [#]srccw
        steppers[stepperIndex]->rotateSteps(-STEPS_PER_ROT);
      }
      else if (currentMessage.startsWith("rcw")) { // [#]rcw
        steppers[stepperIndex]->rotateCont(AsyncStepper::CW);
      }
      else if (currentMessage.startsWith("rccw")) { // [#]rccw
        steppers[stepperIndex]->rotateCont(AsyncStepper::CCW);
      }
      else if (currentMessage.startsWith("set")) { // [#]set[steps]
        steppers[stepperIndex]->setStep(currentMessage.substring(3).toInt());
      }
      else if (currentMessage.startsWith("ang")) { // [#]ang[int angle]
        steppers[stepperIndex]->setAngle(currentMessage.substring(3).toInt());
      }
      else if (currentMessage.startsWith("acc")) { // [#]acc[accel]
        steppers[stepperIndex]->setAcceleration(currentMessage.substring(3).toInt());
      }
      else if (currentMessage.startsWith("s")) { // [#]s
        steppers[stepperIndex]->stop();
      }
      else if (currentMessage.startsWith("rpm")) { // [#]rpm[int rpm]
        steppers[stepperIndex]->setRPM(currentMessage.substring(3).toInt());
      }
      else if (currentMessage.startsWith("e")) { // [#]e[bool 0/1 (ENABLE/DISABLE)]
        if (currentMessage.substring(1).toInt() == 1) {
          steppers[stepperIndex]->enable();
        }
        else {
          steppers[stepperIndex]->disable();
        }
      }
      else {
        Serial.println("InvalidCommand");
      }
      Serial.println("Done");
    }

    update();

    if (!original.startsWith("l")) {
      lastCommand = original;
    }
  }

  update();
  stepper1.update();
}

void update() {
  // stepper1.update();
  // stepper2.update();
  // stepper3.update();
}