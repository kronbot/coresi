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

        bool reseted = false;
        bool singleStepper = false;

        void stepper1Run()
        {
            esp_task_wdt_init(60, false);
            while (true)
            {
                stepper1.run();
                delay(500);
            }
        }

        void stepper2Run()
        {
            esp_task_wdt_init(60, false);
            while (true)
            {
                stepper2.run();
                delay(500);
            }
        }

        void limitSwitchRun()
        {
            esp_task_wdt_init(60, false);
            while (true)
            {
                if (limitSwitch.isPressed())
                    reset();

                delay(50);
            }
        }

    public:
        Driver() = default;

        Driver(int pulsePin1, int directionPin1, int pulsePin2, int directionPin2, int limitSwitchPin)
        {
            this->stepper1 = Hardware::Stepper(pulsePin1, directionPin1);
            this->stepper2 = Hardware::Stepper(pulsePin2, directionPin2);
            this->limitSwitch = Hardware::LimitSwitch(limitSwitchPin);

            stepper1Thread = std::thread(&Driver::stepper1Run, this);
            stepper2Thread = std::thread(&Driver::stepper2Run, this);
            limitSwitchThread = std::thread(&Driver::limitSwitchRun, this);

            moveTo(-100000);

            while (!reseted)
                delay(50);
        }

        Driver(int pulsePin1, int directionPin1, int pulsePin2, int directionPin2, int limitSwitchPin, bool telemetry)
        {
            this->stepper1 = Hardware::Stepper(pulsePin1, directionPin1, telemetry);
            this->stepper2 = Hardware::Stepper(pulsePin2, directionPin2, telemetry);
            this->limitSwitch = Hardware::LimitSwitch(limitSwitchPin, telemetry);

            stepper1Thread = std::thread(&Driver::stepper1Run, this);
            stepper2Thread = std::thread(&Driver::stepper2Run, this);
            limitSwitchThread = std::thread(&Driver::limitSwitchRun, this);

            moveTo(-100000);

            while (!reseted)
                delay(50);
        }

        Driver(int pulsePin1, int directionPin1, int limitSwitchPin)
        {
            this->stepper1 = Hardware::Stepper(pulsePin1, directionPin1);
            this->limitSwitch = Hardware::LimitSwitch(limitSwitchPin);

            singleStepper = true;

            stepper1Thread = std::thread(&Driver::stepper1Run, this);
            limitSwitchThread = std::thread(&Driver::limitSwitchRun, this);

            moveTo(-100000);

            while (!reseted)
                delay(50);
        }

        Driver(int pulsePin1, int directionPin1, int limitSwitchPin, bool telemetry)
        {
            this->stepper1 = Hardware::Stepper(pulsePin1, directionPin1, telemetry);
            this->limitSwitch = Hardware::LimitSwitch(limitSwitchPin, telemetry);

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
        }

        void moveTo(int position)
        {
            stepper1.moveTo(position);
            if (!singleStepper)
                stepper2.moveTo(-position);
        }

        void reset()
        {
            stepper1.reset();
            if (!singleStepper)
                stepper2.reset();
            reseted = true;
        }

        Driver &operator=(const Driver &other)
        {
            this->stepper1 = other.stepper1;
            this->stepper2 = other.stepper2;
            this->limitSwitch = other.limitSwitch;

            return *this;
        }
    };
}

#endif