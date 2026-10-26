#pragma once


#include "CommonTypes.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <numbers>

#include "KeyboardHandler.h"
inline float pi = std::numbers::pi_v<float>;





class CameraHandler {
private:
  struct FreeCameraInfo {
    glm::vec3 camera_pos={0.0,0.5,-1};;
    bool is_movement_active=false;
    glm::vec3 camera_front;
    float movement_speed=10.0f;
    float yaw;
    float pitch;
    float lastX=0;
    float lastY=0;

  };
  struct GimballCameraInfo {
    glm::vec3 camera_pos={0.0,0.5,-1};
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
  struct MovementInfo {
    glm::vec3 *pos_ptr;
    glm::vec3 target_pos;
    glm::vec3 *front_ptr;
    glm::vec3 target_front;
    MovementInfo( glm::vec3 start_pos, glm::vec3 front_pos,glm::vec3 target_pos,
      glm::vec3 target_front,glm::vec3 *pos_ptr,glm::vec3 *front_ptr)
      :pos_ptr(pos_ptr),front_ptr(front_ptr),target_front(target_front),target_pos(target_pos) {
      *pos_ptr=start_pos;
      *front_ptr=front_pos;
    }
  };

  std::unique_ptr<MovementInfo> camera_movement_info;
  KeyboardHandler *keyboard;
  GimballCameraInfo gimball_info;
  FreeCameraInfo freecam_info;
  CameraMode current_camera_mode = GIMBALL;
  GLFWwindow *window;
  float delta_time;


  glm::vec3 camera_up = {0.0f, 1.0f, 0.0f};


  // Clamp elevation so camera doesn't flip upside down
  const float MAX_ELEVATION = glm::radians(89.0f);
  const float MIN_ELEVATION = glm::radians(-89.0f);
  glm::mat4 current_view_matrix;

public:
  bool is_camera_moving=false;
  CameraHandler(GLFWwindow *window,KeyboardHandler *keyboard):keyboard(keyboard),window(window) {
    int w,h;
    glfwGetWindowSize(window,&w,&h);
    freecam_info.lastX=w/2;
    freecam_info.lastY=h/2;
    setCurrentCameraMode(GIMBALL);

  }

  void freeCameraUpdate(bool *keys,double xposIn, double yposIn);
  void FreeCameraMovementUpdate();
  void gimballCameraUpdate();
  void setKeyboard(KeyboardHandler *keyboard){this->keyboard=keyboard;}
  void processZoom(double yoffset);

  void setDeltaTime(float delta_time) { this->delta_time = delta_time; }

  glm::mat4 getCurrentViewMatrix() { return current_view_matrix; }

  void setCurrentCameraMode(CameraMode camera_mode) {
    updateInfoForCameraChange(this->current_camera_mode,camera_mode);
    this->current_camera_mode = camera_mode;
  }

  void setFreeCameraCanMove(bool state) {
    freecam_info.is_movement_active=state;
  }
  glm::vec3 getCameraFront() { return gimball_info.curr_target; }
  CameraMode getCurrCameraMode(){return current_camera_mode;}
  void updateInfoForCameraChange(CameraMode swap_from,CameraMode swap_into);
  void stepCameraMovementToPos();
};
