//
// Created by rodrigo on 27/04/2026.
//
#include "CameraHandler.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <map>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include "KeyboardHandler.h"

void CameraHandler::FreeCameraMovementUpdate() {
  if(!free_camera_info.is_movement_active) {
    return;
  }
  bool did_camera_move=false;
  if(keyboard->checkActionIsActive(MOVE_FORWARD)) {
    camera_pos+=free_camera_info.camera_front*free_camera_info.movement_speed*delta_time;
  }
  if(keyboard->checkActionIsActive(MOVE_BACKWARDS)) {
    camera_pos-=free_camera_info.camera_front*free_camera_info.movement_speed*delta_time;
  }
  if(keyboard->checkActionIsActive(MOVE_LEFT)) {
    camera_pos-=glm::cross(free_camera_info.camera_front,camera_up)*free_camera_info.movement_speed*delta_time;
  }
  if(keyboard->checkActionIsActive(MOVE_RIGHT)) {
    camera_pos+=glm::cross(free_camera_info.camera_front,camera_up)*free_camera_info.movement_speed*delta_time;
  }
  current_view_matrix = glm::lookAt(camera_pos, camera_pos+free_camera_info.camera_front, camera_up);
}

void CameraHandler::gimballCameraUpdate() {
  if (keyboard->checkActionIsActive(MOVE_UP)) {
    gimball_info.cur_elevation += gimball_info.rotation_speed * delta_time;
    if (gimball_info.cur_elevation > gimball_info.max_elevation)
      gimball_info.cur_elevation = gimball_info.max_elevation;
  }
  if (keyboard->checkActionIsActive(MOVE_DOWN)) {
    gimball_info.cur_elevation -= gimball_info.rotation_speed * delta_time;
    if (gimball_info.cur_elevation < gimball_info.min_elevation)
      gimball_info.cur_elevation = gimball_info.min_elevation;
  }
  if (keyboard->checkActionIsActive(MOVE_LEFT)) {
    gimball_info.curr_azimuth += gimball_info.rotation_speed * delta_time;
    gimball_info.curr_azimuth = fmodf(gimball_info.curr_azimuth, -glm::radians(360.0f));
  }
  if (keyboard->checkActionIsActive(MOVE_RIGHT)) {
    gimball_info.curr_azimuth -= gimball_info.rotation_speed * delta_time;
    gimball_info.curr_azimuth = fmodf(gimball_info.curr_azimuth, glm::radians(360.0f));
  }
  float x, y, z;
  x = std::cos(gimball_info.curr_azimuth) * std::cos(gimball_info.cur_elevation) * gimball_info.curr_rotation_radius;
  y = std::sin(gimball_info.cur_elevation) * gimball_info.curr_rotation_radius;
  z = std::sin(gimball_info.curr_azimuth) * std::cos(gimball_info.cur_elevation) * gimball_info.curr_rotation_radius;
  camera_pos = {x, y, z};
  current_view_matrix = glm::lookAt(camera_pos, gimball_info.curr_target, camera_up);
}

void CameraHandler::processZoom(double yoffset) {
  switch (current_camera_mode) {
  case GIMBALL:
    gimball_info.curr_rotation_radius += yoffset * gimball_info.gimball_zoom_sensitivity;
    if (gimball_info.curr_rotation_radius > gimball_info.max_zoom_radius)
      gimball_info.curr_rotation_radius = gimball_info.max_zoom_radius;
    if (gimball_info.curr_rotation_radius < gimball_info.min_zoom_radius)
      gimball_info.curr_rotation_radius = gimball_info.min_zoom_radius;
    float x, y, z;
    x = std::cos(gimball_info.curr_azimuth) * std::cos(gimball_info.cur_elevation) * gimball_info.curr_rotation_radius;
    y = std::sin(gimball_info.cur_elevation) * gimball_info.curr_rotation_radius;
    z = std::sin(gimball_info.curr_azimuth) * std::cos(gimball_info.cur_elevation) * gimball_info.curr_rotation_radius;
    camera_pos = {x, y, z};
    current_view_matrix = glm::lookAt(camera_pos, gimball_info.curr_target, camera_up);
    break;
  case FREE:
    break;;
  }
}

void CameraHandler::freeCameraUpdate(bool *keys,double xposIn, double yposIn) {
  if(!free_camera_info.is_movement_active) {
    return;
  }
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

   float xoffset = xpos - free_camera_info.lastX;
   float yoffset = free_camera_info.lastY - ypos;
   free_camera_info.lastX = xpos;
   free_camera_info.lastY = ypos;
  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  free_camera_info.yaw += xoffset;
  free_camera_info.pitch += yoffset;

  // make sure that when pitch is out of bounds, screen doesn't get flipped
  if (free_camera_info.pitch > 89.0f)
    free_camera_info.pitch = 89.0f;
  if (free_camera_info.pitch < -89.0f)
    free_camera_info.pitch = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(free_camera_info.yaw)) * cos(glm::radians(free_camera_info.pitch));
  front.y = sin(glm::radians(free_camera_info.pitch));
  front.z = sin(glm::radians(free_camera_info.yaw)) * cos(glm::radians(free_camera_info.pitch));
  free_camera_info.camera_front = glm::normalize(front);
  current_view_matrix = glm::lookAt(camera_pos, camera_pos +free_camera_info.camera_front, camera_up);

}

void CameraHandler::updateInfoForCameraChange(CameraMode swap_from,CameraMode swap_into) {
  if(swap_from==GIMBALL && swap_into==FREE) {
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
    int h,w;
    glfwGetWindowSize(window,&w,&h);
    glfwSetCursorPos(window,(float)(w/2),(float)(h/2));
    free_camera_info.is_movement_active=true;
    free_camera_info.camera_front=glm::normalize(gimball_info.curr_target-camera_pos);
    free_camera_info.yaw= glm::degrees(atan2(free_camera_info.camera_front.z, free_camera_info.camera_front.x));
    free_camera_info.pitch= glm::degrees(asin(free_camera_info.camera_front.y));
  }


}
