#ifndef _UART_H_
#define _UART_H_

#include <utility>

namespace COM
{
    class UART
    {
        HardwareSerial &serial;
        bool readyToRecieve = false;

        std::pair<String, String> data;

    public:
        UART(
            HardwareSerial &serial) : serial(serial)
        {
            serial.begin(115200);
        }

        void update()
        {
            String buffer;
            if (serial.available())
            {
                String buffer;
                while (serial.available())
                    buffer += (char)serial.read();
                data.first = buffer.substring(0, buffer.indexOf(','));
                data.second = buffer.substring(buffer.indexOf(',') + 1);
                readyToRecieve = true;
            }
        }

        bool isReadyToReceive()
        {
            return readyToRecieve;
        }

        std::pair<String, String> recieve()
        {
            if (!readyToRecieve)
                throw std::runtime_error("Data not ready to recieve");

            readyToRecieve = false;
            return data;
        }

        void send(String str)
        {
            serial.write(str.c_str());
        }
    };
}

#endif