//
// Created by rodrigo on 27/04/2026.
//

#include<Mesh.h>
#include <iostream>
#include <random>
#include <ranges>
#include<glm/glm.hpp>

#include "ShaderHandler.h"
//given an array of vertices to form triangles, and grupos of 3 indices that from a face
//we need to create the mesh structure
void Mesh::process_mesh(std::vector<float> *input_vertices, std::vector<int> *input_faces) {
	int current_face_index=0,current_halfedge_index=0,current_edge_index=0;
	//preprocess all vertices
	for (int i=0;i<input_vertices->size();i+=3) {
		Vertex vertex;
		vertex.point=glm::vec3((*input_vertices)[i],(*input_vertices)[i+1],(*input_vertices)[i+2]);
		vertex.halfedge=-1;
		vertices.push_back(vertex);
	}

	for (int i=0;i<input_faces->size();i+=3) {
		//first create the face that will represent 3 vertices
		Face face;
		//extract the indices of the vertices that form the face i
		//index of vertices that form the face e.g. {0,1,2} vertex 0 ,vertex 1, vertex 2
		int vertex_indices[3]={(*input_faces)[i],(*input_faces)[i+1],(*input_faces)[i+2]};

		//create the 3 halfedges of the face and fill them with basic data;
		for (int j=0;j<3;j++) {
			//create the halfedge
			HalfEdge half_edge;
			half_edge.vertex=vertex_indices[j];//assign vertex
			half_edge.face=current_face_index;//assign face
			if (vertices[vertex_indices[j]].halfedge==-1)
				vertices[vertex_indices[j]].halfedge=current_halfedge_index+j;

			half_edges.push_back(half_edge);//save the created half edges
		}

		//link face with the first of the half edges
		face.halfedge=current_halfedge_index;

		//3 new edge objects created, halfedge_index represents the first of the 3 vertices
		//link all 3 half edges
		for (int j=0;j<3;j++) {//assign next,prev,create edges, check for twin
			half_edges[current_halfedge_index+j].next=current_halfedge_index+(j+1)%3;//assign next
			half_edges[current_halfedge_index+j].prev=current_halfedge_index+(j+2)%3;//assign prev
			/*first we need to check if halfedge twin exists
			-if twin exists:
			 	assign twin to current half edge
				assign current half_edge(twin of the twin halfedge) to the twin
				assign twin's edge to the current half edge
			 -if twin doesn't exist:
				create an edge,assign the current half edge to it
				assign edge to the current half edge
				assign -1 to twin
				save new edge on the main vector
			 */
			int current_vertex=vertex_indices[j], next_vertex=vertex_indices[(j+1)%3];
			//twin exists, csecond=model;heck if there's an edge already in the opposite direction
			//if it exists ,it's halfedge must be the twin half edge
			if (edge_lookup.contains({next_vertex,current_vertex})) {

				int twin_halfedge_index=edges[edge_lookup.at({next_vertex,current_vertex})].halfedge;//twin_index on main edge vector
				half_edges[current_halfedge_index+j].twin=twin_halfedge_index;//assign twin to halfedge
				half_edges[twin_halfedge_index].twin=current_halfedge_index+j;//assign half edge to twin
					half_edges[current_halfedge_index+j].edge=half_edges[twin_halfedge_index].edge;//assign twin's edge to current halfedge
			}
			else {
				Edge edge;//create half edge
				edge.halfedge=current_halfedge_index+j;
				half_edges[current_halfedge_index+j].twin=-1;
				half_edges[current_halfedge_index+j].edge=current_edge_index;
				edges.push_back(edge);
				edge_lookup.insert({{current_vertex,next_vertex},current_edge_index++});
			}
		}
		current_halfedge_index+=3;
		current_face_index++;
		faces.push_back(face);
	}
}

void Mesh::show_mesh_structure() {
	std::cout<<"EDGES: "<<edges.size()<<std::endl;
	int i=0;
	for (const auto &key: edge_lookup | std::views::keys) {
		std::cout<<"edge :"<<i++<<"  "<<key.first<<"-->"<<key.second<<std::endl;
	}
	std::cout<<std::endl<<std::endl;
	for (int i =0;i<faces.size();i++) {
		int halfedge_index=faces[i].halfedge;
		std::cout<<"Face: "<<i<<std::endl;
		for (int j=0;j<3;j++) {
			HalfEdge half_edge=half_edges[halfedge_index] ;
			std::cout<<"	Halfedge: "<<j+3*i<<std::endl;
			std::cout<<"		vertex: "<<half_edge.vertex<<std::endl;
			std::cout<<"		edge: "<<half_edge.edge<<std::endl;
			std::cout<<"		face: "<<i<<std::endl;
			std::cout<<"		next: "<<half_edge.next<<std::endl;
			std::cout<<"		prev: "<<half_edge.prev<<std::endl;
			std::cout<<"		twin:"<<half_edge.twin<<std::endl;
			halfedge_index=half_edge.next;
		}
	}
	for (auto index:face_render_indices) {
		std::cout<<index<<" ";
	}
}


void Mesh::setupFaceRenderIndices() {
	//need to extract the order in witch to draw the triangles
	face_render_indices.clear();
	for (auto face:faces) {
		//for each face, extract the 3 vertices that form it
		int halfedge_index=face.halfedge;
		for (int i=0;i<3;i++) {
			face_render_indices.push_back(half_edges[halfedge_index].vertex);
			halfedge_index=half_edges[halfedge_index].next;
		}
	}

}

void Mesh::setupEdgeRenderIndices() {
	edge_render_indices.clear();
	for (auto edge:edges) {
		int halfedge_index=edge.halfedge;
		edge_render_indices.push_back(half_edges[halfedge_index].vertex);
		edge_render_indices.push_back(half_edges[half_edges[halfedge_index].next].vertex);
	}
	for(auto i:edge_render_indices)std::cout<<i<<" ";
	std::cout<<std::endl;
}
glm::vec3 Mesh::randomRGB() {
	// 1. Set up the random number generator (do this statically so it's only seeded once)
	static std::random_device rd;  // Hardware seed
	static std::mt19937 gen(rd()); // Mersenne Twister engine

	// 2. Define the distribution range (0.0 to 1.0 for OpenGL)
	std::uniform_real_distribution<float> dis(0.0f, 1.0f);

	// 3. Generate the RGB channels
	float r = dis(gen);
	float g = dis(gen);
	float b = dis(gen);

	return glm::vec3(r, g, b);
}
