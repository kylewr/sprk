#include "AsyncStepper.hpp";
#include <Array.h>;

#define SIM

#define TURRET_STEP_PIN 2 
#define TURRET_DIR_PIN 5
#define TURRET_DISABLE_PIN 8
#define ARM_STEP_PIN 3
#define ARM_DIR_PIN 6
#define ARM_DISABLE_PIN 8
#define WRIST_STEP_PIN 4
#define WRIST_DIR_PIN 7
#define WRIST_DISABLE_PIN 8

#define STEPS_PER_ROT 200

AsyncStepper turret = AsyncStepper(STEPS_PER_ROT, TURRET_DIR_PIN, TURRET_STEP_PIN, TURRET_DISABLE_PIN);
AsyncStepper arm = AsyncStepper(STEPS_PER_ROT, ARM_DIR_PIN, ARM_STEP_PIN, ARM_DISABLE_PIN);
AsyncStepper wrist = AsyncStepper(STEPS_PER_ROT, WRIST_DIR_PIN, WRIST_STEP_PIN, WRIST_DISABLE_PIN);

AsyncStepper *steppers[] = {
    &turret,
    &arm,
    &wrist,
};

void setup()
{
    turret.setRPM(280);
    arm.setRPM(160);
    wrist.setRPM(160);

    turret.setAccelerationMode(AsyncStepper::CONSTANT);
    arm.setAccelerationMode(AsyncStepper::CONSTANT);
    wrist.setAccelerationMode(AsyncStepper::CONSTANT);

    Serial.begin(115200);
    Serial.setTimeout(100);
    Serial.println("Init");
}

String lastCommand = "";

bool startedIncoming = false;
bool completeIncoming = false;
String incomingCommand = "";

void loop()
{
    if (Serial.available() > 0)
    {
        startedIncoming = true;
        char c = (char)Serial.read();
        if (c == '!') {
          completeIncoming = true;
          startedIncoming = false;
          return;
        }
        incomingCommand.concat(c);
    }

    if (completeIncoming) {
        completeIncoming = false;
        String original = incomingCommand;
        incomingCommand = "";

        Serial.println(original);

        if (original.startsWith("l"))
        {
            original = lastCommand;
        }

        if (original.startsWith(","))
        { // predef macros
            switch (original.substring(1).toInt())
            {
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
        if (original.startsWith(";"))
        { // multiline
            String prefixAdded = " ." + original.substring(1, original.length() - 1);
            char *token = strtok(prefixAdded.c_str(), ".");

            uint8_t i = 0; // todo: delete
            while (token != NULL)
            {
                if (i > 0 && token != "\n" && token != " ")
                {
                    messages.push_back(token);
                    numTimes++;
                }
                token = strtok(NULL, ".");
                i++;
            }
            update();
        }
        else
        {
            messages[0] = original;
            numTimes++;
        }

        uint8_t stepperIndex;
        String currentMessage;
        for (uint8_t i = 0; i < numTimes; i++)
        {
            if (messages[i] == "")
            {
                continue;
            }
            stepperIndex = messages[i].substring(0, 1).toInt();
            currentMessage = messages[i].substring(1);

            update();

            if (currentMessage.startsWith("srcw"))
            { // [#]srcw
                steppers[stepperIndex]->rotateSteps(10);
            }
            else if (currentMessage.startsWith("srccw"))
            { // [#]srccw
                steppers[stepperIndex]->rotateSteps(-10);
            }
            else if (currentMessage.startsWith("rcw"))
            { // [#]rcw
                steppers[stepperIndex]->rotateCont(AsyncStepper::CW);
            }
            else if (currentMessage.startsWith("rccw"))
            { // [#]rccw
                steppers[stepperIndex]->rotateCont(AsyncStepper::CCW);
            }
            else if (currentMessage.startsWith("set"))
            { // [#]set[steps]
                steppers[stepperIndex]->setStep(currentMessage.substring(3).toInt());
            }
            else if (currentMessage.startsWith("ang"))
            { // [#]ang[int angle]
                steppers[stepperIndex]->setAngle(currentMessage.substring(3).toInt());
            }
            // else if (currentMessage.starsWith("macc")) {
            //     steppers[stepperIndex]->setAccelerationMode(currentMessage.substring(4).toInt());
            // }
            else if (currentMessage.startsWith("acc"))
            { // [#]acc[accel]
                steppers[stepperIndex]->setAcceleration(currentMessage.substring(3).toInt());
            }
            else if (currentMessage.startsWith("s"))
            { // [#]s
                steppers[stepperIndex]->stop();
            }
            else if (currentMessage.startsWith("rpm"))
            { // [#]rpm[int rpm]
                steppers[stepperIndex]->setRPM(currentMessage.substring(3).toInt());
            }
            else if (currentMessage.startsWith("e"))
            { // [#]e[bool 0/1 (ENABLE/DISABLE)]
                if (currentMessage.substring(1).toInt() == 1)
                {
                    steppers[stepperIndex]->enable();
                }
                else
                {
                    steppers[stepperIndex]->disable();
                }
            }
            else
            {
                Serial.println("InvalidCommand");
            }
            Serial.println("Done");
        }

        update();

        // if (!original.startsWith("l"))
        // {
        //     lastCommand = original;
        // }
    }

    update();
}

void update()
{
    turret.update();
    arm.update();
    wrist.update();
}