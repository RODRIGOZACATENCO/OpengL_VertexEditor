#version 460 core

layout(std430, binding = 0) buffer ElementSelection {
int elements_selected[];
};
uniform uint faces_offset;
out vec4 FragColor;
void main() {

bool isFaceSelected = elements_selected[faces_offset + int(gl_PrimitiveID)] == 1;
vec4 fillColor = isFaceSelected ? vec4(0.8, 0.2, 0.2, 1.0) : vec4(0.2, 0.2, 0.2, 1.0);

FragColor = fillColor;
}