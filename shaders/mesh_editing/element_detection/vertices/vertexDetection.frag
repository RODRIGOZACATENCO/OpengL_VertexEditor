#version 460 core

out uvec3 FragColor;
uniform uint MeshID;
flat in uint vertexID_frag;

uniform sampler2D main_depth_texture;

void main()
{
    vec2 uv = gl_FragCoord.xy / vec2(textureSize(main_depth_texture, 0));

    float scene_depth = texture(main_depth_texture, uv).r;

    float bias = 0.001;

    if ((gl_FragCoord.z - bias) > scene_depth) {
        discard;
    }

    vec2 point_coord = gl_PointCoord - vec2(0.5);
    if (dot(point_coord, point_coord) > 0.25) {
        discard;
    }

    uint outvertexID = vertexID_frag + 1u;
    FragColor = uvec3(outvertexID, MeshID, 0);
}