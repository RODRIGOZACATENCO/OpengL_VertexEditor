#version 460 core

out vec4 FragColor;

layout(std430, binding = 0) buffer DataBuffer { 
    int vertex_selected[];
};
uniform int num_vertices_offset;
flat in uint vertexID;
void main()
{
    float radius = 0.3;
    if(vertex_selected[num_vertices_offset + int(vertexID)] == 1){

    /*substract 0.5 because gl_PointCoord gives the local coordinates of 
    all fragments inthe "square" created by renderizing a point, with
    coordinates (0,0) at top left corner and (1,1) at bottom-right corner
    */
    vec2 coord = gl_PointCoord - vec2(0.5);
    if(length(coord) > radius){
        discard;
    }
    FragColor = vec4(0.165, 0.871, 0.878,1.0);
    }
    else{
        discard;
    }
}