//
// Created by rodrigo on 27/04/2026.
//

#pragma once
#include <glm/glm.hpp>
#include <map>
#include <vector>

struct HalfEdge;
struct Vertex {
  glm::vec3 point;
  int halfedge;
};

struct Edge {
  int halfedge;
};

struct Face {
  int halfedge;
};

struct HalfEdge {
  int next; // next half-edge
  int prev; // previous half edge
  int twin; // reversed half edge
  int vertex;
  int edge;
  int face;
};
class Mesh {
public:
  Mesh(std::vector<float> *vertices, std::vector<int> *faces) {
    process_mesh(vertices, faces);
    setupFaceRenderIndices();
    setupEdgeRenderIndices();
  }

  void process_mesh(std::vector<float> *vertices, std::vector<int> *faces);
  void show_mesh_structure();
  void setupFaceRenderIndices();
  void setupEdgeRenderIndices();

  // Getters
  const std::vector<HalfEdge> &getHalfEdges() const { return half_edges; }
  const std::vector<Face> &getFaces() const { return faces; }
  const std::vector<Vertex> &getVertices() const { return vertices; }
  const std::vector<Edge> &getEdges() const { return edges; }
  const std::vector<unsigned int> &getFaceRenderIndices() const {
    return face_render_indices;
  }
  const std::vector<unsigned int> &getEdgeRenderIndices() const {
    return edge_render_indices;
  }

private:
  std::vector<HalfEdge> half_edges;
  std::vector<Face> faces;
  std::vector<Vertex> vertices;
  std::vector<Edge> edges;
  std::map<std::pair<int, int>, int> edge_lookup;
  std::vector<unsigned int> face_render_indices;
  std::vector<unsigned int> edge_render_indices;
  std::vector<glm::vec3> colors;

  glm::vec3 randomRGB();
};