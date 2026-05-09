#version 460 core

layout(std430, binding = 0) buffer DataBuffer {
int faces_selected[];
};

uniform int num_faces_offset;

out vec4 FragColor;
in vec3 Barycentric;
flat in uint faceID;
void main() {

bool isSelected = faces_selected[num_faces_offset + int(faceID)] == 1;
float minBary = min(min(Barycentric.x, Barycentric.y), Barycentric.z);
float edgeThickness = fwidth(minBary) * 2;
float edgeFactor = smoothstep(0.0, edgeThickness, minBary);

vec4 wireColor = vec4(0.0, 1.0, 0.0, 1.0);
vec4 fillColor = isSelected ? vec4(0.8, 0.2, 0.2, 1.0) : vec4(0.2, 0.2, 0.2, 1.0);

FragColor = mix(wireColor, fillColor, edgeFactor);
}