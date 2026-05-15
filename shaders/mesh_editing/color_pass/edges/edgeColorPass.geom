#version 460 core


struct EdgeNormals{
  vec4 face_normal_1;
  vec4 face_normal_2;
};

layout(std430,binding = 2) buffer NormalFacesAdjacent{
    EdgeNormals face_normals[];
};
uniform mat3 normal_matrix;


/*offset needed to read each mesh 
face normals from buffer*/
uniform uint edge_faces_normal_offset;


layout(lines) in;
layout(triangle_strip, max_vertices=4) out;

flat out uint edgeID;

/*transforms a line into a quad, to make it thicker*/
void main(){
    //transform and check face normals
    EdgeNormals en=face_normals[edge_faces_normal_offset+gl_PrimitiveIDIn];
    vec3 face_normal_1=normal_matrix*vec3(en.face_normal_1);
    vec3 face_normal_2=normal_matrix*vec3(en.face_normal_2);
    float epsilon=0.01;
    if((face_normal_1.z > -epsilon|| face_normal_1.z < epsilon)||
     face_normal_2.z > -epsilon|| face_normal_2.z < epsilon){//if any face front facing
        edgeID=gl_PrimitiveIDIn;
        //coordinates are in clip space(4d), they need to be in 2d space NDC 
        vec4 vertex1 = gl_in[0].gl_Position;
        vec4 vertex2 = gl_in[1].gl_Position;
        
        vec2 ndc1 = vertex1.xy / vertex1.w;
        vec2 ndc2 = vertex2.xy / vertex2.w;

        vec2 direction=ndc1-ndc2;
        vec2 perpendicular=normalize(vec2(direction.y,-direction.x));
        float thickness=0.01;

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
}