

#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
enum CameraType {
	GIMBALL,//camera view fixed on the center of the object, arrow movement
	FREE,//camera free to move and rotate
};
class CameraHandler {
	CameraType current_camera_mode;
	float CAMERA_SPEED = 3.0f;
  // Spherical coordinates
    float radius    = 5.0f;
    float azimuth   = 0.0f;   // horizontal angle
    float elevation = 0.5f;   // vertical angle

    glm::vec3 target = glm::vec3(0.0f); // point to orbit around

	private:
    // Clamp elevation so camera doesn't flip upside down
    const float MAX_ELEVATION = glm::radians(89.0f);
    const float MIN_ELEVATION = glm::radians(-89.0f);
};



