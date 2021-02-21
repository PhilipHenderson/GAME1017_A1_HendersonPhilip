#include "Engine.h"
#include "StateManager.h"
#include "States.h"

#include "Utilities.h"
#include <iostream>
#include <ctime>
#define WIDTH 1024
#define HEIGHT 768
#define FPS 60
#define BGSCROLL 2 // Could these scroll/speed values be handled in the class? Yes. Consider it!
#define PSPEED 6
using namespace std;



/* Update is SUPER way too long on purpose! Part of the Assignment 1, if you use
   this program as a start project is to chop up Update and figure out where each
   part of the code is supposed to go. A practice in OOP is to have objects handle
   their own behaviour and this is a big hint for you. */


