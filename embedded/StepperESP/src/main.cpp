#include <HardwareSerial.h>
#include "stepper/controller.h"
#include "uart.h"

Control::Controller *controller;
COM::UART *uart;

HardwareSerial uartSerial(2);

void setup()
{
  Serial.begin(115200);
  uartSerial.begin(115200, SERIAL_8N1, 16, 17);

  controller = new Control::Controller(true);
  uart = new COM::UART(uartSerial);

  delay(2000);
}

void loop()
{
  // if (uart->isReadyToReceive())
  // {
  //   std::pair<String, String> data = uart->recieve();
  //   controller->runToPosition(data.first, data.second);

  //   uart->send("RECIEVED");
  // }
  // uart->update();
  // delay(5);

  controller->runToPosition("ACTUATOR", "FIRST");

  delay(10000);

  controller->runToPosition("ACTUATOR", "SECOND");

  delay(10000);
  // controller->stop();
}