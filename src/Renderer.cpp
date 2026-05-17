//
// Created by rodrigo on 04/05/2026.
//

#include "../include/Renderer.h"
#include "GUI.h"
#include "Scene.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>
#include <iostream>
#include <vector>

void Renderer::processDrawCall(Render_type type_of_render) {
  updateModelMatrices();
  setViewProjectionMatrices();
  glfwGetFramebufferSize(window, (&width), &height);
  glViewport(0, 0, width, height);
  switch (type_of_render) {
  case main_render_pass:
    mainRenderPass();
    break;

  case element_detection_pass:
    current_scene->resetVertexAlreadyRendered();
    elementDetectionPass();
    break;
  }
  
}

void Renderer::elementDetectionPass() {

  glBindFramebuffer(GL_FRAMEBUFFER, element_detection_framebuffer_info.FBO);
  glClearColor(0.0, 0.0, 0.0, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glFrontFace(GL_CCW);
  glEnable(GL_PROGRAM_POINT_SIZE); // enable setting point size in the shader
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  unsigned int mesh_id = 0;

  switch (current_rendering_mode) {
  case FACE_EDITING:
    shaders[face_detection]->use();
    for (const auto &mesh_ptr : current_scene->getMeshes()) {
      Mesh *mesh = mesh_ptr.get();
      RenderInfo ri = current_scene->getRenderInfo(mesh);
      shaders[face_detection]->setMat4("model", ri.model);
      shaders[face_detection]->setUint("MeshID", mesh_id++);
      glBindVertexArray(ri.VAO);
      glDrawElements(GL_TRIANGLES, mesh->getFaceRenderIndices().size(),
                     GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
    }
    break;

  case VERTEX_EDITING:{
    shaders[vertex_detection]->use();
    unsigned int vertex_offset = 0;
    for (const auto &mesh_ptr : current_scene->getMeshes()) {
      Mesh *mesh = mesh_ptr.get();
      RenderInfo ri = current_scene->getRenderInfo(mesh);
      shaders[vertex_detection]->setMat4("model", ri.model);
      shaders[vertex_detection]->setMat4("projection",
                                         current_scene->getProjectionMatrix());
      shaders[vertex_detection]->setMat3(
          "normal_matrix", current_scene->getNormalMatrixFromModel(ri.model));
      shaders[vertex_detection]->setUint("MeshID", mesh_id++);
      shaders[vertex_detection]->setUint("vertex_offset", vertex_offset);
      vertex_offset += mesh->getVertices().size();
      glBindVertexArray(ri.VAO);
      glDrawElements(GL_TRIANGLES, mesh->getFaceRenderIndices().size(),
                     GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
    }
  }
    break;
    case EDGE_EDITING:
    auto &shader =shaders[edge_detection];
    shader->use();
    unsigned int edge_faces_normal_offset=0;
    for (const auto &mesh_ptr : current_scene->getMeshes()) {
      Mesh *mesh = mesh_ptr.get();
      RenderInfo ri = current_scene->getRenderInfo(mesh);
      shader->setMat4("model", ri.model);
      shader->setMat3("normal_matrix", current_scene->getNormalMatrixFromModel(ri.model));
      shader->setUint("edge_faces_normal_offset", edge_faces_normal_offset);
      shader->setUint("MeshID", mesh_id++);
      edge_faces_normal_offset+=mesh->getEdges().size();
      glBindVertexArray(ri.edge_VAO);
      glDrawElements(GL_LINES, mesh->getEdgeRenderIndices().size(),
                     GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
      }
    break;
  }
}

void Renderer::mainRenderPass() {
  std::string error;
  if (!rendererIsReady(&error)) {
    std::cout << "Renderer is not ready" << std::endl;
    std::cout << error << std::endl;
    return;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_PROGRAM_POINT_SIZE);
  int face_offset = 0;
  int vertex_offset = current_scene->getFaceSelectionArray()->size();
  int edges_offset =
      current_scene->getFaceSelectionArray()->size()
       + current_scene->getVertexSelectionArray()->size();

  unsigned int edge_faces_normal_offset=0;
  for (const auto &mesh_ptr : current_scene->getMeshes()) {
    Mesh *mesh = mesh_ptr.get();
    RenderInfo ri = current_scene->getRenderInfo(mesh);
    // renders the default mesh color, grey with green outline on the edges
    // Render default pass (base color + outline) AFTER face pass so outline is
    // on top
    Shader *shader=shaders[face_color_pass].get();
    shader->use();
    shader->setMat4("model", ri.model);
    shader->setUint("current_rendering_mode", current_rendering_mode);
    shader->setUint("faces_offset", face_offset);
    face_offset += mesh->getFaces().size();

    glBindVertexArray(ri.VAO);
    glDrawElements(GL_TRIANGLES, mesh->getFaceRenderIndices().size(),
                   GL_UNSIGNED_INT, 0);
    

    shader=shaders[edge_color_pass].get();
    shader->use();
    shader->setMat4("model", ri.model);
    shader->setMat3("normal_matrix", current_scene->getNormalMatrixFromModel(ri.model));
    shader->setUint("edge_faces_normal_offset", edge_faces_normal_offset);
    shader->setUint("edges_offset", edges_offset);
    edges_offset+=mesh->getEdges().size();
    edge_faces_normal_offset+=mesh->getEdges().size();
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(-1.0f, -1.0f);
    glDisable(GL_CULL_FACE);
    glBindVertexArray(ri.edge_VAO);
    glDrawElements(GL_LINES, mesh->getEdgeRenderIndices().size(),
                GL_UNSIGNED_INT, 0);
    glEnable(GL_CULL_FACE);


    if (current_rendering_mode == VERTEX_EDITING) {
      shader=shaders[vertex_color_pass].get();
      shader->use();
      shader->setMat4("model", ri.model);
      shader->setInt("num_vertices_offset", vertex_offset);
      vertex_offset += mesh->getVertices().size();
      glBindVertexArray(ri.VAO);
      glDrawArrays(GL_POINTS, 0, mesh->getVertices().size());

    }
    glDisable(GL_POLYGON_OFFSET_FILL);
     
    
  } 
}

void Renderer::FramebufferSetup() {
  glfwGetFramebufferSize(window, &width, &height);
  // it will create the frame buffer
  glGenFramebuffers(1, &element_detection_framebuffer_info.FBO);
  glGenTextures(1, &element_detection_framebuffer_info.texture);
  glGenRenderbuffers(1, &element_detection_framebuffer_info.DBO);
  glBindFramebuffer(GL_FRAMEBUFFER, element_detection_framebuffer_info.FBO);
  glBindTexture(GL_TEXTURE_2D, element_detection_framebuffer_info.texture);
  // it's a texture that will cover the entire viewport
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, width, height, 0, GL_RGB_INTEGER,
               GL_UNSIGNED_INT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // binds the texture to the framebuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         element_detection_framebuffer_info.texture, 0);
  GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, drawBuffers);
  glReadBuffer(GL_COLOR_ATTACHMENT0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindRenderbuffer(GL_RENDERBUFFER, element_detection_framebuffer_info.DBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER,
                            element_detection_framebuffer_info.DBO);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
              << std::endl;
  }
}
void Renderer::setViewProjectionMatrices() {

  for (int i = 0; i < shaders.size(); i++) {
    if (shaders[i]) {
      shaders[i]->use();
      shaders[i]->setMat4("view_projection",
                          current_scene->getViewProjectionMatrix());

      // vertex shaders need projection separated
      if (i == Shader_names::vertex_detection ||
          i == Shader_names::vertex_color_pass) {
        shaders[i]->setMat4("projection", current_scene->getProjectionMatrix());
      }
    }
  }
}

//@TODO later, change it into different function that generates new renders
void Renderer::shaderSetup() {
  // the paths for the color pass
  std::string root = ROOT_DIR;
  std::string face_color_pass_dir =
      root + "shaders/mesh_editing/color_pass/faces/";

  std::string vertex_color_pass_dir =
      root + "shaders/mesh_editing/color_pass/vertices/";
  std::string edge_color_pass_dir =
      root + "shaders/mesh_editing/color_pass/edges/";

  std::string face_detection_dir =
      root + "shaders/mesh_editing/element_detection/faces/";
  std::string vertex_detection_dir =
      root + "shaders/mesh_editing/element_detection/vertices/";
  std::string edge_detection_dir =
      root + "shaders/mesh_editing/element_detection/edges/";
  shaders.resize(Shader_names::shader_count); // resize to create elements

  shaders[Shader_names::face_color_pass] = std::make_unique<Shader>(
      face_color_pass_dir + "faceColorPass.vert",
      face_color_pass_dir + "faceColorPass.frag");

  shaders[Shader_names::vertex_color_pass] =
      std::make_unique<Shader>(vertex_color_pass_dir + "vertexColorPass.vert",
                               vertex_color_pass_dir + "vertexColorPass.frag");
  
  shaders[Shader_names::edge_color_pass] =
  std::make_unique<Shader>(edge_color_pass_dir + "edgeColorPass.vert",
                           edge_color_pass_dir + "edgeColorPass.geom",
                           edge_color_pass_dir + "edgeColorPass.frag");
                           

  shaders[Shader_names::face_detection] =
      std::make_unique<Shader>(face_detection_dir + "faceDetection.vert",
                               face_detection_dir + "faceDetection.frag");

  shaders[Shader_names::vertex_detection] =
      std::make_unique<Shader>(vertex_detection_dir + "vertexDetection.vert",
                               vertex_detection_dir + "vertexDetection.geom",
                               vertex_detection_dir + "vertexDetection.frag");
  
shaders[Shader_names::edge_detection] =
      std::make_unique<Shader>(edge_detection_dir + "edgeDetection.vert",
                                edge_detection_dir + "edgeDetection.geom",
                                edge_detection_dir + "edgeDetection.frag");

  setViewProjectionMatrices();
}

void Renderer::updateModelMatrices() {
  constexpr float pi = std::numbers::pi_v<float>;
  for (int i = 0; i < current_scene->getMeshes().size(); i++) {
    auto *mesh = current_scene->getMeshes()[i].get();
    auto name = current_scene->getMeshName(mesh);
    glm::quat& object_orientation = current_scene->getRenderInfo(mesh).object_orientation;
    glm::vec3 rotation_axis= {1.0f, 0.2f, 0.5f};
    float rotation_speed= pi/10;
    // model for piramid
    if (name == "pyramid") {
      glm::mat4 model = glm::mat4(1.0f);

      model = glm::translate(model, glm::vec3(0.0f, -1.0f, -5.0f));

      float delta_angle = rotation_speed * delta_time;
      glm::quat delta_rotation = glm::angleAxis(delta_angle, rotation_axis);
      object_orientation=delta_rotation*object_orientation;
      object_orientation=glm::normalize(object_orientation);

      model= model*glm::mat4_cast(object_orientation);
      current_scene->setModelMatrix(mesh, model);
    }
    // model for piramid
    if (name == "cube") {
      rotation_axis= glm::vec3(1.0f, 0.2f, 0.5f);
      rotation_speed= -pi/10;
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
      model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
 
      float delta_angle = rotation_speed * delta_time;
      glm::quat delta_rotation = glm::angleAxis(delta_angle, rotation_axis);
      object_orientation=delta_rotation*object_orientation;
      object_orientation=glm::normalize(object_orientation);
      
      model =model*glm::mat4_cast(object_orientation);
      current_scene->setModelMatrix(mesh, model);
    }
  }
}

void Renderer::cleanup() {
  glDeleteFramebuffers(1, &element_detection_framebuffer_info.FBO);
  glDeleteTextures(1, &element_detection_framebuffer_info.texture);
  glDeleteRenderbuffers(1, &element_detection_framebuffer_info.DBO);
}

void Renderer::resizeFramebuffer() {
  glfwGetFramebufferSize(window, &width, &height);
  glBindTexture(GL_TEXTURE_2D, element_detection_framebuffer_info.texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, width, height, 0, GL_RGB_INTEGER,
               GL_UNSIGNED_INT, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0); // Unbind when done

  // 2. Bind and resize the depth renderbuffer attachment
  glBindRenderbuffer(GL_RENDERBUFFER, element_detection_framebuffer_info.DBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0); // Unbind when done
}

std::optional<std::tuple<unsigned int, unsigned int, unsigned int>>
Renderer::meshElementDetection() {
  auto [framebuffer_x, framebuffer_y] = getCursorPositionInViewport(window);
  glBindFramebuffer(GL_FRAMEBUFFER, element_detection_framebuffer_info.FBO);
  glReadBuffer(GL_COLOR_ATTACHMENT0);
  unsigned int pixel_data[3];
  GLint currentFBO;
  glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentFBO);
  glReadPixels(framebuffer_x, framebuffer_y, 1, 1, GL_RGB_INTEGER,
               GL_UNSIGNED_INT, &pixel_data);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  std::cout << pixel_data[0] << " " << pixel_data[1] << " " << pixel_data[2]
            << std::endl;
  if (pixel_data[0] == 0)
    return std::nullopt; // background
  return {{pixel_data[0] - 1, pixel_data[1], pixel_data[2]}};
}

std::pair<int, int> Renderer::getCursorPositionInViewport(GLFWwindow *window) {

  // GLFW cursor: 0,0 is top left corner, width,height is bottom right corner
  // glReadPixels: 0,0 is bottom left corner, width,height is top right corner
  double mouse_x, mouse_y;
  glfwGetCursorPos(window, &mouse_x, &mouse_y);
  glfwGetWindowSize(window, &width, &height);
  int framebuffer_width, framebuffer_height;
  glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
  // convert mouse coordinates to framebuffer coordinates
  int framebuffer_x = mouse_x * (framebuffer_width / (double)width);
  int framebuffer_y =
      (height - mouse_y) * (framebuffer_height / (double)height);
  return {framebuffer_x, framebuffer_y};
}

bool Renderer::rendererIsReady(std::string *out_error) const {
  auto fail = [&](const std::string &msg) {
    if (out_error)
      *out_error = msg;
    return false;
  };

  if (!window)
    return fail("No GLFW window set.");
  if (!current_scene)
    return fail("No scene set.");
  if (shaders.size() < Shader_names::shader_count)
    return fail("Shaders array is not fully populated.");
  if (width <= 0 || height <= 0)
    return fail("Invalid screen dimensions.");
  if (element_detection_framebuffer_info.FBO == 0)
    return fail("Framebuffer not set up.");

  return true;
}