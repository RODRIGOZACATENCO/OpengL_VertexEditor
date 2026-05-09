#version 460 core


out uvec4 FragColor;
uniform uint MeshID;
flat in uint vertexID; // Received from vertex shader

void main()
{   
    uint outvertexID = vertexID + 1u; // Add 1 to avoid using 0 as a valid ID
    FragColor = uvec4(outvertexID, MeshID, 0, 0);

}    