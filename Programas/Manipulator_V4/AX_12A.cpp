#include "AX_12A.h"

AX_12A::AX_12A() {
  id = 0;
  serial = NULL;
}

void AX_12A::init(byte servoID, HardwareSerial* serialPort) {
  id = servoID;
  serial = serialPort;

  writeRegister(0x06, 0);     // CW Angle Limit
  writeRegister(0x08, 0);     // CCW Angle Limit
  writeRegister(0x18, 1);     // Torque Enable
}

void AX_12A::moveToPosition(uint16_t position) {
  if (position > 1023) position = 1023;
  writeRegister(0x1E, position); 
}

void AX_12A::setLED(bool state) {
  writeRegister(0x19, state ? 1 : 0);
}

void AX_12A::moveAtSpeed(bool direction, uint16_t speed, uint32_t duration) {
  
  if (speed > 1023) speed = 1023;

  uint16_t speedVal = direction ? (speed | 0x400) : speed;

  writeRegister(0x20, speedVal); // Mover a velocidad y dirección
  delay(duration);               // Mantener el movimiento
  writeRegister(0x20, 0);        // Detener el servo
}

void AX_12A::writeRegister(byte reg, uint16_t value) {
  
  byte valueL = value & 0xFF;
  byte valueH = (value >> 8) & 0xFF;
  byte checksum = ~(id + 5 + 0x03 + reg + valueL + valueH);

  byte paquete[] = { 0xFF, 0xFF, id, 0x05, 0x03, reg, valueL, valueH, checksum };
  serial->write(paquete, 9);
  delay(10); // Pausa mínima entre comandos

}
