#version 460 core

layout(std430, binding = 0) buffer ElementSelection {
int elements_selected[];
};
uniform uint faces_offset;
uniform uint current_rendering_mode;
const uint FACE_EDITING=1u;
out vec4 FragColor;
void main() {

bool isFaceSelected = elements_selected[faces_offset + int(gl_PrimitiveID)] == 1;
vec4 fillColor =vec4(0.2, 0.2, 0.2, 1.0);
if(isFaceSelected && current_rendering_mode==FACE_EDITING){
    fillColor=vec4(0.8, 0.2, 0.2, 1.0);

}
FragColor = fillColor;
}