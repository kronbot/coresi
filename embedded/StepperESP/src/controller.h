#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "driver.h"

namespace Control
{
    namespace Config
    {
        namespace Pins
        {
            namespace Stepper
            {
                const int ACTUATOR_1_PULSE_PIN = 32;
                const int ACTUATOR_1_DIRECTION_PIN = 33;

                const int ACTUATOR_2_PULSE_PIN = 25;
                const int ACTUATOR_2_DIRECTION_PIN = 26;

                const int BOX_1_PULSE_PIN = 27;
                const int BOX_1_DIRECTION_PIN = 14;

                const int BOX_2_PULSE_PIN = 12;
                const int BOX_2_DIRECTION_PIN = 13;

                const int DOOR_PULSE_PIN = 5;
                const int DOOR_DIRECTION_PIN = 4;

                const int ENABLE_PIN = 2;
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
                Config::Pins::Stepper::ACTUATOR_1_PULSE_PIN, Config::Pins::Stepper::ACTUATOR_1_DIRECTION_PIN,
                Config::Pins::Stepper::ACTUATOR_2_PULSE_PIN, Config::Pins::Stepper::ACTUATOR_2_DIRECTION_PIN,
                Config::Pins::LimitSwitch::ActuatorPin);

            boxDriver = Drive::Driver(
                Config::Pins::Stepper::BOX_1_PULSE_PIN, Config::Pins::Stepper::BOX_1_DIRECTION_PIN,
                Config::Pins::Stepper::BOX_2_PULSE_PIN, Config::Pins::Stepper::BOX_2_DIRECTION_PIN,
                Config::Pins::LimitSwitch::BoxPin);

            doorDriver = Drive::Driver(
                Config::Pins::Stepper::DOOR_PULSE_PIN, Config::Pins::Stepper::DOOR_DIRECTION_PIN,
                Config::Pins::LimitSwitch::DoorPin);

            pinMode(Config::Pins::Stepper::ENABLE_PIN, OUTPUT);
        }

        Controller(bool telemetry)
        {
            actuatorsDriver = Drive::Driver(
                Config::Pins::Stepper::ACTUATOR_1_PULSE_PIN, Config::Pins::Stepper::ACTUATOR_1_DIRECTION_PIN,
                Config::Pins::Stepper::ACTUATOR_2_PULSE_PIN, Config::Pins::Stepper::ACTUATOR_2_DIRECTION_PIN,
                Config::Pins::LimitSwitch::ActuatorPin, telemetry);

            boxDriver = Drive::Driver(
                Config::Pins::Stepper::BOX_1_PULSE_PIN, Config::Pins::Stepper::BOX_1_DIRECTION_PIN,
                Config::Pins::Stepper::BOX_2_PULSE_PIN, Config::Pins::Stepper::BOX_2_DIRECTION_PIN,
                Config::Pins::LimitSwitch::BoxPin, telemetry);

            doorDriver = Drive::Driver(
                Config::Pins::Stepper::DOOR_PULSE_PIN, Config::Pins::Stepper::DOOR_DIRECTION_PIN,
                Config::Pins::LimitSwitch::DoorPin, telemetry);

            pinMode(Config::Pins::Stepper::ENABLE_PIN, OUTPUT);
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
            digitalWrite(Config::Pins::Stepper::ENABLE_PIN, HIGH);
            actuatorsDriver.forceStop();
            boxDriver.forceStop();
            doorDriver.forceStop();
        }
    };
}

#endif