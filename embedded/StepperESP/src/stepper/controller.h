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

            const int FirstBoxPositionSteps = 10000;
            const int SecondBoxPositionSteps = 20000;
            const int ThirdBoxPositionSteps = 30000;
            const int RestPositionSteps = 0;
        }
    }

    class Controller
    {
        Drive::Driver actuatorsDriver, boxDriver, doorDriver;

    public:
        Controller()
        {
            actuatorsDriver = Drive::Driver(
                Config::Pins::Stepper::Actuator1PulusePin, Config::Pins::Stepper::Actuator1DirectionPin,
                Config::Pins::Stepper::Actuator2PulusePin, Config::Pins::Stepper::Actuator2DirectionPin,
                Config::Pins::LimitSwitch::ActuatorPin);

            boxDriver = Drive::Driver(
                Config::Pins::Stepper::Box1PulusePin, Config::Pins::Stepper::Box1DirectionPin,
                Config::Pins::Stepper::Box2PulusePin, Config::Pins::Stepper::Box2DirectionPin,
                Config::Pins::LimitSwitch::BoxPin);

            doorDriver = Drive::Driver(
                Config::Pins::Stepper::DoorPulusePin, Config::Pins::Stepper::DoorDirectionPin,
                Config::Pins::LimitSwitch::DoorPin);

            pinMode(Config::Pins::Stepper::EnablePin, OUTPUT);
        }

        Controller(bool telemetry)
        {
            actuatorsDriver = Drive::Driver(
                Config::Pins::Stepper::Actuator1PulusePin, Config::Pins::Stepper::Actuator1DirectionPin,
                Config::Pins::Stepper::Actuator2PulusePin, Config::Pins::Stepper::Actuator2DirectionPin,
                Config::Pins::LimitSwitch::ActuatorPin, telemetry);

            boxDriver = Drive::Driver(
                Config::Pins::Stepper::Box1PulusePin, Config::Pins::Stepper::Box1DirectionPin,
                Config::Pins::Stepper::Box2PulusePin, Config::Pins::Stepper::Box2DirectionPin,
                Config::Pins::LimitSwitch::BoxPin, telemetry);

            doorDriver = Drive::Driver(
                Config::Pins::Stepper::DoorPulusePin, Config::Pins::Stepper::DoorDirectionPin,
                Config::Pins::LimitSwitch::DoorPin, telemetry);

            pinMode(Config::Pins::Stepper::EnablePin, OUTPUT);
        }

        void runToPosition(String system, String position)
        {
            if (system == "ACTUATOR")
            {
                if (position == "FIRST")
                    actuatorsDriver.moveTo(Config::Coordinates::FirstBoxPositionSteps);
                else if (position == "SECOND")
                    actuatorsDriver.moveTo(Config::Coordinates::SecondBoxPositionSteps);
                else if (position == "THIRD")
                    actuatorsDriver.moveTo(Config::Coordinates::ThirdBoxPositionSteps);
                else if (position == "REST")
                    actuatorsDriver.moveTo(Config::Coordinates::RestPositionSteps);
            }
            else if (system == "BOX")
            {
                if (position == "FIRST")
                    boxDriver.moveTo(Config::Coordinates::FirstBoxPositionSteps);
                else if (position == "SECOND")
                    boxDriver.moveTo(Config::Coordinates::SecondBoxPositionSteps);
                else if (position == "THIRD")
                    boxDriver.moveTo(Config::Coordinates::ThirdBoxPositionSteps);
                else if (position == "REST")
                    boxDriver.moveTo(Config::Coordinates::RestPositionSteps);
            }
            else if (system == "DOOR")
                if (position == "OPEN")
                    doorDriver.moveTo(Config::Coordinates::DoorOpenPositionSteps);
                else if (position == "CLOSE")
                    doorDriver.moveTo(Config::Coordinates::RestPositionSteps);
        }

        void stop()
        {
            digitalWrite(Config::Pins::Stepper::EnablePin, HIGH);
            actuatorsDriver.forceStop();
            boxDriver.forceStop();
            doorDriver.forceStop();
        }
    };
}

#endif