#ifndef AX_12A_H
#define AX_12A_H

#include "Arduino.h"
#include <HardwareSerial.h>


class AX_12A {
  private:
    byte id;
    HardwareSerial* serial;
    void writeRegister(byte reg, uint16_t value);

  public:
    AX_12A();
    void init (byte servoID, HardwareSerial* serialPort);
    void moveToPosition(uint16_t position);
    void setLED(bool state);
    void moveAtSpeed(bool direction, uint16_t speed, uint32_t duration);
};

#endif
