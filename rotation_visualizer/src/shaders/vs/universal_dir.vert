#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;

out vec3 frag_input_normal;
out vec3 viewspace_frag_pos;
out vec3 viewspace_light_dir;

out vec2 tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 light_dir;

void main()
{   
    gl_Position = projection * view * model * vec4(a_pos, 1.0);
    
    viewspace_frag_pos = vec3(view * model * vec4(a_pos, 1.0));
    // w = 0 since we only care about the direction, no translation
    viewspace_light_dir = vec3(view * vec4(light_dir, 0.0));
    frag_input_normal = mat3(transpose(inverse(view * model))) * a_normal;
    tex_coord = a_tex_coord;
}