//
// Created by rodrigo on 04/05/2026.
//

#include "../include/Scene.h"

#include <glad/glad.h>
#include <glm/matrix.hpp>
#include <iostream>
#include <vector>
void Scene::updateFacesSelected(unsigned int face_id, unsigned int mesh_id) {
  unsigned int global_face_id = 0;
  for (int i = 0; i < mesh_id; i++) {
    global_face_id += meshes[i]->getFaces().size();
  }
  // change the state of the face in the global array
  face_selection_array[global_face_id + face_id] =
      !face_selection_array[global_face_id + face_id];
  updateSelectionBuffer(FACE_EDITING);
}

void Scene::updateVerticesSelected(unsigned int vertex_id,
                                   unsigned int mesh_id) {
  unsigned int global_id = 0;
  for (int i = 0; i < mesh_id; i++) {
    global_id += meshes[i]->getVertices().size();
  }
  // change the state of the vertex in the global array
  vertex_selection_array[global_id + vertex_id] =
      !vertex_selection_array[global_id + vertex_id];
  updateSelectionBuffer(VERTEX_EDITING);
}

void Scene::updateEdgesSelected(unsigned int edge_id, unsigned int mesh_id) {
  unsigned int global_id = 0;
  for (int i = 0; i < mesh_id; i++) {
    global_id += meshes[i]->getEdges().size();
  }

  // change the state of the edge in the global array
  edge_selection_array[global_id + edge_id] =
      !edge_selection_array[global_id + edge_id];
  updateSelectionBuffer(EDGE_EDITING);
}
void Scene::resetVertexAlreadyRendered() {
  std::fill(vertex_already_rendered_array.begin(),
            vertex_already_rendered_array.end(), 0);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertex_already_rendered_ssbo);
  glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,
                  vertex_already_rendered_array.size() * sizeof(int),
                  vertex_already_rendered_array.data());
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
void Scene::resetSelectionBuffer(GUIState type) {
  switch (type) {
  case FACE_EDITING:
    std::fill(face_selection_array.begin(), face_selection_array.end(), 0);
    updateSelectionBuffer(FACE_EDITING);
    break;
  case VERTEX_EDITING:
    std::fill(vertex_selection_array.begin(), vertex_selection_array.end(), 0);
    updateSelectionBuffer(VERTEX_EDITING);
    break;
  case EDGE_EDITING:
    std::fill(edge_selection_array.begin(), edge_selection_array.end(), 0);
    updateSelectionBuffer(EDGE_EDITING);
    break;
  default:
    break;
  }
}

void Scene::meshArraysSetup(Mesh *mesh) {
  // adds zeros to the end for each of these values of the mesh
  for (int i = 0; i < mesh->getFaces().size(); i++) {
    face_selection_array.push_back(0);
  }
  for (int i = 0; i < mesh->getVertices().size(); i++) {
    vertex_selection_array.push_back(0);

    vertex_already_rendered_array.push_back(0);
  }
  for (int i = 0; i < mesh->getEdges().size(); i++) {
    edge_selection_array.push_back(0);
  }

  // Setup the combined face selection array to color selection on the window
  std::vector<int> combined_selection_array;
  combined_selection_array.reserve(face_selection_array.size() +
                                   vertex_selection_array.size() +
                                   edge_selection_array.size());
  combined_selection_array.insert(combined_selection_array.end(),
                                  face_selection_array.begin(),
                                  face_selection_array.end());
  combined_selection_array.insert(combined_selection_array.end(),
                                  vertex_selection_array.begin(),
                                  vertex_selection_array.end());
  combined_selection_array.insert(combined_selection_array.end(),
                                  edge_selection_array.begin(),
                                  edge_selection_array.end());
  glGenBuffers(1, &selected_elements_ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, selected_elements_ssbo);

  // the buffer size increases each time a new mesh is added
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               combined_selection_array.size() * sizeof(int),
               combined_selection_array.data(), GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, selected_elements_ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

  glGenBuffers(1, &vertex_already_rendered_ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertex_already_rendered_ssbo);

  glBufferData(GL_SHADER_STORAGE_BUFFER,
               vertex_already_rendered_array.size() * sizeof(int),
               vertex_already_rendered_array.data(), GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vertex_already_rendered_ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

/*Setup all the meshes RenderInfo to show on the screen */
void Scene::meshRenderSetup(Mesh *mesh) {
  // generate VAO,VBO,EBO for the mesh
  RenderInfo *ri = &mesh_to_render_info[mesh];
  glGenVertexArrays(1, &ri->VAO);
  glGenVertexArrays(1, &ri->edge_VAO);
  glGenBuffers(1, &ri->VBO);
  glGenBuffers(1, &ri->EBO);
  glGenBuffers(1, &ri->edge_EBO);
  glGenBuffers(1, &ri->edge_VBO);
  // generates the VBO vector in render info
  auto vertices = mesh->getVertices();
  auto mesh_faces = mesh->getFaceRenderIndices();
  auto faceIndices = mesh->getFaceRenderIndices();

  // setup of VBO_information of each mesh
  for (int i = 0; i < vertices.size(); i++) {
    ri->VBO_information.push_back(vertices[i].point);
    ri->VBO_information.push_back(vertices[i].normal);
  }
  // send the vertex data to the GPU
  glBindBuffer(GL_ARRAY_BUFFER, ri->VBO);
  glBufferData(GL_ARRAY_BUFFER, ri->VBO_information.size() * sizeof(glm::vec3),
               ri->VBO_information.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ri->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               mesh->getFaceRenderIndices().size() * sizeof(unsigned int),
               mesh->getFaceRenderIndices().data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ri->edge_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               mesh->getEdgeRenderIndices().size() * sizeof(unsigned int),
               mesh->getEdgeRenderIndices().data(), GL_STATIC_DRAW);

  // setup face and vertex VAO
  glBindVertexArray(ri->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, ri->VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ri->EBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3),
                        (void *)0); // vertex information
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3),
                        (void *)(sizeof(glm::vec3))); // vertex information
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  // setup edge VAO and vbo
  // the edge detection needs to know wich edges are in view
  // data structure: (line,face1,face2), faces that touch that edge
  //(l1,l2,f11,f12,f13,f21,f22,f23), one per edge, with an EBO
  glBindVertexArray(ri->edge_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, ri->VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ri->edge_EBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, point));

  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
}

void Scene::updateSelectionBuffer(GUIState state) {
  switch (state) {
  case FACE_EDITING:

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, selected_elements_ssbo);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER,
                    0, // byte offset where to start
                    face_selection_array.size() * sizeof(int), // size in bytes
                    face_selection_array.data());              // new data
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    break;
  case VERTEX_EDITING:

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, selected_elements_ssbo);
    glBufferSubData(
        GL_SHADER_STORAGE_BUFFER,
        face_selection_array.size() * sizeof(int), // byte offset where to start
        vertex_selection_array.size() * sizeof(int), // size in bytes
        vertex_selection_array.data());              // new data
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    break;
  case EDGE_EDITING:

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, selected_elements_ssbo);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER,
                    face_selection_array.size() * sizeof(int) +
                        vertex_selection_array.size() *
                            sizeof(int), // byte offset where to start
                    edge_selection_array.size() * sizeof(int), // size in bytes
                    edge_selection_array.data());              // new data
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    break;
  default:
    break;
  }
}

glm::mat3 Scene::getNormalMatrixFromModel(glm::mat4 model_matrix) {
  return glm::transpose(glm::inverse(glm::mat3(model_matrix)));
}

void Scene::cleanup() {

  for (const auto &mesh : meshes) {
    Mesh *mesh_ptr = mesh.get();
    glDeleteVertexArrays(1, &mesh_to_render_info[mesh_ptr].VAO);
    glDeleteBuffers(1, &mesh_to_render_info[mesh_ptr].VBO);
    glDeleteBuffers(1, &mesh_to_render_info[mesh_ptr].EBO);
    glDeleteVertexArrays(1, &mesh_to_render_info[mesh_ptr].edge_VAO);
    glDeleteBuffers(1, &mesh_to_render_info[mesh_ptr].edge_EBO);
  }
  glDeleteBuffers(1, &selected_elements_ssbo);
}

bool Scene::sceneIsReady(std::string *out_error) const {
  auto fail = [&](const std::string &msg) {
    if (out_error)
      *out_error = msg;
    return false;
  };

  if (meshes.empty())
    return fail("No meshes in scene.");
  if (view == glm::mat4(0.0f))
    return fail("View matrix not set.");
  if (projection == glm::mat4(0.0f))
    return fail("Projection matrix not set.");
  if (selected_elements_ssbo == 0)
    return fail("Selection SSBO not set up.");

  for (const auto &mesh : meshes) {
    Mesh *mesh_ptr = mesh.get();
    if (!mesh_ptr)
      return fail("Null mesh found in scene.");

    auto it = mesh_to_render_info.find(mesh_ptr);
    if (it == mesh_to_render_info.end())
      return fail("Mesh missing render info.");

    const RenderInfo &info = it->second;
    if (info.VAO == 0)
      return fail("Mesh has invalid VAO.");
    if (info.VBO == 0)
      return fail("Mesh has invalid VBO.");
    if (info.EBO == 0)
      return fail("Mesh has invalid EBO.");
  }

  return true;
}