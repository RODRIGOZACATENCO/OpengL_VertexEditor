//
// Created by rodrigo on 04/05/2026.
//

#pragma once
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "GUI.h"
#include "Mesh.h"

// Scene class stores al relevant information of the meshes in the viewport,
// this includes
/*
 *all mesh data
 *model, view and projection matrices of the scene
 *render information of each mesh (VAO,VBO,EBO, etc )
 *
 */

struct RenderInfo {
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
  unsigned int edge_VAO;
  unsigned int edge_EBO;

  glm::mat4 model;
};
class Scene {
private:
  glm::mat4 view;
  glm::mat4 projection;
  glm::mat4 view_projection_matrix;
  std::vector<int> face_selection_array;
  std::vector<int> vertex_selection_array;
  std::vector<int> edge_selection_array;

  /*array that the vertex shaders use to determine wich vertices are already
drawn, because vertex pass uses GL_TRIANGLES to draw
*/
  unsigned int vertex_already_rendered_ssbo = 0;

  // it combines all vertices index to check if alreay rendered, of all meshes,
  // into a single vector index offset needed per mesh
  std::vector<int> vertex_already_rendered_array;

  unsigned int total_meshes = 0;
  std::map<Mesh *, RenderInfo> mesh_to_render_info;
  std::map<Mesh *, std::string> mesh_to_name;
  std::vector<std::unique_ptr<Mesh>> meshes;

  // array that the shaders use to determine what elements of the mesh are
  // currently selected
  unsigned int selected_elements_ssbo = 0;

public:
  bool sceneIsReady(std::string *out_error = nullptr) const;

  Scene(glm::mat4 view, glm::mat4 projection)
      : view(view), projection(projection) {
    view_projection_matrix = projection * view;
  }

  void addMesh(std::unique_ptr<Mesh> mesh, std::string name, glm::mat4 model) {
    Mesh *mesh_ptr = mesh.get();
    RenderInfo render_info;
    render_info.model = model;
    mesh_to_render_info[mesh_ptr] = render_info;
    meshes.push_back(std::move(mesh));
    mesh_to_name[mesh_ptr] = name;
    total_meshes++;
    meshRenderSetup(mesh_ptr);
    meshArraysSetup(mesh_ptr);
  }

  // Getters and Setters
  void setModelMatrix(Mesh *mesh, glm::mat4 model) {
    mesh_to_render_info[mesh].model = model;
  }

  void setViewMatrix(const glm::mat4 &view) {
    this->view = view;
    updateViewProjectionMatrix();
  }

  void setProjectionMatrix(const glm::mat4 &projection) {
    this->projection = projection;
    updateViewProjectionMatrix();
  }
  void updateViewProjectionMatrix() {
    view_projection_matrix = projection * view;
  }

  const glm::mat4 &getViewProjectionMatrix() const {
    return view_projection_matrix;
  }
  const glm::mat4 &getViewMatrix() const { return view; }
  const glm::mat4 &getProjectionMatrix() const { return projection; }

  const std::vector<std::unique_ptr<Mesh>> &getMeshes() const { return meshes; }

  RenderInfo getRenderInfo(Mesh *mesh) { return mesh_to_render_info[mesh]; }

  std::vector<int> *getFaceSelectionArray() { return &face_selection_array; }
  std::vector<int> *getVertexSelectionArray() {
    return &vertex_selection_array;
  }

  std::vector<int> *getEdgeSelectionArray() { return &edge_selection_array; }

  std::string getMeshName(Mesh *mesh) { return mesh_to_name[mesh]; }
  void meshArraysSetup(Mesh *mesh);
  void updateSelectionBuffer(GUIState state);
  void resetSelectionBuffer(GUIState type);
  void meshRenderSetup(Mesh *mesh);
  void updateFacesSelected(unsigned int face_id, unsigned int mesh_id);
  void updateVerticesSelected(unsigned int vertex_id, unsigned int mesh_id);
  void updateEdgesSelected(unsigned int edge_id, unsigned int mesh_id);
  void cleanup();
};
