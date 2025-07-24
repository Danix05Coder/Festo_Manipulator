#ifndef MANIPULATOR_SHIELD_H
#define MANIPULATOR_SHIELD_H

#include "Arduino.h"
#include "QuinticProfile.h"
#include "AX_12A.h"

// Configuración Claw
extern HardwareSerial SerialAX; // SOLO DECLARAR AQUÍ
#define BAUD_RATE 1000000
#define TX_PIN     17

// Pines Motores a Pasos
#define Y_STEP_PIN0   32 
#define Y_DIR_PIN0    33 
#define Y_STEP_PIN1   25
#define Y_DIR_PIN1    26
#define X_STEP_PIN    19 
#define X_DIR_PIN     21
#define Z_STEP_PIN    22
#define Z_DIR_PIN     23
#define ENABLE_PIN    27

// Finales de Carrera
#define PIN_FRX 39
#define PIN_FRY 34
#define PIN_FRZ 35


///X Inverso y Y rotado  

class MANIPULATOR_SHIELD {
  public:
    MANIPULATOR_SHIELD();
    void init();
    void set_XYZ_STEPmm  ( double  Xstp_mm, double  Ystp_mm, double  Zstp_mm );
    void set_XYZ_maxRPM  ( double _XmaxRPM, double _YmaxRPM, double _ZmaxRPM ); 
    void set_minSpeedPercentage (double percent);
    bool get_FRX   ( );
    bool get_FRY   ( );
    bool get_FRZ   ( ); 
    void move_X    ( double mm  );
    void move_Y    ( double mm  );
    void move_Z    ( double mm  );
    void move_XYZ  ( double Xmm, double Ymm, double Zmm );
    void move_Home ( );
    void move_Claw ( bool dir , uint16_t vel, long _t );
    static void limitSwitchMonitor(void *parameter);
         
  private:
    AX_12A Claw;
    QuinticProfile profile[3];
    double *speed_rpm;
    double  minSpeedPercentage;
    double  XmaxRPM;
    double  YmaxRPM;
    double  ZmaxRPM;
    double  X_STPmm;
    double  Y_STPmm;
    double  Z_STPmm;
    bool    limitX;
    bool    limitY;
    bool    limitZ;
};

#endif
