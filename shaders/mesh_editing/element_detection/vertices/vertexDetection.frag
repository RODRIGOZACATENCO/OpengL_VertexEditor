#version 460 core


out uvec3 FragColor;
uniform uint MeshID;
flat in uint vertexID_frag; // Received from geom shader

void main()
{   
    uint outvertexID = vertexID_frag + 1u; // Add 1 to avoid using 0 as a valid ID
    FragColor = uvec3(outvertexID, MeshID, 0);

}    