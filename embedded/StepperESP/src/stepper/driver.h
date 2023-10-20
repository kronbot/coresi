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
        Hardware::Stepper *stepper1, *stepper2;
        Hardware::LimitSwitch *limitSwitch;

        std::thread stepper1Thread;
        std::thread stepper2Thread;
        std::thread limitSwitchThread;

        bool reseted = false;

        void stepper1Run()
        {
            esp_task_wdt_init(60, false);
            while (true)
            {
                stepper1->run();
                delay(500);
            }
        }

        void stepper2Run()
        {
            esp_task_wdt_init(60, false);
            while (true)
            {
                stepper2->run();
                delay(500);
            }
        }

        void limitSwitchRun()
        {
            esp_task_wdt_init(60, false);
            while (true)
            {
                if (limitSwitch->isPressed())
                    reset();

                delay(50);
            }
        }

    public:
        Driver(int pulsePin1, int directionPin1, int pulsePin2, int directionPin2, int limitSwitchPin)
        {
            this->stepper1 = new Hardware::Stepper(pulsePin1, directionPin1);
            this->stepper2 = new Hardware::Stepper(pulsePin2, directionPin2);
            this->limitSwitch = new Hardware::LimitSwitch(limitSwitchPin);

            stepper1Thread = std::thread(&Driver::stepper1Run, this);
            stepper2Thread = std::thread(&Driver::stepper2Run, this);
            limitSwitchThread = std::thread(&Driver::limitSwitchRun, this);

            moveTo(-100000);

            while (!reseted)
                delay(50);
        }

        Driver(int pulsePin1, int directionPin1, int pulsePin2, int directionPin2, int limitSwitchPin, bool telemetry)
        {
            this->stepper1 = new Hardware::Stepper(pulsePin1, directionPin1, telemetry);
            this->stepper2 = new Hardware::Stepper(pulsePin2, directionPin2, telemetry);
            this->limitSwitch = new Hardware::LimitSwitch(limitSwitchPin, telemetry);

            stepper1Thread = std::thread(&Driver::stepper1Run, this);
            stepper2Thread = std::thread(&Driver::stepper2Run, this);
            limitSwitchThread = std::thread(&Driver::limitSwitchRun, this);

            moveTo(-100000);

            while (!reseted)
                delay(50);
        }

        ~Driver()
        {
            stepper1Thread.join();
            stepper2Thread.join();

            delete stepper1;
            delete stepper2;
        }

        void forceStop()
        {
            stepper1->forceStop();
            stepper2->forceStop();
        }

        void moveTo(int position)
        {
            stepper1->moveTo(position);
            stepper2->moveTo(position);
        }

        void reset()
        {
            stepper1->reset();
            stepper2->reset();
            reseted = true;
        }
    };
}

#endif