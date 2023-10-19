#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#include <AccelStepper.h>
#include "pidController.h"

namespace Hardware
{
    namespace Config
    {
        const int DefaultStepperAcceleration = 500;
        const int DefaultStepperSpeed = 500;
        const int DefaultStepperMaxSpeed = 1000;

        const int StepsPerRevolution = 200;
        const double WheelDiameter = 0.5;

        namespace PID
        {
            const double Kp = 0.1;
            const double Ki = 0.1;
            const double Kd = 0.1;
        }
    }

    class Stepper
    {
        AccelStepper *stepper;
        PID::PIDController *pidController;

        int pulsePin, directionPin;
        int acceleration = Config::DefaultStepperAcceleration;
        int speed = Config::DefaultStepperSpeed;
        int maxSpeed = Config::DefaultStepperMaxSpeed;

        int stepsPerRevolution = Config::StepsPerRevolution;
        double wheelDiameter = Config::WheelDiameter;

        int position = 0;

        bool telemetry = false;

    public:
        Stepper(int pulsePin, int directionPin)
        {
            this->pulsePin = pulsePin;
            this->directionPin = directionPin;

            pidController = new PID::PIDController(Config::PID::Kp, Config::PID::Ki, Config::PID::Kd);
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

            pidController = new PID::PIDController(Config::PID::Kp, Config::PID::Ki, Config::PID::Kd);
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

        void run()
        {
            speed = pidController->compute(distance());

            if (telemetry)
            {
                Serial.print("Stepper PID speed: ");
                Serial.println(speed);
            }

            // stepper->setSpeed(speed);

            stepper->run();
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

        void setPosition(int position)
        {
            if (telemetry)
            {
                Serial.print("Stepper new position: ");
                Serial.println(position);
            }
            stepper->moveTo(position);
            stepper->runToPosition();
        }
    };
}

#endif