#version 330 core
layout(location = 0) in vec3 a_pos;

uniform mat4 model, view, projection;

out vec3 frag_pos;

void main(){
    frag_pos = vec3(model * vec4(a_pos, 1.0));

    gl_Position = projection * view * vec4(frag_pos, 1.0);
}