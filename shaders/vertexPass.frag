#version 460 core


out vec4 FragColor;

flat in uint vertexID; // Received from vertex shader


layout(std430, binding = 0) buffer DataBuffer { 
    int vertex_selected[];
};
uniform int num_vertices_offset;
void main()
{
    float radius = 0.3;
    if(vertex_selected[num_vertices_offset + gl_PrimitiveID] == 1){
    vec2 coord = gl_PointCoord - vec2(0.5);
    if(length(coord) > radius){
        discard;
    }
    FragColor = vec4(0.8, 0.2, 0.2, 1.0);
    }
    else{
        discard;
    }
}