#version 460 core
layout(lines) in;
layout(triangle_strip,max_vertices=4) out;
//recieves 2 vertices, needs to create a quad for each pair of points
//quad diagonal passing through the line formed by 2 vertices

flat out uint edgeID;

void main(){
    edgeID=gl_PrimitiveIDIn;

    //coordinates are in clip space(4d), they need to be in 2d space NDC 
    vec4 vertex1 = gl_in[0].gl_Position;
    vec4 vertex2 = gl_in[1].gl_Position;
    
    vec2 ndc1 = vertex1.xy / vertex1.w;
    vec2 ndc2 = vertex2.xy / vertex2.w;

    vec2 direction=ndc1-ndc2;
    vec2 perpendicular=normalize(vec2(direction.y,-direction.x));
    float thickness=0.05;

    vec2 offset1 = ndc1 + (thickness / 2) * perpendicular;
    vec2 offset2 = ndc1 - (thickness / 2) * perpendicular;

    vec2 offset3 = ndc2 + (thickness / 2) * perpendicular;
    vec2 offset4 = ndc2 - (thickness / 2) * perpendicular;
    
    gl_Position = vec4(offset1 * vertex1.w, vertex1.z, vertex1.w); EmitVertex();
    gl_Position = vec4(offset2 * vertex1.w, vertex1.z, vertex1.w); EmitVertex();
    gl_Position = vec4(offset3 * vertex2.w, vertex2.z, vertex2.w); EmitVertex();
    gl_Position = vec4(offset4 * vertex2.w, vertex2.z, vertex2.w); EmitVertex();
    EndPrimitive();
}