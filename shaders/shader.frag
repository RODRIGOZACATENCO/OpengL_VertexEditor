#version 460 core


out vec4 FragColor;

flat in uint faceID;
in vec3 Barycentric;

layout(std430, binding = 0) buffer DataBuffer { 
    int faces_selected[];
};
uniform int num_faces_offset;//offset to access the correct part of the faces_selected array for the current mesh
void main()
{
    // Simple color interpolation based on barycentric coordinates
    float minBary = min(min(Barycentric.x, Barycentric.y), Barycentric.z);
    float edgeThickness = fwidth(minBary) * 2;
    float edgeFactor = smoothstep(0.0, edgeThickness, minBary);

    vec4 wireColor = vec4(0.0, 1.0, 0.0, 1.0);
    vec4 fillColor = vec4(0.2, 0.2, 0.2, 1.0);
    if(faces_selected[num_faces_offset + int(faceID)] == 1){
        fillColor = vec4(0.8, 0.2, 0.2, 1.0);
    }
    FragColor = mix(wireColor, fillColor, edgeFactor);
}