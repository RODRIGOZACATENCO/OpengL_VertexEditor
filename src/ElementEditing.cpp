#include "ElementEditingRenderer.h"


/*calculates the 3D position of a selected point in the screen to move
 *vertex selected to that position
*/
void ElementEditing::vertexRayCaster(glm::vec2 mouse_pos,glm::vec3 camera_pos,glm::vec3 camera_front){

  //mouse position given in NDC coordinates
  //prepare the vector for un-projection, clip space
  glm::vec4 world_pos={mouse_pos.x,mouse_pos.y,-1,1};
  world_pos=inverse_view_projection_matrix*world_pos;
  world_pos/=world_pos.w;//position of the mouse in 3d space

  glm::vec3 ray_direction=glm::normalize(glm::vec3(world_pos)-camera_pos);
  Mesh *selected_mesh=current_scene->getMeshes()[std::get<1>(selected_element)].get();
  Vertex selected_vertex=selected_mesh->getvVertexFromIndex(std::get<2>(selected_element));
  glm::vec3 vertex_pos=selected_vertex.point;

  //normal direction of the plane, the plane must be parallel to the camera window
  glm::vec3 plane_normal=glm::normalize(camera_pos-camera_front);

  //ray formula given by R(t)=O+t*D
  float t=glm::dot(plane_normal,(vertex_pos-camera_pos))/(glm::dot(plane_normal,ray_direction));
  glm::vec3 intersection_point =camera_pos+t*ray_direction;//position to move the vertex into
  std::cout<<intersection_point.x<<" "<<intersection_point.y<<" "<<intersection_point.z<<" "<<std::endl;
  current_scene->updateVertexPos(std::get<1>(selected_element),std::get<2>(selected_element),intersection_point);
}