#ifndef _DRIVER_H_
#define _DRIVER_H_

#include "hardware.h"
#include <thread>
#include <mutex>
#include <condition_variable>

namespace Drive
{
    class Driver
    {
        Hardware::Stepper *stepper1, *stepper2;
        Hardware::LimitSwitch *limitSwitch;

        std::mutex mutex;

        std::thread stepper1Thread;
        std::condition_variable stepper1Condition;
        bool stepper1Running = true;

        std::thread stepper2Thread;
        std::condition_variable stepper2Condition;
        bool stepper2Running = true;

        std::thread limitSwitchThread;
        bool limitSwitchRunning = true;

        int stepper1PositionToGo;
        int stepper2PositionToGo;

        void stepper1Run()
        {
            while (stepper1Running)
            {
                if (stepper1PositionToGo != 0)
                {
                    stepper1->run();
                    stepper1PositionToGo = 0;
                }

                delay(100);
            }
        }

        void stepper2Run()
        {
            while (stepper2Running)
            {
                if (stepper2PositionToGo != 0)
                {
                    stepper2->run();
                    stepper2PositionToGo = 0;
                }

                delay(100);
            }
        }

        // void limitSwitchRun()
        // {
        //     // while (limitSwitchRunning)
        //     // {
        //     //     // if (limitSwitch->isPressed())
        //     //     // {
        //     //     //     reset();
        //     //     // }
        //     //     delay(500);
        //     // }
        // }

    public:
        Driver(int pulsePin1, int directionPin1, int pulsePin2, int directionPin2, int limitSwitchPin)
        {
            this->stepper1 = new Hardware::Stepper(pulsePin1, directionPin1);
            this->stepper2 = new Hardware::Stepper(pulsePin2, directionPin2);
            this->limitSwitch = new Hardware::LimitSwitch(limitSwitchPin);

            stepper1Thread = std::thread(&Driver::stepper1Run, this);
            stepper2Thread = std::thread(&Driver::stepper2Run, this);
            // limitSwitchThread = std::thread(&Driver::limitSwitchRun, this);

            // moveTo(-100000);
        }

        Driver(int pulsePin1, int directionPin1, int pulsePin2, int directionPin2, int limitSwitchPin, bool telemetry)
        {
            this->stepper1 = new Hardware::Stepper(pulsePin1, directionPin1, telemetry);
            this->stepper2 = new Hardware::Stepper(pulsePin2, directionPin2, telemetry);
            this->limitSwitch = new Hardware::LimitSwitch(limitSwitchPin, telemetry);

            stepper1Thread = std::thread(&Driver::stepper1Run, this);
            stepper2Thread = std::thread(&Driver::stepper2Run, this);

            stepper1Thread.detach();
            stepper2Thread.detach();

            // limitSwitchThread = std::thread(&Driver::limitSwitchRun, this);

            // moveTo(-100000);
        }

        ~Driver()
        {
            mutex.lock();
            stepper1Running = false;
            stepper2Running = false;
            limitSwitchRunning = false;
            mutex.unlock();

            stepper1Thread.join();
            stepper2Thread.join();
            limitSwitchThread.join();

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

            stepper1PositionToGo = position;
            stepper2PositionToGo = position;
        }

        void reset()
        {
            delay(100);

            stepper1->reset();
            stepper1->reset();

            delay(100);
        }
    };
}

#endif