#ifndef _DRIVER_H_
#define _DRIVER_H_

#include "hardware.h"
#include <esp_task_wdt.h>
#include <thread>
#include <mutex>

namespace Drive
{
    class Driver
    {
        Hardware::Stepper stepper1, stepper2;
        Hardware::LimitSwitch limitSwitch;

        std::thread stepper1Thread;
        std::thread stepper2Thread;
        std::thread limitSwitchThread;

        enum class State
        {
            BUSY,
            ZERO
        };

        State state;
        bool reseted = false;
        bool singleStepper = false;

        String data;

        void stepper1Run()
        {
            esp_task_wdt_init(120, false);
            while (true)
            {
                stepper1.run();
                if (state != State::ZERO)
                {
                    if (data == "")
                        data = "FINISHED";
                    state = State::ZERO;
                }
                delay(500);
            }
        }

        void stepper2Run()
        {
            esp_task_wdt_init(120, false);
            while (true)
            {
                stepper2.run();
                if (state != State::ZERO)
                {
                    if (data == "")
                        data = "FINISHED";
                    state = State::ZERO;
                }
                delay(500);
            }
        }

        void limitSwitchRun()
        {
            esp_task_wdt_init(120, false);
            while (true)
            {
                if (limitSwitch.isPressed())
                    reset();

                delay(100);
            }
        }

    public:
        void initDual(int pulsePin1, int directionPin1, int pulsePin2, int directionPin2, int limitSwitchPin, bool telemetry)
        {
            stepper1.init(pulsePin1, directionPin1, telemetry);
            stepper2.init(pulsePin2, directionPin2, telemetry);
            limitSwitch.init(limitSwitchPin, telemetry);

            stepper1Thread = std::thread(&Driver::stepper1Run, this);
            stepper2Thread = std::thread(&Driver::stepper2Run, this);
            limitSwitchThread = std::thread(&Driver::limitSwitchRun, this);

            moveTo(-100000);

            while (!reseted)
                delay(50);
        }

        void initSingle(int pulsePin1, int directionPin1, int limitSwitchPin, bool telemetry)
        {
            stepper1.init(pulsePin1, directionPin1, telemetry);
            limitSwitch.init(limitSwitchPin, telemetry);

            singleStepper = true;

            stepper1Thread = std::thread(&Driver::stepper1Run, this);
            limitSwitchThread = std::thread(&Driver::limitSwitchRun, this);

            moveTo(-100000);

            while (!reseted)
                delay(50);
        }

        ~Driver()
        {
            stepper1Thread.join();
            stepper2Thread.join();
        }

        void forceStop()
        {
            stepper1.forceStop();
            if (!singleStepper)
                stepper2.forceStop();
            state = State::ZERO;
        }

        bool isFree()
        {
            return state == State::ZERO;
        }

        void moveTo(int position)
        {
            stepper1.moveTo(position);
            if (!singleStepper)
                stepper2.moveTo(-position);

            state = State::BUSY;
        }

        void reset()
        {
            stepper1.reset();
            if (!singleStepper)
                stepper2.reset();
            reseted = true;

            state = State::ZERO;

            data = "RESETED";
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