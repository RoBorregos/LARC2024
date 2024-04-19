
#include "Movement.h"
#include "Raspy.h"
#include "Encoder.h"

Movement *robot = nullptr;
BNO *bnoInstance = nullptr;
LineSensor *myLineSensor = nullptr;
ColorSensor *myColorSensor = nullptr;
Gripper *myGripper = nullptr;
Raspy raspy;

bool CHECK_PID = false;
bool CHECK_ODOMETRY = false;
bool CHECK_LINES = true;
bool CHECK_GRASP = false;

/////////////////////////////////////remove after testing///////////////////////////////////////////////////  
unsigned long curr_millis = 0;
unsigned long prev_millis = 0;
int iteration = 0;
double angleOffset = 0.0; //for tests
double squares = 0; 
float angleAmount = 0.0;  //for state machne
uint8_t start_pos_x = 0; //for tests 
Direction movementVector[5] = {FORWARD, TOLEFT, BACKWARD, TORIGHT, STOP};

bool reachedAngle;
int prev_pos_x = 0;
bool fullScanCompleted = false;
bool startingScanFrom0 = false;
bool startingScanFrom6 = false;
bool fromOtherSide = false;
bool ready = true;

///////////////////////////////////// //////////////////////////////////////////////////////////////////////////////////////


void moveForward(Movement *robot) {
    robot->orientedMovement(0.0, 0.35, 0.0);
    //serial.println("Moving Forward");
}

void moveLeft(Movement *robot) {
    robot->orientedMovement(0.35, 0.0, 0.0);
    //serial.println("Moving Left");
}
 
void moveRight(Movement *robot) { 
    robot->orientedMovement(0.0, -0.35, 0.0);
    //serial.println("Moving Right");
}

void moveBackward(Movement *robot) {
    robot->orientedMovement(-0.35, 0.0, 0.0);
    //serial.println("Moving Backwards");
}

enum States {
    TESTS,
    FIND_ORIGIN,
    FIND_EMPTY_PATH,
    DRIVE_TO_COLOR,
    ROTATE_180,
    SEARCH_CUBE,
    DRIVE_TO_CUBE,
    GRAB_CUBE,
    ENTER_CLOSEST_SQUARE,
    ROTATE_SEARCH_COLOR,
    RELEASE_CUBE,
    DEFAULT_STATE
};

States currentState;

void setup() {
    //Wire.begin();
    Serial.begin(115200);
    bnoInstance = new BNO(); 
    myLineSensor = new LineSensor();
    myColorSensor = new ColorSensor();
    myGripper = new Gripper(); 
    robot = new Movement(bnoInstance, myLineSensor, myColorSensor); 
    robot->initEncoders();
    // robot->setGlobalPosX(start_pos_x);
    robot->angleOffsetReached = false;
    //robot->setSquareCounter(0);
    //myGripper->StepperHome();

    raspy.import(robot);
//     //serial.print("Starting");
    
        //currentState = TESTS;
//     //currentState = ENTER_CLOSEST_SQUARE;
        //currentState = FIND_ORIGIN;
}

void loop() {
    raspy.readSerial();
    if(ready){
        myGripper->StepperHome();
        myGripper->upLevel(5);
        ready = false;
     }

   
   if(raspy.update){
      currentState = raspy.get_State();
      raspy.update = false;
   }

    curr_millis = millis();

    if (currentState == TESTS) {
        tests();
    } else if (currentState == FIND_ORIGIN) {
        findOrigin();
    } else if (currentState == FIND_EMPTY_PATH) {
        findEmptyPath();
    } else if (currentState == DRIVE_TO_COLOR) {
        driveToColor();
    } else if (currentState == ROTATE_180) {
        rotate_180();
    } else if (currentState == SEARCH_CUBE) {
        searchCube();
    } else if (currentState == DRIVE_TO_CUBE) {
        driveToCube();
    } else if (currentState == GRAB_CUBE) {
        grabCube();
    } else if (currentState == ENTER_CLOSEST_SQUARE) {
        enterClosestSquare();
    } else if (currentState == ROTATE_SEARCH_COLOR) {
        goToPosition();
    } else if (currentState == RELEASE_CUBE) {
        releaseCube();
    } else {
        // Handle the default case
        // In case nothing is received from raspy
    }

    // Serial.println(currentState);
    // Serial.print("Global Pos X: "); Serial.println(robot->getCurrentPosX());
    // Serial.print("Global Angle: "); Serial.print(robot->getRobotAngle());
}
