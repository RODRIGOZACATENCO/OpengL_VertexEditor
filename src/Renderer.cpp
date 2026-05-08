//
// Created by rodrigo on 04/05/2026.
//
#include <filesystem>
#include <glad/glad.h>
#include "../include/Renderer.h"
#include "../OpenGL_include/TextureHandler.h"

void Renderer::selectionBufferPass()
{
	glfwGetFramebufferSize(window, (&width), &height);
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, color_picking_framebuffer_info.FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE); // enable setting point size in the shader
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	unsigned int mesh_id = 0;
	switch (render_mode)
	{
		case FACE_EDITING:
			color_picking_shader->use();
			for (const auto& mesh_ptr : current_scene->getMeshes())
			{
				Mesh* mesh = mesh_ptr.get();
				RenderInfo ri = current_scene->getRenderInfo(mesh);
				color_picking_shader->setMat4("model", ri.model);
				color_picking_shader->setUint("MeshID", mesh_id++);
				color_picking_shader->setUint("gui_state",render_mode);
				glBindVertexArray(ri.VAO);
				glDrawElements(GL_TRIANGLES, mesh->getFaceRenderIndices().size(), GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
			break;
		case VERTEX_EDITING:
			color_picking_shader->use();
			for (const auto& mesh_ptr : current_scene->getMeshes())
			{
				Mesh* mesh = mesh_ptr.get();
				RenderInfo ri = current_scene->getRenderInfo(mesh);
				color_picking_shader->setMat4("model", ri.model);
				color_picking_shader->setUint("MeshID", mesh_id++);
				color_picking_shader->setUint("gui_state", render_mode);
				glBindVertexArray(ri.VAO);
				glDrawElements(GL_POINTS, mesh->getFaceRenderIndices().size(), GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
			break;
		case EDGE_EDITING:
			edge_pass_shader->use();
			for (const auto& mesh_ptr : current_scene->getMeshes()) {
				Mesh* mesh = mesh_ptr.get();
				RenderInfo ri = current_scene->getRenderInfo(mesh);
				edge_pass_shader->setMat4("model", ri.model);
				edge_pass_shader->setUint("MeshID", mesh_id++);
				glBindVertexArray(ri.edge_VAO);
				glDrawElements(GL_LINES, mesh->getEdgeRenderIndices().size(), GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
			break;
	}
}

void Renderer::mainRenderPass()
{
	setViewProjectionMatrices();
	std::string error;
	if (!rendererIsReady(&error)) {
		std::cout << "Renderer is not ready" << std::endl;
		std::cout << error << std::endl;
		return;
	}
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	int face_offset=0;//offset of the number of faces in each mesh, sent to the gpu
	int vertex_offset=current_scene->getFaceSelectionArray()->size();
	int edges_offset =vertex_offset+current_scene->getVertexSelectionArray()->size();
	int i=0;
	for (const auto& mesh_ptr : current_scene->getMeshes())

	{
		Mesh* mesh = mesh_ptr.get();
		RenderInfo ri = current_scene->getRenderInfo(mesh);
		main_shader->use();
		main_shader->setMat4("model", ri.model);
		main_shader->setInt("num_faces_offset", face_offset);
		main_shader->setUint("gui_state", render_mode);
		face_offset += mesh->getFaces().size();

		glBindVertexArray(ri.VAO);
		glDrawElements(GL_TRIANGLES, mesh->getFaceRenderIndices().size(), GL_UNSIGNED_INT, 0);
		
		if(render_mode==VERTEX_EDITING) {

			vertex_pass_shader->use();
			vertex_pass_shader->setMat4("model", ri.model);
			vertex_pass_shader->setInt("num_vertices_offset", vertex_offset);
			vertex_offset += mesh->getVertices().size();
			glDrawArrays(GL_POINTS, 0,mesh->getVertices().size());
		}
		/*
				if (render_mode == EDGE_EDITING)
				{
					glDisable(GL_CULL_FACE);
					main_pass_edge_shader->use();
					main_pass_edge_shader->setMat4("model", ri.model);
					main_pass_edge_shader->setInt("num_vertices_offset", vertex_offset);
					edges_offset += mesh->getEdges().size();
					glDrawElements(GL_LINES, mesh->getEdgeRenderIndices().size(), GL_UNSIGNED_INT, 0);
				}
					*/
		glBindVertexArray(0);
	}
}

void Renderer::FramebufferSetup()
{
	glfwGetFramebufferSize(window, &width, &height);
	// it will create the frame buffer
	glGenFramebuffers(1, &color_picking_framebuffer_info.FBO);
	glGenTextures(1, &color_picking_framebuffer_info.texture);
	glGenRenderbuffers(1, &color_picking_framebuffer_info.DBO);
	glBindFramebuffer(GL_FRAMEBUFFER, color_picking_framebuffer_info.FBO);
	glBindTexture(GL_TEXTURE_2D, color_picking_framebuffer_info.texture);
	// it's a texture that will cover the entire viewport
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, width, height, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// binds the texture to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_picking_framebuffer_info.texture, 0);
	GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, drawBuffers);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, color_picking_framebuffer_info.DBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, color_picking_framebuffer_info.DBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
}
void Renderer::setViewProjectionMatrices(){

	main_shader->use();
	main_shader->setMat4("view", current_scene->getViewMatrix());
	main_shader->setMat4("projection", current_scene->getProjectionMatrix());

	color_picking_shader->use();
	color_picking_shader->setMat4("view", current_scene->getViewMatrix());
	color_picking_shader->setMat4("projection", current_scene->getProjectionMatrix());

	vertex_pass_shader->use();
	vertex_pass_shader->setMat4("view", current_scene->getViewMatrix());
	vertex_pass_shader->setMat4("projection", current_scene->getProjectionMatrix());

	edge_pass_shader->use();
	edge_pass_shader->setMat4("view", current_scene->getViewMatrix());
	edge_pass_shader->setMat4("projection", current_scene->getProjectionMatrix());

	main_pass_edge_shader->use();
	main_pass_edge_shader->setMat4("view", current_scene->getViewMatrix());
	main_pass_edge_shader->setMat4("projection", current_scene->getProjectionMatrix());
}

//@TODO later, change it into different function that generates new renders
void Renderer::shaderSetup() {
	std::filesystem::path shadersRoot = std::filesystem::path(ROOT_DIR) / "shaders";
	std::string vertex_shader_path = (shadersRoot / "shader.vert").string();
	std::string geometry_shader_path = (shadersRoot / "shader.geom").string();
	std::string fragment_shader_path = (shadersRoot / "shader.frag").string();
	std::string fragment_colorPickingShader_path = (shadersRoot / "colorPickingShader.frag").string();
	std::string fragment_vertexPassShader_path = (shadersRoot / "vertexPass.frag").string();
	std::string fragment_edgePass_path = (shadersRoot / "edgePass.frag").string();
	std::string geometry_edgePass_shader_path = (shadersRoot / "edgePass.geom").string();
	std::string vertex_edgePass_shader_path = (shadersRoot / "edgePass.vert").string();
	std::string main_edge_pass_shader_path = (shadersRoot / "mainEdgePass.frag").string();
	main_shader = std::make_unique<Shader>(vertex_shader_path.c_str(), geometry_shader_path.c_str(), fragment_shader_path.c_str());
	color_picking_shader = std::make_unique<Shader>(vertex_shader_path.c_str(), fragment_colorPickingShader_path.c_str());
	vertex_pass_shader = std::make_unique<Shader>(vertex_shader_path.c_str(), fragment_vertexPassShader_path.c_str());
	edge_pass_shader = std::make_unique<Shader>(vertex_edgePass_shader_path.c_str(), geometry_edgePass_shader_path.c_str(), fragment_edgePass_path.c_str());
	main_pass_edge_shader = std::make_unique<Shader>(vertex_edgePass_shader_path.c_str(), geometry_edgePass_shader_path.c_str(), main_edge_pass_shader_path.c_str());
	setViewProjectionMatrices();
}

void Renderer::cleanup() {
	glDeleteFramebuffers(1, &color_picking_framebuffer_info.FBO);
	glDeleteTextures(1, &color_picking_framebuffer_info.texture);
	glDeleteRenderbuffers(1, &color_picking_framebuffer_info.DBO);
}

void Renderer::resizeFramebuffer()
{
	glfwGetFramebufferSize(window, &width, &height);
	glBindTexture(GL_TEXTURE_2D, color_picking_framebuffer_info.texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, width, height, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind when done

	// 2. Bind and resize the depth renderbuffer attachment
	glBindRenderbuffer(GL_RENDERBUFFER, color_picking_framebuffer_info.DBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0); // Unbind when done
}

std::optional<std::tuple<unsigned int, unsigned int, unsigned int>> Renderer::meshElementDetection(){
	auto [framebuffer_x, framebuffer_y] = getCursorPositionInViewport(window);
	glBindFramebuffer(GL_FRAMEBUFFER, color_picking_framebuffer_info.FBO);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	unsigned int pixel_data[3];
	GLint currentFBO;
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentFBO);
	glReadPixels(framebuffer_x, framebuffer_y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &pixel_data);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	std::cout << pixel_data[0] << " " << pixel_data[1] <<" "<<pixel_data[2] << std::endl;
	if (pixel_data[0] == 0)
		return std::nullopt; // background
	return {{pixel_data[0]-1, pixel_data[1], pixel_data[2]}};
}

std::pair<int,int> Renderer::getCursorPositionInViewport(GLFWwindow *window) {

	// GLFW cursor: 0,0 is top left corner, width,height is bottom right corner
	// glReadPixels: 0,0 is bottom left corner, width,height is top right corner
	double mouse_x, mouse_y;
	glfwGetCursorPos(window, &mouse_x, &mouse_y);
	glfwGetWindowSize(window, &width, &height);
	int framebuffer_width, framebuffer_height;
	glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
	// convert mouse coordinates to framebuffer coordinates
	int framebuffer_x = mouse_x * (framebuffer_width / (double)width);
	int framebuffer_y = (height - mouse_y) * (framebuffer_height / (double)height);
	return {framebuffer_x, framebuffer_y};
}

bool Renderer::rendererIsReady(std::string* out_error) const {
	auto fail = [&](const std::string& msg) {
		if (out_error) *out_error = msg;
		return false;
	};

	if (!window)               return fail("No GLFW window set.");
	if (!current_scene)        return fail("No scene set.");
	if (!main_shader)          return fail("Main shader not loaded.");
	if (!color_picking_shader) return fail("Color picking shader not loaded.");
	if (!vertex_pass_shader)   return fail("Vertex pass shader not loaded.");
	if (!edge_pass_shader)     return fail("Edge pass shader not loaded.");
	if (!main_pass_edge_shader)return fail("Main pass edge shader not loaded.");
	if (width <= 0 || height <= 0) return fail("Invalid screen dimensions.");
	if (color_picking_framebuffer_info.FBO == 0) return fail("Framebuffer not set up.");

	return true;
}