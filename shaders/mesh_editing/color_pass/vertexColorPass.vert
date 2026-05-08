#version 460 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform uint gui_state; // 1 for face picking, 2 for vertex picking
flat out uint vertexID; // Pass vertex ID to geometry shader
flat out uint gui_state_out; // Pass GUI state to geometry shader
void main()
{
    
    gl_Position = projection*view*model*vec4(aPos, 1.0);
    switch(gui_state)
    {
        case 2: // Vertex picking mode
            gl_PointSize = 150 / gl_Position.w * projection[1][1];// Set point size for vertex picking
            break;
    }

    vertexID = gl_VertexID; // Pass vertex ID to geometry shader
    gui_state_out = gui_state; // Pass GUI state to geometry shader

}