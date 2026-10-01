#include "KeyboardHandler.h"
void KeyboardHandler::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

  KeyboardHandler* instance =
    static_cast<KeyboardHandler*>(glfwGetWindowUserPointer(window));

  // 4. Forward the call to the non-static member
  if (instance) { instance->processInput(key,action); }
}

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



