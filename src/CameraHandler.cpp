//
// Created by rodrigo on 27/04/2026.
//
#include "CameraHandler.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

void CameraHandler::gimballCameraUpdate(const bool* keys) {
  if (keys[UP]) {
    gimball_info.cur_elevation += gimball_info.rotation_speed * delta_time;
    if (gimball_info.cur_elevation > gimball_info.max_elevation)
      gimball_info.cur_elevation = gimball_info.max_elevation;
  }
  if (keys[DOWN]) {
    gimball_info.cur_elevation -= gimball_info.rotation_speed * delta_time;
    if (gimball_info.cur_elevation < gimball_info.min_elevation)
      gimball_info.cur_elevation = gimball_info.min_elevation;
  }
  if (keys[LEFT]) {
    gimball_info.curr_azimuth += gimball_info.rotation_speed * delta_time;
    gimball_info.curr_azimuth = fmodf(gimball_info.curr_azimuth, -glm::radians(360.0f));
  }
  if (keys[RIGHT]) {
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
  // float xpos = static_cast<float>(xposIn);
  // float ypos = static_cast<float>(yposIn);
  //
  // float xoffset = xpos - lastX;
  // float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
  // lastX = xpos;
  // lastY = ypos;
  //
  // float sensitivity = 0.1f; // change this value to your liking
  // xoffset *= sensitivity;
  // yoffset *= sensitivity;
  //
  // yaw += xoffset;
  // pitch += yoffset;
  //
  // // make sure that when pitch is out of bounds, screen doesn't get flipped
  // if (pitch > 89.0f)
  //   pitch = 89.0f;
  // if (pitch < -89.0f)
  //   pitch = -89.0f;
  //
  // glm::vec3 front;
  // front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  // front.y = sin(glm::radians(pitch));
  // front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  // cameraFront = glm::normalize(front);
}
