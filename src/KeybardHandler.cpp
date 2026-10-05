#include "KeyboardHandler.h"


void KeyboardHandler::processInput(int key, int action) {
  if (action==GLFW_PRESS) keyStates[key]=PRESSED;
  if (action==GLFW_RELEASE) keyStates[key]=RELEASED;
}

void KeyboardHandler::updateInput() {
  for (auto&[key,state]: keyStates) {
    if (state==PRESSED) state=HOLD;
    if (state==RELEASED) state=IDLE;
  }

}



