#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

// Shared headers from your global directory
#include "model_loader.h"
#include "TextureHandler.h"


// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int		width, int height) {
    glViewport(0, 0, width, height);
}



int SKYBOX_TEXTURE_ID=0,CONTAINER_TEXTURE_ID=1;
int main() {
    // GLFW Init
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Project", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // GLAD Init
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	int frameBufferWidth = 800;
	int frameBufferHeight = 600;
	glfwGetFramebufferSize(window,&frameBufferWidth,&frameBufferHeight);
	glViewport(0,0,frameBufferWidth,frameBufferHeight);
    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}