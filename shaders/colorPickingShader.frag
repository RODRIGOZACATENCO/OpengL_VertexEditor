#version 460 core


out uvec4 FragColor;
uniform uint MeshID;
flat in uint vertexID; // Received from vertex shader
flat in uint gui_state_out; // Received from vertex shader

void main()
{   
    switch(gui_state_out) {
        case 1: // Face picking mode
            uint faceID = uint(gl_PrimitiveID) + 1u; // Add 1 to avoid using 0 as a valid ID
            FragColor = uvec4(faceID, MeshID, 0, 0);
            break;
        case 2: // Vertex picking mode
            uint outvertexID = vertexID + 1u; // Add 1 to avoid using 0 as a valid ID
            FragColor = uvec4(outvertexID, MeshID, 0, 0);
            break;
    }

}