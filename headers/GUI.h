//
// Created by rodrigo on 29/04/2026.
//

#pragma once
#include "CameraHandler.h"
#include<CommonTypes.h>

#include "KeyboardHandler.h"

struct MainGUI_state {

  bool isFaceSelectionActive = false;
  bool isVertexSelectionActive = false;
  bool isEdgeSelectionActive = false;
  CameraMode cameraMode = GIMBALL;
};

class GUI {
public:
  KeyboardHandler *keyboard;
  CameraHandler *camera;
  GLFWwindow *window;
  bool has_state_changed = false;
  bool isPanelVisible = true;
private:
  GUIState currentState = FACE_EDITING;
  CameraMode currentCameraMode = FREE;
  unsigned int reset_selection_buffer_flag =
    0; // flag to reset the selection buffer on the GPU when the clear button
  // is pressed



public:
  MainGUI_state main_state;
  GUI(GLFWwindow *window): window(window){}

  void showMainWindowGUI();
  void setKeyboard(KeyboardHandler *keyboard){this->keyboard=keyboard;}
  void setState(GUIState new_state) {
    currentState = new_state;
    has_state_changed = true;
    main_state.isFaceSelectionActive = (new_state == FACE_EDITING);
    main_state.isVertexSelectionActive = (new_state == VERTEX_EDITING);
    main_state.isEdgeSelectionActive = (new_state == EDGE_EDITING);
  }
  void setCamera(CameraHandler *camera){this->camera=camera;}
  GUIState getCurrentState() const { return currentState; }
  unsigned int getResetFlag() const { return reset_selection_buffer_flag; }
  void setResetFlag(unsigned int flag) { reset_selection_buffer_flag = flag; }

  void setCameraMode(CameraMode mode) {
    currentCameraMode = mode;
    main_state.cameraMode = mode;
    camera->setCurrentCameraMode(mode);
  }

  CameraMode getCurrentCameraMode() const { return currentCameraMode; }
};
