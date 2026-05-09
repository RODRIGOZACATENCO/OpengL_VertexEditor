#version 460 core

out uvec4 FragColor;
uniform uint MeshID;

void main()
{   
uint faceID = uint(gl_PrimitiveID) + 1u; // Add 1 to avoid using 0 as a valid ID
FragColor = uvec4(faceID, MeshID, 0, 0);

}