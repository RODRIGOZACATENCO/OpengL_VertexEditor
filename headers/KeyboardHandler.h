#pragma once
#include <unordered_map>
#include <map>
#include <vector>
#include <GLFW/glfw3.h>

enum Actions {
  ROTATE_LEFT,//GIMBALL
  ROTATE_RIGHT,
  MOVE_UP,
  MOVE_DOWN,

  MOVE_FORWARD,//FREE MODE
  MOVE_BACKWARDS,
  MOVE_LEFT,
  MOVE_RIGHT
};

class KeyboardHandler {

private:
  enum KeyState {
    IDLE,
    PRESS,
    HOLD,
    RELEASE

  };
  std::unordered_map<Actions, std::vector<int>> keyBindings;
  std::map<int,KeyState> KeyStates;


public:
  static void KeyCallback(GLFWwindow* window, int key, int scancode,
                                    int action, int mods);

  void processInput(int key, int action);

  void defineKeybind(int key,Actions action) {

  }

};
