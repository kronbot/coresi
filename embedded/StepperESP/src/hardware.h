#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#include <AccelStepper.h>

namespace Hardware
{
    namespace Config
    {
        const int DefaultStepperAcceleration = 2000;
        const int DefaultStepperSpeed = 2000;
        const int DefaultStepperMaxSpeed = 4000;

        const int StepsPerRevolution = 800;
        const double WheelDiameter = 280; // mm
    }

    class Stepper
    {
        AccelStepper *stepper;

        int pulsePin, directionPin;
        int acceleration = Config::DefaultStepperAcceleration;
        int speed = Config::DefaultStepperSpeed;
        int maxSpeed = Config::DefaultStepperMaxSpeed;

        int stepsPerRevolution = Config::StepsPerRevolution;
        double wheelDiameter = Config::WheelDiameter;

        int position;

        bool telemetry;

    public:
        Stepper(int pulsePin, int directionPin)
        {
            this->pulsePin = pulsePin;
            this->directionPin = directionPin;

            stepper = new AccelStepper(AccelStepper::DRIVER, pulsePin, directionPin);

            stepper->setMaxSpeed(maxSpeed);
            stepper->setAcceleration(acceleration);
            stepper->setSpeed(speed);
            stepper->setCurrentPosition(0);
        }

        Stepper(int pulsePin, int directionPin, bool telemetry)
        {
            this->pulsePin = pulsePin;
            this->directionPin = directionPin;

            stepper = new AccelStepper(AccelStepper::DRIVER, pulsePin, directionPin);

            stepper->setMaxSpeed(maxSpeed);
            stepper->setAcceleration(acceleration);
            stepper->setSpeed(speed);
            stepper->setCurrentPosition(0);

            this->telemetry = telemetry;
        }

        void forceStop()
        {
            stepper->stop();

            if (telemetry)
                Serial.println("Stepper force stopped");
        }

        int distance()
        {
            if (telemetry)
            {
                Serial.print("Stepper distance to go: ");
                Serial.println(stepper->distanceToGo());
            }
            return stepper->distanceToGo();
        }

        void moveTo(int position)
        {
            if (telemetry)
            {
                Serial.print("Stepper new position: ");
                Serial.println(position);
            }
            stepper->moveTo(position);
            this->position = position;
        }

        void run()
        {
            stepper->runToPosition();
        }

        int getPosition()
        {
            return position;
        }

        void reset()
        {
            stepper->stop();
            stepper->setCurrentPosition(0);
            position = 0;
        }
    };

    class LimitSwitch
    {
        int pin;
        bool telemetry;

    public:
        LimitSwitch(int pin)
        {
            this->pin = pin;
            pinMode(pin, INPUT);
        }

        LimitSwitch(int pin, bool telemetry)
        {
            this->pin = pin;
            pinMode(pin, INPUT);

            this->telemetry = telemetry;
        }

        bool isPressed()
        {
            bool pressed = digitalRead(pin) == LOW;

            if (telemetry)
            {
                Serial.print("Limit switch ");
                Serial.print(pin);
                Serial.print(" is pressed: ");
                Serial.println(pressed);
            }

            return pressed;
        }
    };
}

#endif