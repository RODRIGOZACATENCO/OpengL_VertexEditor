#version 460 core


layout(std430, binding = 1) buffer DataBuffer {
int vertices_already_rendered[];
};

layout(triangles) in;
layout(points,max_vertices=3) out;


flat in vec3 normal[];//normal vectors of each vertex

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

void main(){

    vec4 vertices[3]={
        gl_in[0].gl_Position,
        gl_in[1].gl_Position,
        gl_in[2].gl_Position
        };

vec3 faceNormal=normalize(normal[0]+normal[1]+normal[2]);

for(int i=0;i<3;i++){
    if(faceNormal.z>0.0 && isPointInFrustum(vertices[i])){
        uint currentID=vertexID_out[i];
        //updates the buffer so that vertex doesnt get rendered again
        int wasRendered = atomicExchange(vertices_already_rendered[vertex_offset+currentID], 1);
        if (wasRendered == 0) {
            gl_Position = vertices[i];
            vertexID_frag = currentID;
            gl_PointSize = gl_in[i].gl_PointSize;
            EmitVertex();
            EndPrimitive();
        }
    }
}

}
