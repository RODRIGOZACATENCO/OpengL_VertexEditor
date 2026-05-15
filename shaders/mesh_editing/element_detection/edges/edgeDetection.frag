#version 460 core
out uvec4 FragColor;

flat in uint edgeID;
uniform uint MeshID;
void main()
{

    FragColor=uvec4(edgeID+1u,MeshID,0,1);

}