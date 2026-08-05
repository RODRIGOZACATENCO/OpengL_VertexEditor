#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;//normal vector of each vertex
uniform mat4 model;
uniform mat4 view_projection;
uniform mat4 projection;
uniform mat4 view_matrix;
uniform mat3 normal_matrix;

flat out vec3 normal;

flat out uint vertexID_out;
void main()
{
    //hold the curent index of the EBO
    vertexID_out=gl_VertexID;

    normal=normalize(mat3(view_matrix)*normal_matrix*aNormal);

    gl_Position = view_projection*model*vec4(aPos, 1.0);

    gl_PointSize = gl_Position.w > 0.0 ? (180.0 / gl_Position.w * projection[1][1]) : 0.0;
}