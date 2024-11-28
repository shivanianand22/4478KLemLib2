#ifndef ACTIONS_H
#define ACTIONS_H
#include "pros/rtos.hpp"
extern void intakeDist();
extern void intakeColor();
extern void grab();
extern void release();
void macro(void* param); 

// Task declaration
extern pros::Task armMacro;

#endif