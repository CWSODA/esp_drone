#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;

// viewspace vectors
out vec3 vs_normal;
out vec3 vs_frag_pos;
out vec3 vs_light_pos;
out vec3 vs_spot_dir;

out vec2 tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 light_pos;
uniform vec3 spot_dir;

void main()
{   
    gl_Position = projection * view * model * vec4(a_pos, 1.0);
    
    vs_frag_pos = vec3(view * model * vec4(a_pos, 1.0));
    vs_light_pos = vec3(view * vec4(light_pos, 1.0));
    vs_spot_dir = normalize(vec3(view * vec4(spot_dir, 0.0)));
    vs_normal = mat3(transpose(inverse(view * model))) * a_normal;
    tex_coord = a_tex_coord;
}