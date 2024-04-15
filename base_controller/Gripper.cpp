#include "Gripper.h"

Gripper::Gripper() {
    // Initialize servo motor
    myServoR.attach(servoRAnalogPin); 
    myServoL.attach(servoLAnalogPin); 
    releaseCube();
    myStepper = AccelStepper(1, Nema_Steep, Nema_Direction);
    pinMode(limitSwitchPin, INPUT);
    //digitalWrite(limitSwitchPin, 0);
    myStepper.setMaxSpeed(10000);
    myStepper.setAcceleration(5000);
    Nivel=0;
    
    // Assuming the first pin in the array is for the servo
// Assuming the first pin in the array is for the servo
}

void Gripper::downLevel(const uint8_t level) {
 
}

void Gripper::upLevel(const uint8_t level) {

    myStepper.move(100);
    if (level < 3){
        myStepper.moveTo(Paso*level-Ajuste);
        myStepper.runToPosition();
        myStepper.move(0);
      }
}

void Gripper::downSteps(int steps) {
}

void Gripper::upSteps(int steps) {

}

void Gripper::releaseCube() {
  
  myServoL.write(degreesToRelease); // Set servo L to desired position// Assuming 90 degrees is the closed position, adjust as needed
  myServoR.write(degreesToGrab); // Set servo L to desired position// Assuming 90 degrees is the closed position, adjust as needed

}

void Gripper::grabCube(){

  myServoR.write(degreesToRelease); // Set servo R to desired position
  myServoL.write(degreesToGrab); // Set servo L to desired position


}
void Gripper::StepperHome(){
    if(digitalRead(limitSwitchPin)==LOW){
        myStepper.move(-2000);
        myStepper.runToPosition();
        Ajuste=Ajuste+2000;
        //Serial.print(Ajuste);
    }else 
    {
        stop();
    }
    

}

void Gripper::stop() {
        // Stop the stepper motor
    pinMode(Nema_Direction,OUTPUT);
    digitalWrite(Nema_Direction,LOW);
    myStepper.setMaxSpeed(0);
  }
