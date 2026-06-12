//
// Created by rodrigo on 27/04/2026.
//

#pragma once

#include <glm/glm.hpp>
#include <map>
#include <tuple>
#include <vector>

struct HalfEdge;
struct Vertex {
  glm::vec3 point;
  int halfedge;
  glm::vec3 normal; // average across all faces
};

struct Edge {
  int halfedge;
};

struct Face {
  int halfedge;
  glm::vec3 normal;
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
    computeFaceNormalVectors();
    computeVertexNormalVectors();
  }

  void process_mesh(std::vector<float> *vertices, std::vector<int> *faces);
  void show_mesh_structure();
  void setupFaceRenderIndices();
  void setupEdgeRenderIndices();
  void computeVertexNormalVectors();
  void computeFaceNormalVectors();

  // given an edge, will return the face indices of the faces the edge touches
  std::pair< int,  int> getFaceIndicesAssociatedWithEdge(unsigned int edge_index);

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
  const std::map<std::pair<int, int>, int> &getEdgeLookup() const{
    return  edge_lookup;
  }
  HalfEdge getHalfedgeFromIndex(unsigned int index) { return half_edges[index]; }
  Vertex getvVertexFromIndex(unsigned int index) { return vertices[index]; }
  Edge getEdgeFromIndex(unsigned int index) { return edges[index]; }


  std::tuple<unsigned int, unsigned int, unsigned int>
  vertexIndicesFromFace(Face face) {
    HalfEdge start_halfedge = half_edges[face.halfedge];
    unsigned int vertex1 = start_halfedge.vertex;
    start_halfedge = half_edges[start_halfedge.next];
    unsigned int vertex2 = start_halfedge.vertex;
    start_halfedge = half_edges[start_halfedge.next];
    unsigned int vertex3 = start_halfedge.vertex;
    return {vertex1, vertex2, vertex3};
  }

private:
  std::vector<HalfEdge> half_edges;
  std::vector<Face> faces;
  std::vector<Vertex> vertices;
  std::vector<Edge> edges;

  // given a pair of 2 vertices, returns the index the edge at those vertices
  // only one entry per edge ,so it can be (1,2) or (2,1)
  std::map<std::pair<int, int>, int> edge_lookup;
  std::vector<unsigned int> face_render_indices;
  std::vector<unsigned int> edge_render_indices;
  std::vector<glm::vec3> colors;
  glm::vec3 randomRGB();
};