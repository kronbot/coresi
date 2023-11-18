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

        enum class State
        {
            OPEN,
            CLOSE,
            FULL,
            BUSY,
            ZERO
        };

        std::thread motorThread;

        State state;
        String data;

        bool telemetry;

        void motorRun()
        {
            esp_task_wdt_init(120, false);
            while (true)
            {
                if (state == State::OPEN)
                {
                    state = State::BUSY;

                    motor1.control(true);
                    motor2.control(true);
                    delay(20000);
                    forceStop();

                    state = State::ZERO;

                    data = "FINISHED";
                }
                else if (state == State::CLOSE)
                {
                    state = State::BUSY;

                    motor1.control(false);
                    motor2.control(false);
                    delay(20000);
                    forceStop();

                    state = State::ZERO;

                    data = "FINISHED";
                }
                else if (state == State::FULL)
                {
                    state = State::BUSY;

                    motor1.control(true);
                    motor2.control(true);
                    delay(20000);
                    motor1.control(false);
                    motor2.control(false);
                    delay(20000);
                    forceStop();

                    state = State::ZERO;

                    data = "FINISHED";
                }
                delay(500);
            }
        }

    public:
        ~MotorDriver()
        {
            motorThread.join();
        }

        void init(int motor1Pin1, int motor1Pin2, int motor1Enable, int motor2Pin1, int motor2Pin2, int motor2Enable, bool telemetry)
        {
            this->telemetry = telemetry;

            motor1.init(motor1Pin1, motor1Pin2, motor1Enable, telemetry);
            motor2.init(motor2Pin1, motor2Pin2, motor2Enable, telemetry);

            motorThread = std::thread(&MotorDriver::motorRun, this);

            close();
        }

        void forceStop()
        {
            motor1.forceStop();
            motor2.forceStop();
        }

        bool isFree()
        {
            return state == State::ZERO;
        }

        void open()
        {
            state = State::OPEN;
            if (telemetry)
                Serial.println("Open Motor");
        }

        void close()
        {
            state = State::CLOSE;
            if (telemetry)
                Serial.println("Close Motor");
        }

        void full()
        {
            state = State::FULL;
            if (telemetry)
                Serial.println("State Motor");
        }

        String getSendData()
        {
            String temp = data;
            data = "";
            return temp;
        }
    };

    class ServoDriver
    {
        Hardware::ESPServo servo1, servo2, servo3, servo4;
        Hardware::UltraSonic ultraSonic;
        Hardware::LedStrip ledStrip;

        std::thread servoThread;

        enum class State
        {
            OPEN,
            CLOSE,
            BUSY,
            ZERO
        };

        State state;
        String ledPosition;
        String data;

        bool telemetry;

        String doubleToString(double value)
        {
            String str = String(value);
            return str.substring(0, str.indexOf('.') + 3);
        }

        void servoRun()
        {
            esp_task_wdt_init(60, false);
            while (true)
            {
                if (state == State::OPEN)
                {
                    state = State::BUSY;

                    servo1.write(90);
                    servo2.write(90);
                    servo3.write(90);
                    servo4.write(90);

                    delay(3000);

                    state = State::ZERO;

                    data = doubleToString(ultraSonic.getDistance());
                    ledStrip.setColor(0, 0, 0);
                }
                else if (state == State::CLOSE)
                {
                    state = State::BUSY;

                    servo1.write(0);
                    servo2.write(0);
                    servo3.write(0);
                    servo4.write(0);

                    state = State::ZERO;
                }
                delay(500);
            }
        }

    public:
        void init(int servo1Pin, int servo2Pin, int servo3Pin, int servo4Pin, int ultraSonicTrigPin, int ultraSonicEchoPin, int ledRedPin, int ledGreenPin, int ledBluePin, bool telemetry)
        {
            this->telemetry = telemetry;
            servo1.init(servo1Pin, telemetry);
            servo2.init(servo2Pin, telemetry);
            servo3.init(servo3Pin, telemetry);
            servo4.init(servo4Pin, telemetry);
            ultraSonic.init(ultraSonicTrigPin, ultraSonicEchoPin, telemetry);
            ledStrip.init(ledRedPin, ledGreenPin, ledBluePin, telemetry);

            servoThread = std::thread(&ServoDriver::servoRun, this);

            close();
        }

        bool isFree()
        {
            return state == State::ZERO;
        }

        void open()
        {
            state = State::OPEN;
            if (telemetry)
                Serial.println("Open Servo");
        }

        void close()
        {
            state = State::CLOSE;
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

        String getSendData()
        {
            String temp = data;
            data = "";
            return temp;
        }
    };
}

#endif