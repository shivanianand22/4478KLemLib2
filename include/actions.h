#ifndef ACTIONS_H
#define ACTIONS_H
 extern double armTargets[];
 extern double target;
 extern bool blueAllaince;
 extern bool loadLB;
 extern bool primed;
 void intakeDist();
 void intakeColor(void* param);
 void grab();
 void moveArm(void* param);
 void release();
#endif