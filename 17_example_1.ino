#include <Servo.h>

// Arduino pin assignment

#define PIN_POTENTIOMETER 3 // Potentiometer at Pin A3
#define PIN_SERVO 10
#define PIN_SENSOR 0
#define PIN_LED 9

#define _DUTY_MIN 553  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counter-clockwise position (180 degree)

#define _DIST_MIN 100.0
#define _DIST_MAX 250.0

#define LOOP_INTERVAL 50
#define _EMA_ALPHA 0.2

float dist_raw;
float dist_prev = _DIST_MIN;
float dist_ema;

Servo myservo;
unsigned long last_loop_time;

void setup()
{
  myservo.attach(PIN_SERVO);
  pinMode(PIN_LED, OUTPUT); 
  myservo.writeMicroseconds(_DUTY_NEU);

  Serial.begin(1000000);
}

void loop()
{
  unsigned long time_curr = millis();
  int a_value, duty;
  float deg;
  

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  a_value = analogRead(PIN_SENSOR);
  
  dist_raw = (6762.0/(a_value-9)-4.0)*10.0;
  
  dist_ema =  _EMA_ALPHA * dist_raw + (1 - _EMA_ALPHA) * dist_ema;

  deg = 1846/15 * (dist_prev - 10) + 553;

  if ( dist_raw < _DIST_MIN ){
  myservo.writeMicroseconds(553);
  dist_prev = dist_raw;
  digitalWrite(PIN_LED, 1); }
  
  else if ( _DIST_MAX < dist_raw ){
  myservo.writeMicroseconds(2399);
  dist_prev = dist_raw;
  digitalWrite(PIN_LED, 1);
  }
  
  else { 
  myservo.writeMicroseconds(deg); 
  digitalWrite(PIN_LED, 0);
  dist_prev = dist_raw;}
  
  
  // we need EMA filter here !!!
  dist_ema =  _EMA_ALPHA * dist_raw + (1 - _EMA_ALPHA) * dist_ema;
  
  // map distance into duty
  duty = 923.0 / 75.0 * dist_ema - 2033.0 / 3.0;
  myservo.writeMicroseconds(duty);

  // print IR sensor value, distnace, duty !!!
 Serial.print("MIN:");  Serial.print(_DIST_MIN);
 Serial.print(",IR:"); Serial.print("a_value");
 Serial.print(",dist:"); Serial.print(dist_raw);
 Serial.print(",ema:"); Serial.print(dist_ema);
 Serial.print(",servo:"); Serial.print(duty);
 Serial.print(",MAX:"); Serial.print(_DUTY_MAX);
 Serial.println("");
 
 
}
