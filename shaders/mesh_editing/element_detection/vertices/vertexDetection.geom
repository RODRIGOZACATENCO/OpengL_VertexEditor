#version 460 core

layout(std430, binding = 1) buffer DataBuffer {
    int vertices_already_rendered[];
};

uniform uint vertex_offset;
flat in uint vertexID_out[];
flat out uint vertexID_frag;

bool isPointInFrustum(vec4 pos) {
    if (pos.w <= 0.0001) return false;
    if (abs(pos.x) > pos.w) return false;
    if (abs(pos.y) > pos.w) return false;
    if (abs(pos.z) > pos.w) return false;
    return true;
}

layout(triangles) in;
layout(points, max_vertices=3) out;

void main() {
    for(int i = 0; i < 3; i++) {
        if(!isPointInFrustum(gl_in[i].gl_Position)) continue;

        uint currentID = vertexID_out[i];

        // Ensure we only emit each shared vertex once
        int wasRendered = atomicExchange(vertices_already_rendered[vertex_offset + currentID], 1);

        if(wasRendered == 0) {
            gl_Position = gl_in[i].gl_Position;
            vertexID_frag = currentID;
            gl_PointSize = gl_in[i].gl_PointSize;
            EmitVertex();
            EndPrimitive();
        }
    }
}