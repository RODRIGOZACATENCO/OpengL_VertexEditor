#pragma once
#include <unordered_map>
#include <map>
#include <vector>
#include <GLFW/glfw3.h>
#include "JsonParser.h"
#include "CommonTypes.h"
#include <iostream>
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


class KeyboardHandler {

private:
  std::unordered_map<Actions,int> action_to_key;
  std::unordered_map<Actions, std::vector<int>> keyBindings;
  std::map<int,KeyState> keyStates;


public:
  KeyboardHandler() {
    JsonParser jsonparser;
    JsonValue settings=jsonparser.parseFile("../settings/key_bindings.json");
    for (auto [action,key]: settings["key_bindings"].as_object()) {
      for (auto key_name : key.as_array()) {
        keyBindings[string_to_action[action]].push_back(str_to_key_binding[key_name.as_string()]);
      }
    }
    for(int i=0;i<=1024;i++) keyStates[i]=IDLE;
  };
  static void KeyCallback(GLFWwindow* window, int key, int scancode,
                                    int action, int mods);

  void processInput(int key, int action);

  void updateInput();
  std::map<int,KeyState> *getKeys() {
    return &keyStates;
  }
  bool checkAction(Actions action,KeyState state) {
    for(auto key_binding:keyBindings[action]) {
        if(keyStates[key_binding]==state) return true;
    }
    return false;
  }

  bool checkActionIsActive(Actions action) {
    for(auto key_binding:keyBindings[action]) {
      if(keyStates[key_binding]==PRESSED || keyStates[key_binding]==HOLD) return true;
    }
    return false;
  }
  bool isKeyJustPressed(int key){return keyStates[key]==PRESSED;}
  bool isKeyHeld(int key){return keyStates[key]==HOLD;}
  bool isKeyJustReleased(int key){return keyStates[key]==RELEASED;}


};
