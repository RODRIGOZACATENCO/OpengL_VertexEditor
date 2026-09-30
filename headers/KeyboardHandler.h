#pragma once
#include <unordered_map>
#include <map>
#include <vector>
#include <GLFW/glfw3.h>
#include "JsonParser.h"
enum Actions {
  ROTATE_LEFT,//GIMBALL
  ROTATE_RIGHT,
  MOVE_UP,
  MOVE_DOWN,

  MOVE_FORWARD,//FREE MODE
  MOVE_BACKWARDS,
  MOVE_LEFT,
  MOVE_RIGHT,

  TOGGLE_MENU//MENU
};

inline std::unordered_map<std::string,Actions>string_to_action{
{"ROTATE_LEFT", ROTATE_LEFT},
{"ROTATE_RIGHT", ROTATE_RIGHT},
{"MOVE_UP", MOVE_UP},
{"MOVE_DOWN", MOVE_DOWN},
{"MOVE_FORWARD", MOVE_FORWARD},
{"MOVE_BACKWARDS", MOVE_BACKWARDS},
{"MOVE_LEFT", MOVE_LEFT},
{"MOVE_RIGHT", MOVE_RIGHT},
{"OPEN_MENU",TOGGLE_MENU},
};

inline std::unordered_map<std::string,int> str_to_key_binding{
{"LEFT",GLFW_KEY_LEFT},
{"RIGHT",GLFW_KEY_RIGHT},
{"UP",GLFW_KEY_UP},
{"DOWN",GLFW_KEY_DOWN},
  {"TAB",GLFW_KEY_TAB}


};

inline std::unordered_map<Actions,int> action_to_key;
class KeyboardHandler {

private:
  enum KeyState {
    IDLE,
    PRESSED,
    HOLD,
    RELEASED

  };
  std::unordered_map<Actions, std::vector<int>> keyBindings;
  std::map<int,KeyState> keyStates;


public:
  KeyboardHandler() {
    JsonParser jsonparser;
    JsonValue settings=jsonparser.parseFile("../settings/key_bindings.json");
    for (auto [action,key]: settings.as_object()) {
      for (auto key_name : key.as_array()) {
        keyBindings[string_to_action[action]].push_back(str_to_key_binding[key_name]);
      }

    }
  };
  static void KeyCallback(GLFWwindow* window, int key, int scancode,
                                    int action, int mods);

  void processInput(int key, int action);

  void updateInput();

};
