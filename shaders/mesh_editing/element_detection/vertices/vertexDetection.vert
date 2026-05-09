#version 460 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view_projection;
uniform mat4 projection;

flat out uint vertexID;
void main()
{
    vertexID=gl_VertexID;
    gl_Position = view_projection*model*vec4(aPos, 1.0);
    
    gl_PointSize = 180 / gl_Position.w * projection[1][1];// Set point size for vertex picking

}