#ifndef _DRIVER_H_
#define _DRIVER_H_

#include "hardware.h"
#include <esp_task_wdt.h>
#include <thread>

namespace Drive
{
    class MotorDriver
    {
        Hardware::DCMotor motor1, motor2;

        std::thread motorThread;
        bool direction;
        bool action;
        bool telemetry;

        void motorRun()
        {
            esp_task_wdt_init(60, false);
            while (true)
            {
                if (action)
                {
                    action = false;
                    motor1.control(!direction);
                    motor2.control(!direction);
                    delay(20000);
                    forceStop();
                }
                delay(500);
            }
        }

    public:
        MotorDriver() = default;

        MotorDriver(int motor1Pin1, int motor1Pin2, int motor1Enable, int motor2Pin1, int motor2Pin2, int motor2Enable, bool telemetry)
        {
            this->telemetry = telemetry;

            motor1 = Hardware::DCMotor(motor1Pin1, motor1Pin2, motor1Enable, telemetry);
            motor2 = Hardware::DCMotor(motor2Pin1, motor2Pin2, motor2Enable, telemetry);

            motorThread = std::thread(&MotorDriver::motorRun, this);

            close();
        }

        ~MotorDriver()
        {
            motorThread.join();
        }

        void forceStop()
        {
            motor1.forceStop();
            motor2.forceStop();
        }

        void open()
        {
            action = true;
            direction = true;
            if (telemetry)
                Serial.println("Open Motor");
        }

        void close()
        {
            action = true;
            direction = false;
            if (telemetry)
                Serial.println("Close Motor");
        }

        MotorDriver &operator=(const MotorDriver &other)
        {
            this->motor1 = other.motor1;
            this->motor2 = other.motor2;
            this->action = other.action;
            this->direction = other.direction;

            return *this;
        }
    };

    class ServoDriver
    {
        Hardware::ESPServo servo1, servo2, servo3, servo4;
        Hardware::UltraSonic ultraSonic;
        Hardware::LedStrip ledStrip;

        std::thread servoThread;
        bool action;
        bool direction;
        bool telemetry;
        String ledPosition;

        void servoRun()
        {
            esp_task_wdt_init(60, false);
            while (true)
            {
                if (action)
                {
                    action = false;

                    if (direction)
                    {
                        servo1.write(90);
                        servo2.write(90);
                        servo3.write(90);
                        servo4.write(90);
                    }
                    else
                    {
                        servo1.write(0);
                        servo2.write(0);
                        servo3.write(0);
                        servo4.write(0);
                    }

                    delay(3000);

                    // if (direction == false)
                    // {
                    //     ultraSonic.getDistance();

                    //     ledStrip.setColor(0, 0, 0);
                    // }

                    delay(2000);
                }
                delay(500);
            }
        }

    public:
        ServoDriver() = default;

        ServoDriver(int servo1Pin, int servo2Pin, int servo3Pin, int servo4Pin, int ultraSonicTrigPin, int ultraSonicEchoPin, int ledRedPin, int ledGreenPin, int ledBluePin, bool telemetry)
        {
            this->telemetry = telemetry;
            servo1 = Hardware::ESPServo(servo1Pin, telemetry);
            servo2 = Hardware::ESPServo(servo2Pin, telemetry);
            servo3 = Hardware::ESPServo(servo3Pin, telemetry);
            servo4 = Hardware::ESPServo(servo4Pin, telemetry);
            ultraSonic = Hardware::UltraSonic(ultraSonicTrigPin, ultraSonicEchoPin, telemetry);
            ledStrip = Hardware::LedStrip(ledRedPin, ledGreenPin, ledBluePin, telemetry);

            servoThread = std::thread(&ServoDriver::servoRun, this);

            close();
        }

        void open()
        {
            action = true;
            direction = true;
            if (telemetry)
                Serial.println("Open Servo");
        }

        void close()
        {
            action = true;
            direction = false;
            if (telemetry)
                Serial.println("Close Servo");
        }

        double getDistance()
        {
            return ultraSonic.getDistance();
        }

        void setLedPosition(String position)
        {
            ledPosition = position;
        }

        ServoDriver &operator=(const ServoDriver &other)
        {
            this->servo1 = other.servo1;
            this->servo2 = other.servo2;
            this->servo3 = other.servo3;
            this->servo4 = other.servo4;
            this->ultraSonic = other.ultraSonic;
            this->action = other.action;
            this->direction = other.direction;

            return *this;
        }
    };
}

#endif