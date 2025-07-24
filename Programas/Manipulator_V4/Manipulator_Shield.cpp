#include "Manipulator_Shield.h"

HardwareSerial SerialAX(2);

MANIPULATOR_SHIELD::MANIPULATOR_SHIELD() {
  speed_rpm = (double *)calloc(3, sizeof(double));
  limitX = false;
  limitY = false;
  limitZ = false;
  minSpeedPercentage = 30.0; // Valor por defecto del 20%
}

void MANIPULATOR_SHIELD::set_minSpeedPercentage(double percent) {
  if (percent < 0) percent = 0;
  if (percent > 100) percent = 100;
  minSpeedPercentage = percent;
}

void MANIPULATOR_SHIELD::init() {

  SerialAX.begin(BAUD_RATE, SERIAL_8N1, -1, TX_PIN);
  Claw.init(1, &SerialAX);
    
  pinMode(Y_STEP_PIN0 ,  OUTPUT);
  pinMode(Y_DIR_PIN0  ,  OUTPUT);
  pinMode(Y_STEP_PIN1 ,  OUTPUT);
  pinMode(Y_DIR_PIN1  ,  OUTPUT);
  pinMode(X_STEP_PIN  ,  OUTPUT);
  pinMode(X_DIR_PIN   ,  OUTPUT);
  pinMode(Z_STEP_PIN  ,  OUTPUT);
  pinMode(Z_DIR_PIN   ,  OUTPUT);
  pinMode(ENABLE_PIN  ,  OUTPUT);
  
  pinMode(PIN_FRX, INPUT_PULLUP);
  pinMode(PIN_FRY, INPUT_PULLUP);
  pinMode(PIN_FRZ, INPUT_PULLUP);

  
  digitalWrite(ENABLE_PIN, LOW);

  digitalWrite(Y_STEP_PIN0 ,   LOW);
  digitalWrite(Y_DIR_PIN0  ,   LOW);
  digitalWrite(Y_STEP_PIN1 ,   LOW);
  digitalWrite(Y_DIR_PIN1  ,   LOW);
  digitalWrite(X_STEP_PIN  ,   LOW);
  digitalWrite(X_DIR_PIN   ,   LOW);
  digitalWrite(Z_STEP_PIN  ,   LOW);
  digitalWrite(Z_DIR_PIN   ,   LOW);


  xTaskCreate(
    limitSwitchMonitor,
    "LimitSwitchMonitor",
    2048,
    this, // Pasas el objeto
    1,
    NULL
  );

}

void MANIPULATOR_SHIELD::set_XYZ_STEPmm(double Xstp_mm, double Ystp_mm, double Zstp_mm) {
  X_STPmm = Xstp_mm;
  Y_STPmm = Ystp_mm;
  Z_STPmm = Zstp_mm;
}

void MANIPULATOR_SHIELD::set_XYZ_maxRPM(double _XmaxRPM, double _YmaxRPM, double _ZmaxRPM) {
  XmaxRPM = _XmaxRPM;
  YmaxRPM = _YmaxRPM;
  ZmaxRPM = _ZmaxRPM;
}

bool MANIPULATOR_SHIELD::get_FRX   (){
  return limitX;
}

bool MANIPULATOR_SHIELD::get_FRY   (){
  return limitY;
}

bool MANIPULATOR_SHIELD::get_FRZ   (){
  return limitZ;
}

void MANIPULATOR_SHIELD::move_X(double mm) {
  
  speed_rpm[0] = 0;

  bool startPulse = true;
  unsigned long currentTime = 0;
  unsigned long lastTime = 0;
  unsigned long stepDelay = 0;

  long stepsTarget = abs(mm * X_STPmm) + 1;
  long stepsDone = 1;

  double total_time = 0;

  digitalWrite(X_DIR_PIN, (mm > 0) ? LOW : HIGH);

  profile[0].set_final_position(stepsTarget);
  profile[0].set_max_speed(XmaxRPM);
  total_time = profile[0].get_final_time();

  lastTime = micros();

  while (stepsDone < stepsTarget) {
    
    if (limitX && mm < 0) return;

    currentTime = micros();

    if (currentTime - lastTime >= stepDelay) {
      lastTime = currentTime;

      if (startPulse) {
        digitalWrite(X_STEP_PIN, HIGH);
        startPulse = false;
      } else {
        digitalWrite(X_STEP_PIN, LOW);
        startPulse = true;
        stepsDone++;

        double t = total_time * ((double)stepsDone / (double)stepsTarget);
        speed_rpm[0] = profile[0].compute_speed(t);

        double minRPM = (minSpeedPercentage / 100.0) * XmaxRPM;
        if (speed_rpm[0] < minRPM) speed_rpm[0] = minRPM;

        stepDelay = 150000 / speed_rpm[0];
      }
    }
  }
}

void MANIPULATOR_SHIELD::move_Y(double mm) {

  limitY = false;
  
  bool startPulse = true;
  unsigned long currentTime = 0;
  unsigned long lastTime = 0;
  unsigned long stepDelay = 0;

  long stepsTarget = abs(mm * Y_STPmm) + 1;
  long stepsDone = 1;

  double total_time = 0;

  digitalWrite(Y_DIR_PIN0, (mm > 0) ? HIGH : LOW);
  digitalWrite(Y_DIR_PIN1, (mm > 0) ? LOW : HIGH);

  profile[1].set_final_position(stepsTarget);
  profile[1].set_max_speed(YmaxRPM);
  total_time = profile[1].get_final_time();

  lastTime = micros();

  while (stepsDone < stepsTarget) {
   
    if (limitY && mm < 0 ) return;
 
    currentTime = micros();

    if (currentTime - lastTime >= stepDelay) {
      lastTime = currentTime;

      if (startPulse) {
        digitalWrite(Y_STEP_PIN0, HIGH);
        digitalWrite(Y_STEP_PIN1, HIGH);
        startPulse = false;
      } else {
        digitalWrite(Y_STEP_PIN0, LOW);
        digitalWrite(Y_STEP_PIN1, LOW);
        startPulse = true;
        stepsDone++;

        double t = total_time * ((double)stepsDone / (double)stepsTarget);
        speed_rpm[1] = profile[1].compute_speed(t);

        double minRPM = (minSpeedPercentage / 100.0) * YmaxRPM;
        if (speed_rpm[1] < minRPM) speed_rpm[1] = minRPM;

        stepDelay = 150000 / speed_rpm[1];
      }
    }
  }
}

void MANIPULATOR_SHIELD::move_Z(double mm) {

  limitZ = false;
  
  
  bool startPulse = true;
  unsigned long currentTime = 0;
  unsigned long lastTime = 0;
  unsigned long stepDelay = 0;

  long stepsTarget = abs(mm * Z_STPmm) + 1;
  long stepsDone = 1;

  double total_time = 0;

  digitalWrite(Z_DIR_PIN, (mm > 0) ? HIGH : LOW);

  profile[2].set_final_position(stepsTarget);
  profile[2].set_max_speed(ZmaxRPM);
  total_time = profile[2].get_final_time();

  lastTime = micros();

  while (stepsDone < stepsTarget) {
    
    if (limitZ && mm < 0) return;

    currentTime = micros();

    if (currentTime - lastTime >= stepDelay) {
      lastTime = currentTime;

      if (startPulse) {
        digitalWrite(Z_STEP_PIN, HIGH);
        startPulse = false;
      } else {
        digitalWrite(Z_STEP_PIN, LOW);
        startPulse = true;
        stepsDone++;

        double t = total_time * ((double)stepsDone / (double)stepsTarget);
        speed_rpm[2] = profile[2].compute_speed(t);

        double minRPM = (minSpeedPercentage / 100.0) * ZmaxRPM;
        if (speed_rpm[2] < minRPM) speed_rpm[2] = minRPM;

        stepDelay = 150000 / speed_rpm[2];
      }
    }
  }
}

void MANIPULATOR_SHIELD::move_XYZ(double Xmm, double Ymm, double Zmm) {
  
  bool startPulse[] = {true, true, true};
  unsigned long currentTime = 0;
  unsigned long lastTime[] = {0, 0, 0};
  unsigned long stepDelay[] = {0, 0, 0};

  long stepsTarget[] = {0, 0, 0};
  long stepsDone[] = {1, 1, 1};

  double total_time[] = {0, 0, 0};

  bool axisActive[] = {true, true, true}; // 🔹 Ejes activos (se detendrán individualmente)

  digitalWrite(Y_DIR_PIN0, (Xmm > 0) ? HIGH : LOW);
  digitalWrite(Y_DIR_PIN1, (Xmm > 0) ? LOW : HIGH);
  digitalWrite(X_DIR_PIN, (Ymm > 0) ? LOW : HIGH);
  digitalWrite(Z_DIR_PIN, (Zmm > 0) ? HIGH : LOW);

  stepsTarget[0] = abs(Xmm * X_STPmm) + 1;
  stepsTarget[1] = abs(Ymm * Y_STPmm) + 1;
  stepsTarget[2] = abs(Zmm * Z_STPmm) + 1;

  profile[0].set_final_position(stepsTarget[0]);
  profile[0].set_max_speed(XmaxRPM);
  total_time[0] = profile[0].get_final_time();

  profile[1].set_final_position(stepsTarget[1]);
  profile[1].set_max_speed(YmaxRPM);
  total_time[1] = profile[1].get_final_time();

  profile[2].set_final_position(stepsTarget[2]);
  profile[2].set_max_speed(ZmaxRPM);
  total_time[2] = profile[2].get_final_time();

  unsigned long t = micros();
  lastTime[0] = t;
  lastTime[1] = t;
  lastTime[2] = t;

  while (axisActive[0] || axisActive[1] || axisActive[2]) { // 🔹 Mientras al menos un eje siga activo

    currentTime = micros();

    for (int i = 0; i < 3; i++) {

      if (!axisActive[i]) continue; // 🔹 Saltar ejes que ya se detuvieron

      // 🔹 Verificar final de carrera por eje
      if ((i == 0 && limitX && Xmm < 0) ||
          (i == 1 && limitY && Ymm < 0) ||
          (i == 2 && limitZ && Zmm < 0)) {
        axisActive[i] = false; // Detener solo este eje
        continue;
      }

      if (stepsDone[i] >= stepsTarget[i]) {
        axisActive[i] = false; // Ya terminó su recorrido
        continue;
      }

      if (currentTime - lastTime[i] >= stepDelay[i]) {

        lastTime[i] = currentTime;

        if (startPulse[i]) {
          if (i == 0) digitalWrite(X_STEP_PIN, HIGH);
          if (i == 1) {
            digitalWrite(Y_STEP_PIN0, HIGH);
            digitalWrite(Y_STEP_PIN1, HIGH);
          }
          if (i == 2) digitalWrite(Z_STEP_PIN, HIGH);

          startPulse[i] = false;

        } else {
          if (i == 0) digitalWrite(X_STEP_PIN, LOW);
          if (i == 1) {
            digitalWrite(Y_STEP_PIN0, LOW);
            digitalWrite(Y_STEP_PIN1, LOW);
          }
          if (i == 2) digitalWrite(Z_STEP_PIN, LOW);

          startPulse[i] = true;
          stepsDone[i]++;

          double t = total_time[i] * ((double)stepsDone[i] / (double)stepsTarget[i]);
          speed_rpm[i] = profile[i].compute_speed(t);

          double maxRPM = (i == 0) ? XmaxRPM : (i == 1) ? YmaxRPM : ZmaxRPM;
          double minRPM = (minSpeedPercentage / 100.0) * maxRPM;
          if (speed_rpm[i] < minRPM) speed_rpm[i] = minRPM;

          stepDelay[i] = 150000 / speed_rpm[i];
        }
      }
    }
  }
}


void MANIPULATOR_SHIELD::move_Home() {
  move_XYZ(-400, -400, -400);
}


void MANIPULATOR_SHIELD::move_Claw (bool dir, uint16_t vel, long _t){
  
  Claw.setLED(true);
  Claw.moveAtSpeed   ( dir , vel, _t );
  Claw.setLED(false);
  
}

void MANIPULATOR_SHIELD::limitSwitchMonitor(void *parameter) {
  MANIPULATOR_SHIELD *self = (MANIPULATOR_SHIELD *)parameter; // Apuntador al objeto

  const int sampleTime = 1;
  const int debounceCount = 5;

  int countX = 0, countY = 0, countZ = 0;

  bool lastStateX = digitalRead(PIN_FRX);
  bool lastStateY = digitalRead(PIN_FRY);
  bool lastStateZ = digitalRead(PIN_FRZ);

  while (true) {
    bool currentX = digitalRead(PIN_FRX);
    bool currentY = digitalRead(PIN_FRY);
    bool currentZ = digitalRead(PIN_FRZ);

    // Eje X
    if (currentX != lastStateX) {
      countX++;
      if (countX >= debounceCount) {
        lastStateX = currentX;
        countX = 0;
        self->limitX = (currentX == LOW);
        if (currentX == LOW) Serial.println("FinX");
      }
    } else {
      countX = 0;
    }

    // Eje Y
    if (currentY != lastStateY) {
      countY++;
      if (countY >= debounceCount) {
        lastStateY = currentY;
        countY = 0;
        self->limitY = (currentY == LOW);
        if (currentY == LOW) Serial.println("FinY");
      }
    } else {
      countY = 0;
    }

    // Eje Z
    if (currentZ != lastStateZ) {
      countZ++;
      if (countZ >= debounceCount) {
        lastStateZ = currentZ;
        countZ = 0;
        self->limitZ = (currentZ == LOW);
        if (currentZ == LOW) Serial.println("FinZ");
      }
    } else {
      countZ = 0;
    }

    vTaskDelay(pdMS_TO_TICKS(sampleTime));
  }
}
