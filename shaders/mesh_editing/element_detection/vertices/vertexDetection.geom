#version 460 core


layout(std430, binding = 1) buffer DataBuffer {
int vertices_already_rendered[];
};

layout(triangles) in;
layout(points,max_vertices=3) out;

uniform uint vertex_offset;

flat in uint vertexID_out[];

flat out uint vertexID_frag;
/*i need to preprocess each triangle,
-compute if its a front-facing or back-facing triangle
-compare it with the array, if its not rendered:
    emit vertex and update the array
-else:
    dont emit anything
*/
void main(){
    //vertices already rendered

    vec4 vertices[3]={
        gl_in[0].gl_Position,
        gl_in[1].gl_Position,
        gl_in[2].gl_Position
        };

    //transform the vertices into 2D space
    vec2 NDC[3]={
        vertices[0].xy/vertices[0].w,
        vertices[1].xy/vertices[1].w,
        vertices[2].xy/vertices[2].w
    };
    //calculate 2 direction vectors coming out of first vertex
    vec2 direction1=NDC[1]-NDC[0];
    vec2 direction2=NDC[2]-NDC[0];

    //positive if triangle is front facing
    float winding=direction1.x*direction2.y-direction1.y*direction2.x;

    if(winding>0.0){
        for(int i=0;i<3;i++){
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
