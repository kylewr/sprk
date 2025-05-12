/***************************************************
Copyright (c) 2021 Luis Llamas
(www.luisllamas.es)

Modified (c) 2025 Kyle Rush
(quackings.com)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#ifndef _ASYNCSTEPPER_h
#define _ASYNCSTEPPER_h

#define _ASYNCSTEPPER_VERSION_ 3.0

#include <Arduino.h>

typedef void (*StepperCallback)();

class AsyncStepper
{
public:
    enum AccelMode
    {
        CONSTANT,
        LINEAR
    };

    enum StepperDirection
    {
        CW,
        CCW,
    };

    enum StepperState
    {
        STOPPED,
        ACCELERATING,
        RUNNING,
        DECELERATING
    };

    uint16_t m_motorSteps;
    uint8_t m_dirPin;
    uint8_t m_stepPin;
    uint8_t m_enablePin;

    StepperCallback onFinishCallback = nullptr;

    AccelMode m_accelMode = AccelMode::LINEAR;
    StepperState m_currentState = StepperState::STOPPED;
    StepperDirection m_currentDirection = StepperDirection::CW;

    /**
     * Creates a new AsnycStepper
     *
     * @param motorSteps Number of steps in one rotation
     * @param directionPin Pin mapping to the directin pin on the A4988
     * @param stepPin Pin mapping to the step pin on the A4988
     */
    AsyncStepper(uint16_t motorSteps, uint8_t directionPin, uint8_t stepPin, uint8_t enablePin)
    {
        m_motorSteps = motorSteps;
        m_dirPin = directionPin;
        m_stepPin = stepPin;
        m_enablePin = enablePin;

        onFinishCallback = nullptr;

        pinMode(m_dirPin, OUTPUT);
        pinMode(m_stepPin, OUTPUT);
        pinMode(m_enablePin, OUTPUT);

        disable();
    }

    /**
     * Enable the stepper
    */
    void enable() {
        stop();
        setEnableState(true);
    }

    /**
     * Disable the stepper and cancel any command
    */
    void disable() {
        stop();
        setEnableState(false);
    }

    /**
     * Finds the direction from a given steps
     * 
     * @param steps input steps
     * @return the direction the motor should take
     */
    StepperDirection getDirection(long steps)
    {
        return steps > 0 ? StepperDirection::CW : StepperDirection::CCW;
    }

    /**
     * Rotate Continuously
     *
     * @param direction Direction to rotate with
     */
    void rotateCont(StepperDirection direction)
    {
        onFinishCallback = nullptr;
        m_currentDirection = direction;

        m_currentlyContinuous = true;
        m_queuedSteps = 0;

        initRotation(m_motorSpeed);
    }

    /**
     * Stops the motor
     */
    void stop()
    {
        m_currentState = StepperState::STOPPED;
    }

    /**
     * Sets deceleration to the motor
     */
    void brake()
    {
        if (m_accelMode == AccelMode::CONSTANT)
        {
            stop();
            return;
        }

        decelSTeps = GetRampSteps(getCurrentSpeed(), m_deceleration);
        m_queuedSteps = decelSTeps;
        m_currentlyContinuous = false;
        m_travelCurrentStep = 1;
        m_currentState = StepperState::DECELERATING;
    }

    /**
     * Rotate a set amount of steps
     *
     * @param steps Number of steps
     */
    void rotateSteps(long steps, StepperCallback onFinish = nullptr)
    {
        onFinishCallback = onFinish;
        m_currentDirection = getDirection(steps);

        m_currentlyContinuous = false;
        m_queuedSteps = labs(steps); // long absolute value of steps

        initRotation(m_motorSpeed);
    }

    /**
     * Set the motor position to step
     *
     * @param steps Step to set
     */
    void setStep(long step, StepperCallback onFinish = nullptr)
    {
        long current = getAbsoluteStep();
        if (current == step)
            return;
        rotateSteps(step - current, onFinish);
    }

    /**
     * Rotate a given angle from the current position
     *
     * @param angle Angle to rotate based on the current position
     * @param onFinish callback
     */
    void rotateAngle(float angle, StepperCallback onFinish = nullptr)
    {
        rotateSteps(angle / 360.0f * 200 * (angle > 0 ? 1.0f : -1.0f), onFinish);
    }

    /**
     * Rotate to an angle from the current position
     *
     * @param angle Angle to rotate to based on the zero position
     * @param onFinish callback
     */
    void setAngle(float angle, StepperCallback onFinish = nullptr)
    {
        setStep(angle * (m_motorSteps / 360.0f), onFinish);
    }

    // void rotateWithTime(long steps, float time, StepperDirection direction, StepperCallback onFinish = nullptr)
    // {
    //     float requiredTime = GetTimeForMove(steps);

    //     float speed = requiredTime > time ? m_motorSpeed : GetSpeedForMove(steps, time);

    //     m_currentDirection = direction;
    //     onFinishCallback = onFinish;

    //     m_currentlyContinuous = false;
    //     m_queuedSteps = labs(steps);

    //     initRotation(speed);
    // }

    // void rotateAngleWithTime(float angle, float time, StepperDirection direction, StepperCallback onFinish = nullptr)
    // {
    //     rotateWithTime(angle / 360.0f * 200, time, direction, onFinish);
    // }

    // void setAngleWithTime(float angle, float time, StepperDirection direction, StepperCallback onFinish = nullptr)
    // {
    //     // wrong code (look above)
    //     float currentAngle = getCurrentAngle();
    //     float angleDelta = direction == CW ? currentAngle - angle : angle - currentAngle;
    //     if (angleDelta < 0)
    //         angleDelta = -angleDelta;
    //     rotateAngleWithTime(angleDelta, time, direction, onFinishCallback);
    // }

    /**
     * Main update function to move the stepper
     * Should be called in the Arduino loop() function
     */
    uint16_t update()
    {
        if (m_currentState == StepperState::STOPPED)
            return false;

        uint16_t stepsDone = 0;
        while (static_cast<unsigned long>(micros() - m_lastStepTime) >= m_interval)
        {
            m_lastStepTime += m_interval;
            step();

            if (stepsDone > 0)
                delayMicroseconds(PULSE_OFF_WIDTH);
            stepsDone++;
        }
        return stepsDone;
    }

    void setSpeed(long speed)
    {
        m_motorSpeed = speed;
    }

    void setRPM(float rpm)
    {
        setSpeed(rpm / 60.0f * m_motorSteps);
    }

    void setDPS(float degreesPerSecond)
    {
        setSpeed(degreesPerSecond / 360.0f * m_motorSteps);
    }

    void setAccelerationMode(AccelMode newMode)
    {
        m_accelMode = newMode;
    }

    void setAcceleration(long acceleration)
    {
        m_acceleration = acceleration;
        m_deceleration = acceleration;
    }

    void setAcceleration(long acceleration, long deceleration)
    {
        m_acceleration = acceleration;
        m_deceleration = deceleration;
    }

    long getAbsoluteStep() const
    {
        return m_absoluteStep;
    }

    float getCurrentAngle() const
    {
        return 360.0f * (m_absoluteStep % m_motorSteps) / m_motorSteps;
    }

    // unsigned long getTravelingTime() const
    // {
    //     return micros() - m_travelStartTime;
    // }

    // long getInterval() const
    // {
    //     return m_interval;
    // }

    // long getTravelCurrentStep() const
    // {
    //     return m_travelCurrentStep;
    // }

    // long getQueuedSteps() const
    // {
    //     return m_queuedSteps;
    // }

    long getRemainingSteps() const
    {
        return m_queuedSteps - m_travelCurrentStep;
    }

    long getMaxSpeed() const
    {
        return m_motorSpeed;
    }

    long getCurrentSpeed() const
    {
        if (m_currentState == StepperState::STOPPED)
            return 0;
        return 1e+6 / m_interval;
    }

    float getCurrentRPM() const
    {
        return getCurrentSpeed() / m_motorSteps * 60.0f;
    }

    float getCurrentDPS() const
    {
        return getCurrentSpeed() / m_motorSteps * 360.0f;
    }

    float GetSpeedForMove(long steps, float time)
    {
        float speed = 0;

        if (m_accelMode == AsyncStepper::LINEAR)
        {
            float a_2 = 1.0 / m_acceleration + 1.0 / m_deceleration;
            float discriminant = time * time - 2 * a_2 * steps;
            if (discriminant >= 0)
            {
                speed = (time - (float)sqrt(discriminant)) / a_2;
            };
        }
        else
        {
            speed = steps / time;
        }

        return speed;
    }

    float GetTimeForMove(long steps)
    {
        return GetTimeForMove(steps, m_motorSpeed);
    }

    float GetTimeForMove(long steps, unsigned long speed)
    {
        float time;
        if (steps == 0)
        {
            return 0;
        }
        else
        {
            if (m_accelMode == AsyncStepper::LINEAR)
            {
                auto accSteps = (float)GetRampSteps(m_motorSpeed, m_acceleration);
                auto decSteps = (float)GetRampSteps(m_motorSpeed, m_deceleration);

                float ramps = accSteps + decSteps;
                float runSteps = 0;

                if (steps < ramps)
                {
                    accSteps = steps * m_deceleration / (m_acceleration + m_deceleration);
                    decSteps = steps - accSteps;
                }
                else
                {
                    runSteps = steps - ramps;
                }
                time = (runSteps / speed) + sqrt(2.0 * accSteps / m_acceleration) + sqrt(2.0 * decSteps / m_deceleration);
            }
            else
            {
                time = steps / speed;
            }
        }
        return time;
    }

private:
    // unsigned long Speed = 200;
    unsigned long m_acceleration = 100;
    unsigned long m_deceleration = 100;
    unsigned long m_motorSpeed = 200;

    bool m_currentlyContinuous = false;

    long m_absoluteStep = 0;

    unsigned long m_queuedSteps;
    unsigned long accelSteps;
    unsigned long decelSTeps;
    unsigned long m_travelCurrentStep;

    long m_interval = 0;
    long m_restInterval = 0;
    long m_runInterval = 0;

    unsigned long m_travelStartTime;
    unsigned long m_lastStepTime;

    const unsigned long PULSE_ON_WIDTH = 10;
    const unsigned long PULSE_OFF_WIDTH = 250;

    void initRotation(unsigned long speed)
    {
        m_currentState = m_accelMode == AccelMode::CONSTANT ? StepperState::RUNNING : StepperState::ACCELERATING;

        accelSteps = GetRampSteps(speed, m_acceleration);
        m_interval = 1e+6f * 0.956f * sqrt(1.0f / m_acceleration);
        m_runInterval = 1e+6f / speed;
        m_restInterval = 0;

        m_travelCurrentStep = 0;
        m_travelStartTime = micros();
        m_lastStepTime = m_travelStartTime;
    }

    void step()
    {
        if (m_currentDirection == StepperDirection::CW)
            stepCW();
        else
            stepCCW();

        m_travelCurrentStep++;

        updateState();
        updateInterval();
    }

    void updateState()
    {
        if (m_currentState == StepperState::STOPPED)
            return;

        const long remaining = getRemainingSteps();
        if (m_currentlyContinuous == false && remaining <= 0)
        {
            m_currentState = StepperState::STOPPED;
            if (onFinishCallback != nullptr)
                onFinishCallback();
        }
        else
        {
            if (m_accelMode == AccelMode::LINEAR)
            {
                if (m_currentState == StepperState::DECELERATING)
                    return;

                decelSTeps = GetRampSteps(getCurrentSpeed(), m_deceleration);
                if (m_currentlyContinuous == false && remaining <= decelSTeps)
                {
                    m_currentState = StepperState::DECELERATING;
                }
                else if (m_travelCurrentStep <= accelSteps)
                {
                    m_currentState = StepperState::ACCELERATING;
                }
                else
                {
                    m_currentState = StepperState::RUNNING;
                }
            }
        }
    }

    void updateInterval()
    {
        if (m_currentState == StepperState::ACCELERATING && m_travelCurrentStep > 0)
        {
            m_interval = m_interval - (2 * m_interval + m_restInterval) / (4 * m_travelCurrentStep + 1);
            m_restInterval = (m_travelCurrentStep < accelSteps) ? (2 * m_interval + m_restInterval) % (4 * m_travelCurrentStep + 1) : 0;
        }
        else if (m_currentState == StepperState::DECELERATING && m_travelCurrentStep > 0)
        {
            const long remain = getRemainingSteps();
            m_interval = m_interval - (2 * m_interval + m_restInterval) / (-4 * remain + 1);
            m_restInterval = (2 * m_interval + m_restInterval) % (-4 * remain + 1);
        }
        else if (m_currentState == StepperState::RUNNING)
        {
            m_interval = m_runInterval;
        }

        if (m_interval < m_runInterval)
        {
            m_interval = m_runInterval;
        }
    }

    static unsigned long GetRampSteps(unsigned long speed, unsigned long acceleration)
    {
        return (speed * speed) / (2.0f * acceleration);
    }

    void setEnableState(bool enabled) {
        digitalWrite(m_enablePin, enabled ? LOW : HIGH);
    }

    void stepCW()
    {
        digitalWrite(m_dirPin, LOW);
        digitalPulse(m_stepPin);

        m_absoluteStep++;
    }

    void stepCCW()
    {
        digitalWrite(m_dirPin, HIGH);
        digitalPulse(m_stepPin);

        m_absoluteStep--;
    }

    void digitalPulse(uint8_t pin) const
    {
        digitalWrite(pin, HIGH);
        delayMicroseconds(PULSE_ON_WIDTH);
        digitalWrite(pin, LOW);
    }
};
#endif
