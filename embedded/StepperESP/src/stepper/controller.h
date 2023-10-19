#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "driver.h"
#include "hardware.h"

namespace Control
{
    namespace Config
    {
        namespace Pins
        {
            const int StepperActuator1PulusePin = 26;
            const int StepperActuator1DirectionPin = 27;

            const int StepperActuator2PulusePin = 32;
            const int StepperActuator2DirectionPin = 33;

            const int StepperBox1PulusePin = 34;
            const int StepperBox1DirectionPin = 35;

            const int StepperBox2PulusePin = 36;
            const int StepperBox2DirectionPin = 39;

            const int StepperDoorPulusePin = 4;
            const int StepperDoorDirectionPin = 5;

            const int StepperEnablePin = 25;
        }

        namespace Coordinates
        {
            const int DoorOpenPositionSteps = 1000;

            const int FirstBoxPositionSteps = 1000;
            const int SecondBoxPositionSteps = 2000;
            const int ThirdBoxPositionSteps = 3000;
            const int RestPositionSteps = 0;
        }
    }

    class Controller
    {
        Drive::Driver *driverActuators, *driverBox;
        Hardware::Stepper *doorStepper;

    public:
        Controller()
        {
            driverActuators = new Drive::Driver(
                Config::Pins::StepperActuator1PulusePin, Config::Pins::StepperActuator1DirectionPin,
                Config::Pins::StepperActuator2PulusePin, Config::Pins::StepperActuator2DirectionPin);

            driverBox = new Drive::Driver(
                Config::Pins::StepperBox1PulusePin, Config::Pins::StepperBox1DirectionPin,
                Config::Pins::StepperBox2PulusePin, Config::Pins::StepperBox2DirectionPin);

            doorStepper = new Hardware::Stepper(
                Config::Pins::StepperDoorPulusePin, Config::Pins::StepperDoorDirectionPin);
        }

        Controller(bool telemetry)
        {
            driverActuators = new Drive::Driver(
                Config::Pins::StepperActuator1PulusePin, Config::Pins::StepperActuator1DirectionPin,
                Config::Pins::StepperActuator2PulusePin, Config::Pins::StepperActuator2DirectionPin, telemetry);

            driverBox = new Drive::Driver(
                Config::Pins::StepperBox1PulusePin, Config::Pins::StepperBox1DirectionPin,
                Config::Pins::StepperBox2PulusePin, Config::Pins::StepperBox2DirectionPin, telemetry);

            doorStepper = new Hardware::Stepper(
                Config::Pins::StepperDoorPulusePin, Config::Pins::StepperDoorDirectionPin, telemetry);
        }

        ~Controller()
        {
            delete driverActuators;
            delete driverBox;
        }

        void runToPosition(String system, String position)
        {
            if (system == "ACTUATOR")
            {
                if (position == "FIRST")
                    driverActuators->setPosition(Config::Coordinates::FirstBoxPositionSteps);
                else if (position == "SECOND")
                    driverActuators->setPosition(Config::Coordinates::SecondBoxPositionSteps);
                else if (position == "THIRD")
                    driverActuators->setPosition(Config::Coordinates::ThirdBoxPositionSteps);
                else if (position == "REST")
                    driverActuators->setPosition(Config::Coordinates::RestPositionSteps);
            }
            else if (system == "BOX")
            {
                if (position == "FIRST")
                    driverBox->setPosition(Config::Coordinates::FirstBoxPositionSteps);
                else if (position == "SECOND")
                    driverBox->setPosition(Config::Coordinates::SecondBoxPositionSteps);
                else if (position == "THIRD")
                    driverBox->setPosition(Config::Coordinates::ThirdBoxPositionSteps);
                else if (position == "REST")
                    driverBox->setPosition(Config::Coordinates::RestPositionSteps);
            }
            else if (system == "DOOR")
                if (position == "OPEN")
                    doorStepper->setPosition(Config::Coordinates::DoorOpenPositionSteps);
                else if (position == "CLOSE")
                    doorStepper->setPosition(Config::Coordinates::RestPositionSteps);
        }

        void stop()
        {
            digitalWrite(Config::Pins::StepperEnablePin, HIGH);
            driverActuators->forceStop();
            driverBox->forceStop();
            doorStepper->forceStop();
        }
    };
}

#endif