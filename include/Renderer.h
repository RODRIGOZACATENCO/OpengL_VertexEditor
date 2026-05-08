//
// Created by rodrigo on 04/05/2026.
//

#ifndef VERTEXEDITOR_RENDERER_H
#define VERTEXEDITOR_RENDERER_H
#include <GLFW/glfw3.h>
#include <memory>
#include <optional>
#include <tuple>
#include <utility>
#include "ShaderHandler.h"
#include "GUI.h"
#include "Scene.h"
struct FramebufferInfo {
	unsigned int FBO;
	unsigned int texture;
	unsigned int DBO;
};
/*Renderer Class makes the calls to the graphic API
 *it holds all the shaders
 **/
class Renderer {
private:
	Scene* current_scene; // scene currently being drawn
	GUIState render_mode; // FACE, VERTEX OR EDGE EDITING
	GLFWwindow* window;
	int width, height;

	std::unique_ptr<Shader> main_shader;
	std::unique_ptr<Shader> color_picking_shader;
	std::unique_ptr<Shader> vertex_pass_shader;
	std::unique_ptr<Shader> edge_pass_shader;
	std::unique_ptr<Shader> main_pass_edge_shader;

	FramebufferInfo color_picking_framebuffer_info;

public:
	Renderer(GLFWwindow* window, Scene* start_scene)
	    : window(window), current_scene(start_scene), width(0), height(0) {
		FramebufferSetup();
		shaderSetup();
	}

	// Getters and Setters
	void setScreenSize(unsigned int width, unsigned int height) {
		this->width = width;
		this->height = height;
	}

	void setCurrentScene(Scene* scene) { this->current_scene = scene; }
	Scene* getCurrentScene() { return current_scene; }

	void setRenderMode(GUIState state) { this->render_mode = state; }
	GUIState getRenderMode() const { return render_mode; }

	GLFWwindow* getWindow() const { return window; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }

	// Methods
	void selectionBufferPass();
	void mainRenderPass();
	void FramebufferSetup();
	void shaderSetup();
	void resizeFramebuffer();
	void setViewProjectionMatrices();
	void cleanup();
	bool rendererIsReady(std::string* out_error = nullptr) const;
	std::optional<std::tuple<unsigned int, unsigned int, unsigned int>> meshElementDetection();
	std::pair<int, int> getCursorPositionInViewport(GLFWwindow* window);
};


#endif //VERTEXEDITOR_RENDERER_H