#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
enum ElementType {
  FACE,
  VERTEX,
  LINE,
};

class ElementEditing {
private:
  std::pair<ElementType, int> selected_element;

public:
  void vertex_editing();
  void face_editing();
  void line_editiing();
};