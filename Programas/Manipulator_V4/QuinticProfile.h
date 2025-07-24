#ifndef QUINTIC_PROFILE_H
#define QUINTIC_PROFILE_H

#include "Arduino.h"
#include <math.h>

class QuinticProfile {
  public:
    QuinticProfile();
    QuinticProfile( double _max_vel , double _pf  );
    void   set_final_position   ( double _pf      );
    void   set_max_speed        ( double _max_speed );
    double get_final_position   ();
    double get_max_speed        ();
    double get_final_time       ();
    double compute_position     ( double t );
    double compute_speed        ( double t );
    double compute_acceleration ( double t );
  private:
    double a;
    double b;
    double c;
    double max_speed;
    double pf; 
    double tf;
};

#endif
