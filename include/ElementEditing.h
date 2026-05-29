#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
enum ElementType {
  FACE,
  VERTEX,
  LINE,
};
enum LineType {
  X,
  Y,
  Z
};
//asuming (0,0,0) is the vertex selected
/*
 * Vertex:
 * when selected, pressing x,y,z will render a line in
 * that direction, then the user will hold mouse and
 * move the vertex along this line, lines will be the
 * same size regardless of mesh distance and size, render on top of the mesh
 *
 *
 */
class ElementEditing {
private:
	std::pair<ElementType, int> selected_element;
  LineType selected_line;
public:
  //will render the tree axis lines first
  void vertex_editing();
  void face_editing();
  void line_editing();
};