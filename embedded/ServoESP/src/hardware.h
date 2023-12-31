#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#include "ESP32Servo.h"

namespace Hardware
{
    namespace Config
    {
        const double SOUND_SPEED = 0.034;

        const int DC_MOTOR_SPEED = 255;

        const int ULTRASONIC_SENSOR_READ_STEPS = 100;
    }

    class ESPServo
    {
        Servo servo;
        bool telemetry;

    public:
        void init(int pin, bool telemetry)
        {
            servo.attach(pin);
            this->telemetry = telemetry;
        }

        void write(int value)
        {
            servo.write(value);
        }
    };

    class UltraSonic
    {
        int trigPin;
        int echoPin;
        double distance;
        bool telemetry;

    public:
        void init(int trigPin, int echoPin, bool telemetry)
        {
            this->trigPin = trigPin;
            this->echoPin = echoPin;
            this->telemetry = telemetry;

            pinMode(trigPin, OUTPUT);
            pinMode(echoPin, INPUT);
        }

        void readDistance()
        {
            double totalDistance = 0;

            for (int step = 0; step < Config::ULTRASONIC_SENSOR_READ_STEPS; step++)
            {
                digitalWrite(trigPin, LOW);
                delayMicroseconds(2);
                digitalWrite(trigPin, HIGH);
                delayMicroseconds(10);
                digitalWrite(trigPin, LOW);

                double duration = pulseIn(echoPin, HIGH);
                totalDistance += duration * Config::SOUND_SPEED / 2;
            }

            distance = totalDistance / Config::ULTRASONIC_SENSOR_READ_STEPS;

            if (telemetry)
            {
                Serial.print("Distance: ");
                Serial.println(distance);
            }
        }

        double getDistance()
        {
            return distance;
        }
    };

    class DCMotor
    {
        int pin1, pin2, enablePin;
        int speed;
        bool telemetry;

    public:
        void init(int pin1, int pin2, int enablePin, bool telemetry)
        {
            this->pin1 = pin1;
            this->pin2 = pin2;
            this->enablePin = enablePin;
            this->telemetry = telemetry;

            pinMode(pin1, OUTPUT);
            pinMode(pin2, OUTPUT);
            pinMode(enablePin, OUTPUT);

            analogWrite(enablePin, Config::DC_MOTOR_SPEED);
            digitalWrite(pin1, LOW);
            digitalWrite(pin2, LOW);
        }

        void forceStop()
        {
            digitalWrite(pin1, LOW);
            digitalWrite(pin2, LOW);
        }

        void setSpeed(int speed)
        {
            this->speed = speed;
            analogWrite(enablePin, speed);
        }

        void control(bool direction)
        {
            if (direction)
            {
                digitalWrite(pin1, LOW);
                digitalWrite(pin2, HIGH);
            }
            else
            {
                digitalWrite(pin1, HIGH);
                digitalWrite(pin2, LOW);
            }
        }
    };

    class LedStrip
    {
        int redPin, greenPin, bluePin;
        bool telemetry;

    public:
        void init(int redPin, int greenPin, int bluePin, bool telemetry)
        {
            this->redPin = redPin;
            this->greenPin = greenPin;
            this->bluePin = bluePin;
            this->telemetry = telemetry;

            pinMode(redPin, OUTPUT);
            pinMode(greenPin, OUTPUT);
            pinMode(bluePin, OUTPUT);
        }

        void setColor(int red, int green, int blue)
        {
            analogWrite(redPin, red);
            analogWrite(greenPin, green);
            analogWrite(bluePin, blue);
        }
    };
}

#endif