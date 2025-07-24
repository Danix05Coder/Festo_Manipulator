#include "Manipulator_Shield.h"

MANIPULATOR_SHIELD festo_Manipulator;

String inputString = "";
bool stringComplete = false;

void setup() {

  Serial.begin(115200);

  festo_Manipulator.init();
  festo_Manipulator.set_XYZ_STEPmm(5, 5, 25);       // 20 20 100 para 1/4 paso.
  festo_Manipulator.set_XYZ_maxRPM(200, 200, 450);  // 200 rpm 200 rpm 450 rpm 
  festo_Manipulator.set_minSpeedPercentage(20);      // 20%
  inputString.reserve(50);
  
}

void loop() {
  if (stringComplete) {
    processGCode(inputString);
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void processGCode(String command) {
  command.trim(); // Elimina espacios y saltos de línea

  if (command.startsWith("G0 ")) {
    float x = NAN, y = NAN, z = NAN;

    if (command.indexOf('X') != -1) x = extractValue(command, 'X');
    if (command.indexOf('Y') != -1) y = extractValue(command, 'Y');
    if (command.indexOf('Z') != -1) z = extractValue(command, 'Z');

    if (!isnan(x) && isnan(y) && isnan(z)) {
      Serial.println("Moviendo eje X");
      festo_Manipulator.move_X(x);
    }
    else if (isnan(x) && !isnan(y) && isnan(z)) {
      Serial.println("Moviendo eje Y");
      festo_Manipulator.move_Y(y);
    }
    else if (isnan(x) && isnan(y) && !isnan(z)) {
      Serial.println("Moviendo eje Z");
      festo_Manipulator.move_Z(z);
    }
    else if (!isnan(x) || !isnan(y) || !isnan(z)) {
      if (isnan(x)) x = 0;
      if (isnan(y)) y = 0;
      if (isnan(z)) z = 0;
      Serial.println("Moviendo X Y Z simultáneamente");
      festo_Manipulator.move_XYZ(x, y, z);
    }

    Serial.println("ok");
  }
  else if (command.startsWith("G1 ")) {
    float x = extractValue(command, 'X');
    float y = extractValue(command, 'Y');
    float z = extractValue(command, 'Z');

    Serial.println("Moviendo X Y Z simultáneamente");
    festo_Manipulator.move_XYZ(x, y, z);
    Serial.println("ok");
  }
  else if (command.startsWith("G28")) {
    Serial.println("Moviendo a origen");
    festo_Manipulator.move_Home();
    Serial.println("ok");
  }
  else if (command.startsWith("M3 ")) {
    float gripperState = extractValue(command, 'S'); // Solo 0 o 1
    bool dir = (gripperState == 1); // True si S1, False si S0

    Serial.print("Moviendo gripper: ");
    Serial.println(dir ? "Abrir" : "Cerrar");

    // Usa valores fijos de velocidad y tiempo
    festo_Manipulator.move_Claw(dir, 200, 600);

    Serial.println("ok");
  }
  else {
    Serial.println("Comando no reconocido.");
  }
}

float extractValue(String command, char axis) {
  int index = command.indexOf(axis);
  if (index == -1) return NAN;

  int endIndex = command.indexOf(' ', index + 1);
  if (endIndex == -1) endIndex = command.length();

  String valueString = command.substring(index + 1, endIndex);
  return valueString.toFloat();
}
