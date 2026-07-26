#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 frag_input_normal;
out vec3 frag_pos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{   
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    frag_pos = vec3(model * vec4(aPos, 1.0));
    frag_input_normal = mat3(transpose(inverse(model)))*aNormal;
}