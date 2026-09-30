#include "KeyboardHandler.h"
void KeyboardHandler::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

  KeyboardHandler* instance =
    static_cast<KeyboardHandler*>(glfwGetWindowUserPointer(window));

  // 4. Forward the call to the non-static member
  if (instance) { instance->processInput(key,action); }
}

void KeyboardHandler::processInput(int key, int action) {

}


