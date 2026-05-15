#version 460 core

layout(std430, binding = 0) buffer ElementSelection {
int elements_selected[];
};

uniform uint edges_offset;

out vec4 FragColor;
flat in uint edgeID;
void main() {

bool isEdgeSelected = elements_selected[edges_offset + int(edgeID)] == 1;
vec4 wireColor =vec4(0.0, 1.0, 0.0, 1.0);
if(isEdgeSelected){
     wireColor =vec4(0.576, 0.267, 0.788,1.0);
    
}
FragColor = wireColor;
}