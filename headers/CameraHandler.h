#pragma once


#include "CommonTypes.h"
#include <glm/glm.hpp>
#include <GUI.h>
#include <GLFW/glfw3.h>
#include <numbers>
inline float pi = std::numbers::pi_v<float>;

struct FreeCameraInfo {
  bool is_movement_active=false;
  glm::vec3 camera_front;
  float movement_speed=1.0f;
  float yaw;
  float pitch;
  float lastX;
  float lastY;

};
struct GimballCameraInfo {
  float min_zoom_radius = 1.0f;
  float max_zoom_radius = 7.0f;
  float gimball_zoom_sensitivity = 0.25;
  // Spherical coordinates
  float curr_rotation_radius = 3.0f; //radius of rotation
  float curr_azimuth = pi / 2.0f; // horizontal angle(radinas)
  float cur_elevation = 0.0f; // vertical angle(radians)
  // Clamp elevation so camera doesn't flip upside down
  const float max_elevation = glm::radians(89.0f);
  const float min_elevation = glm::radians(-89.0f);
  float rotation_speed = pi; //radians per second
  glm::vec3 curr_target = glm::vec3(0.0f); // point to orbit around
};



class CameraHandler {
private:
  GimballCameraInfo gimball_info;
  FreeCameraInfo free_camera_info;
  CameraMode current_camera_mode = GIMBALL;
  GUI *gui_ptr;
  GLFWwindow *window;
  float delta_time;

  glm::vec3 camera_pos;
  glm::vec3 camera_up = {0.0f, 1.0f, 0.0f};


  // Clamp elevation so camera doesn't flip upside down
  const float MAX_ELEVATION = glm::radians(89.0f);
  const float MIN_ELEVATION = glm::radians(-89.0f);
  glm::mat4 current_view_matrix;

public:
  CameraHandler(GUI *gui_ptr,GLFWwindow *window):gui_ptr(gui_ptr),window(window) {
    const bool no_keys[1024] = {};
    gimballCameraUpdate(no_keys);
    int w,h;
    glfwGetWindowSize(window,&w,&h);
    free_camera_info.lastX=w/2;
    free_camera_info.lastY=h/2;

  }

  void freeCameraUpdate(bool *keys,double xposIn, double yposIn);
  void gimballCameraUpdate(const bool* keys);

  void processZoom(double yoffset);

  void setDeltaTime(float delta_time) { this->delta_time = delta_time; }

  glm::mat4 getCurrentViewMatrix() { return current_view_matrix; }

  void setCurrentCameraMode(CameraMode camera_mode) { this->current_camera_mode = camera_mode; }
  void setGui(GUI *gui_ptr) {
    this->gui_ptr=gui_ptr;
  }
  glm::vec3 getCameraPosition() { return camera_pos; }

  glm::vec3 getCameraFront() { return gimball_info.curr_target; }
  CameraMode getCurrCameraMode(){return current_camera_mode;}

};
