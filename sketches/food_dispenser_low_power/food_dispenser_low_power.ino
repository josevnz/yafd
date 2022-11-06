/*
Sketch to control the motor that open/ closes the cap that lets the food drop on the dispenser.
References: 
* https://www.arduino.cc/reference/en/
* https://create.arduino.cc/projecthub/knackminds/how-to-measure-distance-using-ultrasonic-sensor-hc-sr04-a-b9f7f8

Modules:
- HC-SR04: Ultrasonic sensor distance module
- SG90 9g Micro Servos: Opens / closes lid on the food dispenser
Author: Jose Vicente Nunez (kodegeek.com@protonmail.com)
*/
#include "LowPower.h"
#include <Servo.h>
Servo servo;

unsigned int const DEBUG = 1;

/*
Pin choice is arbitrary.
 */ 
const unsigned int HC_SR04_TRIGGER_PIN = 2; // Send the ultrasound ping
const unsigned int HC_SR04_ECHO_PIN = 3; // Receive the ultrasound response
const unsigned int SG90_SERVO_PIN = 9;  // Activate the servo to open/ close lid

const unsigned int MEASUREMENTS = 3;
const unsigned int DELAY_BETWEEN_MEASUREMENTS_MILIS = 50;

const unsigned long ONE_MILISECOND = 1;
const unsigned long ONE_SECOND = 1000;
const unsigned long FIVE_SECONDS = 3000;

const unsigned long MIN_DISTANCE_IN_CM = 35; // Between 2cm - 500cm

const unsigned int OPEN_CAP_ROTATION_IN_DEGRESS = 90; // Between 0 - 180
const unsigned int CLOSE_CAP_ROTATION_IN_DEGRESS = 0;

const unsigned int CLOSE = 0;

/*
Speed of Sound: 340m/s = 29microseconds/cm
Sound wave reflects from the obstacle, so to calculate the distance we consider half of the distance traveled.  
DistanceInCms=microseconds/29/2 
*/
long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

unsigned long measure() {
  /*
  Send the ultrasound ping
  */
  digitalWrite(HC_SR04_TRIGGER_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(HC_SR04_TRIGGER_PIN, HIGH);
  delayMicroseconds(15);
  digitalWrite(HC_SR04_TRIGGER_PIN, LOW);

  /*
  Receive the ultrasound ping and convert to distance
  */
  unsigned long pulse_duration_ms = pulseIn(HC_SR04_ECHO_PIN, HIGH);
  return microsecondsToCentimeters(pulse_duration_ms);
}


/*
- Close cap on power on startup
- Set servo, and read/ write pins
 */
void setup() {
  pinMode(HC_SR04_TRIGGER_PIN, OUTPUT);
  pinMode(HC_SR04_ECHO_PIN, INPUT);
  servo.attach(SG90_SERVO_PIN);
  servo.write(CLOSE_CAP_ROTATION_IN_DEGRESS);
  delay(ONE_SECOND);
  servo.detach();
    if (DEBUG) {
    Serial.begin(9600);
  }
}

void loop() {

  float dist = 0;
  for (int i = 0; i < MEASUREMENTS; i++) {  // Average distance
    dist += measure();
    delay(DELAY_BETWEEN_MEASUREMENTS_MILIS);  //delay between measurements
  }
  float avg_dist_cm = dist / MEASUREMENTS;

  /*
  If average distance is less than threshold then keep the door open for 5 seconds 
  to let enough food out, then close it.
  */
  if (avg_dist_cm < MIN_DISTANCE_IN_CM) {
    servo.attach(SG90_SERVO_PIN);
    delay(ONE_MILISECOND);
    servo.write(OPEN_CAP_ROTATION_IN_DEGRESS);
    delay(FIVE_SECONDS);
    servo.write(CLOSE_CAP_ROTATION_IN_DEGRESS);
    delay(ONE_SECOND);
    servo.detach();
    // Pet is eating and in front of the dispenser, we can definitely sleep longer
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  } else {
    LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
  }

  if (DEBUG) {
    Serial.print(avg_dist_cm);
    Serial.print(" cm");
    Serial.println();
  }

}