

#pragma once

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <numbers>
enum CameraMode {
	GIMBALL,//camera view fixed on the center of the object
	FREE,//camera free to move and rotate
};

enum CameraMovement{
  LEFT,
  RIGHT,
  UP,
  DOWN,
};
class CameraHandler {
  private:
  bool keys[1024]={false};
  float pi = std::numbers::pi_v<float>;
	CameraMode current_camera_mode;
  float delta_time;
	float CAMERA_SPEED = pi;//radians per second
  float MIN_ZOOM_RADIUS=3.0f;
  float MAX_ZOOM_RADIUS=7.0f;
  float ZOOM_SENSITIVITY_GIMBALL=0.25;
  // Spherical coordinates
  float RADIUS    = 3.0f;   //radius of rotation
  float azimuth   = pi/2.0f;   // horizontal angle(radinas)
  float elevation = 0.0f;   // vertical angle(radians)

  glm::vec3 camera_pos;
  glm::vec3 camera_normal={0.0f,1.0f,0.0f};
  glm::vec3 target = glm::vec3(0.0f); // point to orbit around

  // Clamp elevation so camera doesn't flip upside down
  const float MAX_ELEVATION = glm::radians(89.0f);
  const float MIN_ELEVATION = glm::radians(-89.0f);
  glm::mat4 current_view_matrix=glm::lookAt(camera_pos, target, camera_normal);
 public:
  CameraHandler(){
    gimballCameraUpdate();
  }
  void gimballCameraUpdate();
  void processZoom(double yoffset);

  
  void setDeltaTime(float delta_time){
    this->delta_time=delta_time;
  }
  glm::mat4 getCurrentViewMatrix(){
    return current_view_matrix;
  }
  void setCurrentCameraMode(CameraMode camera_mode){
    this->current_camera_mode=camera_mode;
  }
  void setKey(int key, bool pressed){
    if(key >= 0 && key < 1024)
      keys[key] = pressed;
  }
};



