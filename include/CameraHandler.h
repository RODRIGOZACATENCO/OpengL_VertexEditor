

#ifndef VERTEXEDITOR_CAMERAHANDLER_H
#define VERTEXEDITOR_CAMERAHANDLER_H
#include <GLFW/glfw3.h>
enum cameraType {
	GIMBALL,//camera view fixed on the center of the object, arrow movement
	FREE,//camera free to move and rotate
};
class CameraHandler {
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float CAMERA_SPEED = 3.0f;

};


#endif //VERTEXEDITOR_CAMERAHANDLER_H