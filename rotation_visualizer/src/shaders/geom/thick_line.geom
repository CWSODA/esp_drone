#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec3 dir;

void draw_box(vec4 pos){
    float size = 1.0f;
    gl_Position = pos + vec4(size, size, 0, 0);
    EmitVertex();
    gl_Position = pos + vec4(-size, size, 0, 0);
    EmitVertex();
    gl_Position = pos + vec4(size, -size, 0, 0);
    EmitVertex();
    gl_Position = pos + vec4(-size, -size, 0, 0);
    EmitVertex();

    EndPrimitive();
}

void main(){
    draw_box(gl_in[0].gl_Position);
}
