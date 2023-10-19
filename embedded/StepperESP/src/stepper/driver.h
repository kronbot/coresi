#ifndef _DRIVER_H_
#define _DRIVER_H_

#include <thread>
#include <mutex>
#include "hardware.h"

namespace Drive
{
    class Driver
    {
        Hardware::Stepper *stepper1, *stepper2;

        std::thread thread;
        std::mutex mutex;
        bool running = true;

        void run()
        {
            while (running)
            {
                if (stepper1->distance() != 0 || stepper2->distance() != 0)
                {
                    // stepper1->run();
                    // stepper2->run();
                }

                delay(5);
            }
        }

    public:
        Driver(int pulsePin1, int directionPin1, int pulsePin2, int directionPin2)
        {
            this->stepper1 = new Hardware::Stepper(pulsePin1, directionPin1);
            this->stepper2 = new Hardware::Stepper(pulsePin2, directionPin2);

            thread = std::thread(&Driver::run, this);
        }

        Driver(int pulsePin1, int directionPin1, int pulsePin2, int directionPin2, bool telemetry)
        {
            this->stepper1 = new Hardware::Stepper(pulsePin1, directionPin1, telemetry);
            this->stepper2 = new Hardware::Stepper(pulsePin2, directionPin2, telemetry);

            thread = std::thread(&Driver::run, this);
        }

        ~Driver()
        {
            mutex.lock();
            running = false;
            mutex.unlock();
            thread.join();

            delete stepper1;
            delete stepper2;
        }

        void forceStop()
        {
            stepper1->forceStop();
            stepper2->forceStop();
        }

        void setPosition(int position)
        {
            stepper1->setPosition(position);
            stepper2->setPosition(position);
        }
    };
}

#endif