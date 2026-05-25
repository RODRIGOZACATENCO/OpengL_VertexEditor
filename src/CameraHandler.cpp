//
// Created by rodrigo on 27/04/2026.
//
#include "CameraHandler.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

void CameraHandler::gimballCameraUpdate(const bool *keys) {
  if (keys[UP]) {
    elevation += CAMERA_SPEED * delta_time;
    if (elevation > MAX_ELEVATION)
      elevation = MAX_ELEVATION;
  }
  if (keys[DOWN]) {
    elevation -= CAMERA_SPEED * delta_time;
    if (elevation < MIN_ELEVATION)
      elevation = MIN_ELEVATION;
  }
  if (keys[LEFT]) {
    azimuth += CAMERA_SPEED * delta_time;
    azimuth = fmodf(azimuth, -glm::radians(360.0f));
  }
  if (keys[RIGHT]) {
    azimuth -= CAMERA_SPEED * delta_time;
    azimuth = fmodf(azimuth, glm::radians(360.0f));
  }
  float x, y, z;
  x = std::cos(azimuth) * std::cos(elevation) * RADIUS;
  y = std::sin(elevation) * RADIUS;
  z = std::sin(azimuth) * std::cos(elevation) * RADIUS;
  camera_pos = {x, y, z};
  current_view_matrix = glm::lookAt(camera_pos, target, camera_normal);
}

void CameraHandler::processZoom(double yoffset) {
  switch (current_camera_mode) {
  case GIMBALL:
    RADIUS += yoffset * ZOOM_SENSITIVITY_GIMBALL;
    if (RADIUS > MAX_ZOOM_RADIUS)
      RADIUS = MAX_ZOOM_RADIUS;
    if (RADIUS < MIN_ZOOM_RADIUS)
      RADIUS = MIN_ZOOM_RADIUS;
    float x, y, z;
    x = std::cos(azimuth) * std::cos(elevation) * RADIUS;
    y = std::sin(elevation) * RADIUS;
    z = std::sin(azimuth) * std::cos(elevation) * RADIUS;
    camera_pos = {x, y, z};
    current_view_matrix = glm::lookAt(camera_pos, target, camera_normal);
    break;
  case FREE:
    break;
    ;
  }
}
