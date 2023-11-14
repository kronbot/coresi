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
            namespace Actuator
            {
                const int MOTOR_2_PIN1 = 33;
                const int MOTOR_2_PIN2 = 32;
                const int MOTOR_2_ENABLE = 25;

                const int MOTOR_1_PIN1 = 27;
                const int MOTOR_1_PIN2 = 14;
                const int MOTOR_1_ENABLE = 26;
            }

            namespace Servo
            {
                const int SERVO_1_PIN = 2;
                const int SERVO_2_PIN = 0;
                const int SERVO_3_PIN = 18;
                const int SERVO_4_PIN = 19;
            }

            namespace UltraSonic
            {
                const int ULTRASONIC_TRIG = 4;
                const int ULTRASONIC_ECHO = 5;
            }

            namespace LedStrip
            {
                const int LED_RED_PIN = 19;
                const int LED_GREEN_PIN = 21;
                const int LED_BLUE_PIN = 22;
            }
        }
    }

    class Controller
    {
        Drive::MotorDriver actuatorsDriver;
        Drive::ServoDriver servosDriver;

    public:
        Controller() = default;

        Controller(bool telemetry)
        {
            actuatorsDriver = Drive::MotorDriver(
                Config::Pins::Actuator::MOTOR_1_PIN1, Config::Pins::Actuator::MOTOR_1_PIN2, Config::Pins::Actuator::MOTOR_1_ENABLE,
                Config::Pins::Actuator::MOTOR_2_PIN1, Config::Pins::Actuator::MOTOR_2_PIN2, Config::Pins::Actuator::MOTOR_2_ENABLE,
                telemetry);

            servosDriver = Drive::ServoDriver(
                Config::Pins::Servo::SERVO_1_PIN,
                Config::Pins::Servo::SERVO_2_PIN,
                Config::Pins::Servo::SERVO_3_PIN,
                Config::Pins::Servo::SERVO_4_PIN, Config::Pins::UltraSonic::ULTRASONIC_TRIG, Config::Pins::UltraSonic::ULTRASONIC_ECHO, Config::Pins::LedStrip::LED_RED_PIN, Config::Pins::LedStrip::LED_GREEN_PIN, Config::Pins::LedStrip::LED_BLUE_PIN, telemetry);
        }

        void run(String system, String action)
        {
            if (system == "ACTUATOR")
            {
                if (action == "OPEN")
                    actuatorsDriver.open();
                else if (action == "CLOSE")
                    actuatorsDriver.close();
                else if (action == "STOP")
                    actuatorsDriver.forceStop();
            }
            else if (system == "SERVO")
            {
                if (action == "OPEN FIRST")
                {
                    servosDriver.open();
                    servosDriver.setLedPosition("FIRST");
                }
                else if (action == "OPEN SECOND")
                {
                    servosDriver.open();
                    servosDriver.setLedPosition("SECOND");
                }
                else if (action == "OPEN THIRD")
                {
                    servosDriver.open();
                    servosDriver.setLedPosition("THIRD");
                }
                else if (action == "CLOSE")
                    servosDriver.close();
            }
        }

        void stop()
        {
            actuatorsDriver.forceStop();
        }

        double getDistance()
        {
            return servosDriver.getDistance();
        }

        Controller &operator=(const Controller &other)
        {
            actuatorsDriver = other.actuatorsDriver;
            servosDriver = other.servosDriver;

            return *this;
        }
    };
}

#endif