#include <HardwareSerial.h>
#include "controller.h"
#include "uart.h"

HardwareSerial uartSerial(0);

Control::Controller controller(true);
COM::UART uart(uartSerial);

void setup()
{
  Serial.begin(115200);
  uartSerial.begin(115200, SERIAL_8N1, 3, 1);

  delay(2000);
}

void loop()
{
  if (uart.isReadyToReceive())
  {
    std::pair<String, String> data = uart.recieve();
    controller.runToPosition(data.first, data.second);

    uart.send("RECIEVED");
  }
  uart.update();
  delay(100);
}