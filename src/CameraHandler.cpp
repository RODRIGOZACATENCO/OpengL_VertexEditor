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
  if(!freecam_info.is_movement_active) {
    return;
  }
  bool did_camera_move=false;
  if(keyboard->checkActionIsActive(MOVE_FORWARD)) {
    freecam_info.camera_pos+=freecam_info.camera_front*freecam_info.movement_speed*delta_time;
  }
  if(keyboard->checkActionIsActive(MOVE_BACKWARDS)) {
     freecam_info.camera_pos-=freecam_info.camera_front*freecam_info.movement_speed*delta_time;
  }
  if(keyboard->checkActionIsActive(MOVE_LEFT)) {
     freecam_info.camera_pos-=glm::cross(freecam_info.camera_front,camera_up)*freecam_info.movement_speed*delta_time;
  }
  if(keyboard->checkActionIsActive(MOVE_RIGHT)) {
     freecam_info.camera_pos+=glm::cross(freecam_info.camera_front,camera_up)*freecam_info.movement_speed*delta_time;
  }
  current_view_matrix = glm::lookAt( freecam_info.camera_pos,  freecam_info.camera_pos+freecam_info.camera_front, camera_up);
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
   gimball_info.camera_pos = {x, y, z};
  current_view_matrix = glm::lookAt(gimball_info.camera_pos, gimball_info.curr_target, camera_up);
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
     gimball_info.camera_pos = {x, y, z};
    current_view_matrix = glm::lookAt( gimball_info.camera_pos, gimball_info.curr_target, camera_up);
    break;
  case FREE:
    break;;
  }
}

void CameraHandler::freeCameraUpdate(bool *keys,double xposIn, double yposIn) {
  if(!freecam_info.is_movement_active) {
    return;
  }
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

   float xoffset = xpos - freecam_info.lastX;
   float yoffset = freecam_info.lastY - ypos;
   freecam_info.lastX = xpos;
   freecam_info.lastY = ypos;
  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  freecam_info.yaw += xoffset;
  freecam_info.pitch += yoffset;

  // make sure that when pitch is out of bounds, screen doesn't get flipped
  if (freecam_info.pitch > 89.0f)
    freecam_info.pitch = 89.0f;
  if (freecam_info.pitch < -89.0f)
    freecam_info.pitch = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(freecam_info.yaw)) * cos(glm::radians(freecam_info.pitch));
  front.y = sin(glm::radians(freecam_info.pitch));
  front.z = sin(glm::radians(freecam_info.yaw)) * cos(glm::radians(freecam_info.pitch));
  freecam_info.camera_front = glm::normalize(front);
  current_view_matrix = glm::lookAt( freecam_info.camera_pos, freecam_info.camera_pos +freecam_info.camera_front, camera_up);

}

void CameraHandler::updateInfoForCameraChange(CameraMode swap_from,CameraMode swap_into) {
  if(swap_from==GIMBALL && swap_into==FREE) {
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    int h,w;
    glfwGetWindowSize(window,&w,&h);
    glfwSetCursorPos(window,(float)(w/2),(float)(h/2));
    freecam_info.is_movement_active=true;
    freecam_info.camera_front=glm::normalize(gimball_info.curr_target- gimball_info.camera_pos);
    freecam_info.camera_pos=gimball_info.camera_pos;
    freecam_info.yaw= glm::degrees(atan2(freecam_info.camera_front.z, freecam_info.camera_front.x));
    freecam_info.pitch= glm::degrees(asin(freecam_info.camera_front.y));

  }
  if(swap_from==FREE && swap_into==GIMBALL) {
    is_camera_moving=true;
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
    camera_movement_info=std::make_unique<MovementInfo>(
    freecam_info.camera_pos,freecam_info.camera_front,gimball_info.camera_pos,
    glm::normalize(gimball_info.curr_target-gimball_info.camera_pos),
    &gimball_info.camera_pos,&gimball_info.curr_target
    );
  }
}

void CameraHandler::stepCameraMovementToPos() {
  float epsilon=0.01;
  float decay_rate=5;
  float factor = 1.0f - std::exp(-decay_rate * delta_time);
  *camera_movement_info->pos_ptr += (camera_movement_info->target_pos - *camera_movement_info->pos_ptr) * factor;
  is_camera_moving=glm::distance(*camera_movement_info->pos_ptr,camera_movement_info->target_pos)>epsilon;

  current_view_matrix=glm::lookAt( *camera_movement_info->pos_ptr, *camera_movement_info->pos_ptr+ *camera_movement_info->front_ptr,camera_up);

}

