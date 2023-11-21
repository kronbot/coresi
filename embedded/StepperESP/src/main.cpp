#include <HardwareSerial.h>
#include "controller.h"
#include "uart.h"

HardwareSerial uartSerial(0);

Control::Controller controller;
COM::UART uart(uartSerial);

bool telemetry = false;

void setup()
{
    if (telemetry)
        Serial.begin(115200);

    uartSerial.begin(115200, SERIAL_8N1, 3, 1);

    uart.send("START");

    controller.init(telemetry);

    // delay(20000);

    String data = controller.getSendData();
    uart.send("INIT");

    if (telemetry)
        Serial.println("Init");

    controller.run("ACTUATOR", "FIRST");

    delay(50000);
}

void loop()
{
    if (uart.isReadyToReceive())
    {
        std::pair<String, String> data = uart.recieve();
        bool response = controller.run(data.first, data.second);

        if (response)
            uart.send("RECIEVED");
        else
            uart.send("BUSY");
    }

    String sendData = controller.getSendData();
    if (sendData != "")
        uart.send(sendData);

    uart.update();
    delay(500);
}