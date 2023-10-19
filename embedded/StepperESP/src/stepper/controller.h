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
            namespace Stepper
            {
                const int Actuator1PulusePin = 32;
                const int Actuator1DirectionPin = 33;

                const int Actuator2PulusePin = 25;
                const int Actuator2DirectionPin = 33;

                const int Box1PulusePin = 26;
                const int Box1DirectionPin = 27;

                const int Box2PulusePin = 14;
                const int Box2DirectionPin = 27;

                const int DoorPulusePin = 12;
                const int DoorDirectionPin = 13;

                const int EnablePin = 2;
            }

            namespace LimitSwitch
            {
                const int ActuatorPin = 34;
                const int BoxPin = 35;
                const int DoorPin = 36;
            }
        }

        namespace Coordinates
        {
            const int DoorOpenPositionSteps = 1000;

            const int FirstBoxPositionSteps = 100000;
            const int SecondBoxPositionSteps = 200000;
            const int ThirdBoxPositionSteps = 30000;
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
                Config::Pins::Stepper::Actuator1PulusePin, Config::Pins::Stepper::Actuator1DirectionPin,
                Config::Pins::Stepper::Actuator2PulusePin, Config::Pins::Stepper::Actuator2DirectionPin,
                Config::Pins::LimitSwitch::ActuatorPin);

            driverBox = new Drive::Driver(
                Config::Pins::Stepper::Box1PulusePin, Config::Pins::Stepper::Box1DirectionPin,
                Config::Pins::Stepper::Box2PulusePin, Config::Pins::Stepper::Box2DirectionPin,
                Config::Pins::LimitSwitch::BoxPin);

            doorStepper = new Hardware::Stepper(
                Config::Pins::Stepper::DoorPulusePin, Config::Pins::Stepper::DoorDirectionPin);

            pinMode(Config::Pins::Stepper::EnablePin, OUTPUT);
        }

        Controller(bool telemetry)
        {
            driverActuators = new Drive::Driver(
                Config::Pins::Stepper::Actuator1PulusePin, Config::Pins::Stepper::Actuator1DirectionPin,
                Config::Pins::Stepper::Actuator2PulusePin, Config::Pins::Stepper::Actuator2DirectionPin,
                Config::Pins::LimitSwitch::ActuatorPin, telemetry);

            // driverBox = new Drive::Driver(
            //     Config::Pins::Stepper::Box1PulusePin, Config::Pins::Stepper::Box1DirectionPin,
            //     Config::Pins::Stepper::Box2PulusePin, Config::Pins::Stepper::Box2DirectionPin,
            //     Config::Pins::LimitSwitch::BoxPin, telemetry);

            doorStepper = new Hardware::Stepper(
                Config::Pins::Stepper::DoorPulusePin, Config::Pins::Stepper::DoorDirectionPin, telemetry);

            pinMode(Config::Pins::Stepper::EnablePin, OUTPUT);
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
                    driverActuators->moveTo(Config::Coordinates::FirstBoxPositionSteps);
                else if (position == "SECOND")
                    driverActuators->moveTo(Config::Coordinates::SecondBoxPositionSteps);
                else if (position == "THIRD")
                    driverActuators->moveTo(Config::Coordinates::ThirdBoxPositionSteps);
                else if (position == "REST")
                    driverActuators->moveTo(Config::Coordinates::RestPositionSteps);
            }
            else if (system == "BOX")
            {
                if (position == "FIRST")
                    driverBox->moveTo(Config::Coordinates::FirstBoxPositionSteps);
                else if (position == "SECOND")
                    driverBox->moveTo(Config::Coordinates::SecondBoxPositionSteps);
                else if (position == "THIRD")
                    driverBox->moveTo(Config::Coordinates::ThirdBoxPositionSteps);
                else if (position == "REST")
                    driverBox->moveTo(Config::Coordinates::RestPositionSteps);
            }
            else if (system == "DOOR")
                if (position == "OPEN")
                    doorStepper->moveTo(Config::Coordinates::DoorOpenPositionSteps);
                else if (position == "CLOSE")
                    doorStepper->moveTo(Config::Coordinates::RestPositionSteps);
        }

        void stop()
        {
            digitalWrite(Config::Pins::Stepper::EnablePin, HIGH);
            driverActuators->forceStop();
            driverBox->forceStop();
            doorStepper->forceStop();
        }
    };
}

#endif