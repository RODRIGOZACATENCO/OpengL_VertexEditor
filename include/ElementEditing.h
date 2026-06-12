#pragma once
#define GLFW_INCLUDE_NONE

#include <iostream>

#include <tuple>
#include "../include/Scene.h"
#include "CommonTypes.h"

class Scene;
class ElementEditing {
private:
  /*type of element, mesh_id, element_id*/
  std::tuple<ElementType,unsigned int,unsigned int> selected_element;

  Scene *current_scene;
  glm::mat4 inverse_view_projection_matrix;
  int window_width,window_height;

public:
  ElementEditing(Scene *current_scene){

    this->current_scene=current_scene;
  }
  void setScreenSize(unsigned int width, unsigned int height) {
    this->window_width = width;
    this->window_height = height;
  }
  void setCurrentScene(Scene *scene) {
    this->current_scene=scene;
  }
  void setViewProjectionMatrix(glm::mat4 view_projection_matrix) {
    this->inverse_view_projection_matrix = glm::inverse(view_projection_matrix);
  }
  void setCurrentSelectedElement(ElementType element_type,unsigned int mesh_id,unsigned int element_id)
  {
    this->selected_element={element_type,mesh_id,element_id};
  }
  void vertex_editing();
  void face_editing();
  void line_editing();
  void vertexMovement();
  void vertexRayCaster(glm::vec2 mouse_pos,glm::vec3 camera_pos,glm::vec3 camera_front);
};