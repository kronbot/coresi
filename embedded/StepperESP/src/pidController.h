#ifndef _PIDCONTROLLER_H_
#define _PIDCONTROLLER_H_

namespace PID
{
    class PIDController
    {
        double kp, ki, kd;
        double previousError = 0;
        double integral = 0;

    public:
        PIDController(double kp, double ki, double kd) : kp(kp), ki(ki), kd(kd) {}

        double compute(double error)
        {
            double output = kp * error + ki * integral + kd * (error - previousError);
            integral += error;
            previousError = error;
            return output;
        }

        void reset()
        {
            previousError = 0;
            integral = 0;
        }
    };
}

#endif