//WHEN UPLOADING TO V5, MAKE SURE THIS FITS NEARSIDE OR FARSIDE
//EDIT VAR BELOW
// 1 = FARSIDE 
//-1 = NEARSIDE
  int invert = 1;

#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <list>

#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START V5 MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS


// Robot configuration code.
controller Controller1 = controller(primary);
digital_out wings = digital_out(Brain.ThreeWirePort.H);
bool holdit = true;
motor mR1 = motor(PORT3, ratio18_1, false);

motor mArmR = motor(PORT4, ratio18_1, true);

motor mR3 = motor(PORT6, ratio18_1, true);

motor mL3 = motor(PORT8, ratio18_1, false);

motor mL1 = motor(PORT9, ratio18_1, true);

motor mR2 = motor(PORT14, ratio18_1, false);

motor mL2 = motor(PORT18, ratio18_1, true);

motor mArmL = motor(PORT19, ratio18_1, false);

motor_group arm= motor_group(mArmL, mArmR);
motor_group mLeft = motor_group(mL1, mL2, mL3);
motor_group mRight = motor_group(mR1, mR2, mR3);
//inertial imu = inertial()
//smartdrive Drivetrain = smartdrive(mLeft, mRight, imu, 319.19, 255, 235, mm, .5*0.5714285714285714);
drivetrain Drivetrain = drivetrain(mLeft, mRight, 319.19, 255, 235, mm, .5*0.5714285714285714);

// Helper to make playing sounds from the V5 in VEXcode easier and
// keeps the code cleaner by making it clear what is happening.
void playVexcodeSound(const char *soundName) {
  printf("VEXPlaySound:%s\n", soundName);
  wait(5, msec);
}

// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true) {
    if(RemoteControlCodeEnabled) {
      
      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = Axis3 + Axis1
      // right = Axis3 - Axis1
      int drivetrainLeftSideSpeed = Controller1.Axis3.position() + Controller1.Axis1.position();
      int drivetrainRightSideSpeed = Controller1.Axis3.position() - Controller1.Axis1.position();
      
      // check if the value is inside of the deadband range
      if (drivetrainLeftSideSpeed < 5 && drivetrainLeftSideSpeed > -5) {
        // check if the left motor has already been stopped
        if (DrivetrainLNeedsToBeStopped_Controller1) {
          // stop the left drive motor
          mLeft.stop();
          // tell the code that the left motor has been stopped
          DrivetrainLNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the left motor nexttime the input is in the deadband range
        DrivetrainLNeedsToBeStopped_Controller1 = true;
      }
      // check if the value is inside of the deadband range
      if (drivetrainRightSideSpeed < 5 && drivetrainRightSideSpeed > -5) {
        // check if the right motor has already been stopped
        if (DrivetrainRNeedsToBeStopped_Controller1) {
          // stop the right drive motor
          mRight.stop();
          // tell the code that the right motor has been stopped
          DrivetrainRNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the right motor next time the input is in the deadband range
        DrivetrainRNeedsToBeStopped_Controller1 = true;
      }
      
      // only tell the left drive motor to spin if the values are not in the deadband range
      if (DrivetrainLNeedsToBeStopped_Controller1) {
        mLeft.setVelocity(drivetrainLeftSideSpeed, percent);
        mLeft.spin(forward);
      }
      // only tell the right drive motor to spin if the values are not in the deadband range
      if (DrivetrainRNeedsToBeStopped_Controller1) {
        mRight.setVelocity(drivetrainRightSideSpeed, percent);
        mRight.spin(forward);
      }
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}

task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);

#pragma endregion VEXcode Generated Robot Configuration


// ----------------------------------------------------------------------------
//                                                                            
//    Project:                                               
//    Author:
//    Created:
//    Configuration: VRC 2023-2024 Striker (Drivetrain 2-motor, No Gyro)
//                   Intake Motor in Port 8
//                   Arm Motor in Port 3
//                   Controller        
//                                                                            
// ----------------------------------------------------------------------------

// Include the V5 Library
#include "vex.h"

// Allows for easier use of the VEX Library
using namespace vex;

// Begin project code

void preAutonomous(void) {
  // actions to do when the program starts
  Brain.Screen.clearScreen();
  Brain.Screen.print("pre auton code");
  wait(1, seconds);

}

//passive arm for now. commented out all Arm motor stuff
void runArm() {
    //Arm.setPosition(Controller1.Axis2.position(),degrees);
}
void turnAround() {
  Drivetrain.turnFor(left, 180, degrees);
}

void stop_hammer_time(){//cannot think of variable names rn
    holdit = !holdit;
    if(holdit == true){arm.setStopping(hold);}
    else{arm.setStopping(coast);}
    arm.stop();}

void _90fwd(){
if(arm.isSpinning()==false){
arm.setPosition(fmod(arm.position(degrees),360),degrees);
}
arm.spinToPosition(arm.position(degrees)+90, degrees, false);
}

void _90rvs(){
if(arm.isSpinning()==false){
arm.setPosition(fmod(arm.position(degrees),360),degrees);
}
arm.spinToPosition(arm.position(degrees)-90, degrees, false);
}

void autonomous(void) {

  Brain.Screen.clearScreen();
  Brain.Screen.print("autonomous code");
  arm.setVelocity(5400,rpm);
  _90fwd();
  _90rvs();
  Drivetrain.setDriveVelocity(50, percent);
  Drivetrain.setTurnVelocity(50, percent);
  Drivetrain.driveFor(forward,12+3.24803,inches);
  Drivetrain.turnFor(right, invert*45, degrees);
  Drivetrain.driveFor(forward,22.869756222+3.24803,inches);
  Drivetrain.turnFor(left,invert*45,degrees);
  Drivetrain.driveFor(forward,15.461+3.24803,inches);
  _90fwd();
  _90rvs();
  //wait(1, seconds);
  //arm.stop();
  //Drivetrain.driveFor(reverse,4.059, inches);
  //if(invert == 1){
    Drivetrain.turnFor(right, 90,degrees);
    Drivetrain.driveFor(forward, 38.23+3.24803, inches);
    Drivetrain.turnFor(left, 90, degrees);
    Drivetrain.driveFor(forward, 6.925+3.24803, inches);
    //wings.set(true);
    Drivetrain.turnFor(right, 45, degrees);
    Drivetrain.turnFor(left, 45, degrees);
    //wings.set(false);
    Drivetrain.turnFor(right, 45, degrees);
    Drivetrain.driveFor(forward, 8.3572, inches);
    Drivetrain.turnFor(left, 34, degrees);
    Drivetrain.driveFor(forward, 5.371, inches);
    _90fwd();
    _90rvs();
    Drivetrain.turnFor(left, 56, degrees);
    //wings.set(true);
    Drivetrain.driveFor(forward, 27.46, inches);
    turnAround();
    Drivetrain.driveFor(forward, 6, inches);
    _90fwd();
    _90rvs();
    Drivetrain.driveFor(forward, 7, inches);
  //}
  //else{

  //}

}

void userControl(void) {
  Brain.Screen.clearScreen();
  // place driver control in this while loop
//set the drivetrain to stop here
Drivetrain.stop();
double armOut;
double canPress = true;
  while (true) {
    Drivetrain.setDriveVelocity(100, percent);
    Drivetrain.setTurnVelocity(100, percent);
    arm.setVelocity(100,percent);
    if (Controller1.ButtonL1.pressing()) {
      //wings.set(true);
    }
    else{
      //wings.set(false);
    }
    armOut=Controller1.Axis2.position()*0.12;
    //Arm.spin(forward,armOut,volt);
    if(Controller1.ButtonL1.pressing()){_90fwd();}//L1 and L2 turn 90 backwards/forwards 4 intake
    else if(Controller1.ButtonL2.pressing()){_90rvs();}
    else if(Controller1.ButtonR1.pressing()){arm.spin(forward,12,volt);}//R1 and R2 yeet it like the old program
    else if(Controller1.ButtonR2.pressing()){arm.spin(forward,-12,volt);}
    else if(Controller1.ButtonL2.pressing() && Controller1.ButtonL1.pressing() && canPress==true){
    stop_hammer_time();//swap between coast and hold stopping patterns
    arm.stop();
    canPress=false;
    }
    else{arm.stop(); canPress=true;}
    Controller1.ButtonA.pressed(turnAround);
    mLeft.spin(forward,Controller1.Axis3.position()*.12+Controller1.Axis1.position()*.75*.12, volt);
    mRight.spin(forward,Controller1.Axis3.position()*.12-Controller1.Axis1.position()*.75*.12, volt);
    //one-stick drive
    //mLeft.spin(forward,Controller1.Axis3.position()*.12+Controller4.Axis1.position()*.75*.12, volt);
    //mRight.spin(forward,Controller1.Axis3.position()*.12-Controller4.Axis1.position()*.75*.12, volt);
  }
}
    
  
  



int main() {
  // create competition instance
  competition Competition;

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(userControl);

  // Run the pre-autonomous function.
  preAutonomous();

  // Prevent main from exiting with an infinite loop.
  
}
