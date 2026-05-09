#version 460 core


layout(std430, binding = 1) buffer DataBuffer {
int vertices_already_rendered[];
};

uniform uint vertex_offset;


layout(triangles) in;
layout(vertices,max_vertices=3) out;

/*i need to preprocess each triangle,
-compute if its a front-facing or back-facing triangle
-compare it with the array, if its not rendered:
    emit vertex and update the array
-else:
    dont emit anything
*/
int main(){
    vec4 vertices[3]={
        gl_in[0].gl_Position,
        gl_in[1].gl_Position,
        gl_in[2].gl_Position
        }

    //transform the vertices into 2D space
    vec2 NDC[3]={
        vertices[1].xy/vertices[0].w,
        vertices[1].xy/vertices[1].w,
        vertices[1].xy/vertices[2].w
    }
    //calculate 2 direction vectors coming out of first vertex
    direction1=NDC[1]-NDC[0];
    direction2=NDC[2]-NDC[0];

    //positive if triangle is front facing
    winding=direction1.x*direction2.y-direction1.y*direction2.x;

    if(winding){
        
    }
}
