#include "QuinticProfile.h"


QuinticProfile::QuinticProfile(){
  a=0,b=0,c=0;
  max_speed=0,pf=0,tf=0;
}
QuinticProfile::QuinticProfile( double _max_speed , double _pf ):
  max_speed(_max_speed), pf(_pf){
  a  = 65536.0 * pow(max_speed, 5) / (253125.0 * pow(pf, 4));
  b  = -4096.0 * pow(max_speed, 4) / (  3375.0 * pow(pf, 3));
  c  =  1024.0 * pow(max_speed, 3) / (   675.0 * pow(pf, 2));
  tf =  15*pf/(8*max_speed);
}

void   QuinticProfile::set_final_position( double _pf ){
  pf = _pf;
  a  = 65536.0 * pow(max_speed, 5) / (253125.0 * pow(pf, 4));
  b  = -4096.0 * pow(max_speed, 4) / (  3375.0 * pow(pf, 3));
  c  =  1024.0 * pow(max_speed, 3) / (   675.0 * pow(pf, 2));
  tf =  15*pf/(8*max_speed);
}

void   QuinticProfile::set_max_speed( double _max_speed ){
  max_speed = _max_speed;
  a  = 65536.0 * pow(max_speed, 5) / (253125.0 * pow(pf, 4));
  b  = -4096.0 * pow(max_speed, 4) / (  3375.0 * pow(pf, 3));
  c  =  1024.0 * pow(max_speed, 3) / (   675.0 * pow(pf, 2));
  tf =  15*pf/(8*max_speed);
}

double QuinticProfile::get_final_position(){
  return pf;
}

double QuinticProfile::get_max_speed(){
  return max_speed;
}

double QuinticProfile::get_final_time(){
   return tf;
}
 
double QuinticProfile::compute_position     ( double t){
  return a*pow(t,5) + b*pow(t,4) + c*pow(t,3);
}

double QuinticProfile::compute_speed        ( double t){
   return 5*a*pow(t,4) + 4*b*pow(t,3) + 3*c*pow(t,2);
}

double QuinticProfile::compute_acceleration ( double t){
   return 20*a*pow(t,3) + 12*b*pow(t,2) + 6*c*pow(t,1);
}
