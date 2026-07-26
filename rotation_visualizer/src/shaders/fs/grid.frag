#version 330 core

in vec3 frag_pos;

uniform float grid_size;
uniform float line_width;

out vec4 frag_color;

void main(){
    vec3 color = vec3(0.7);
    // get decimal value
    float scale = 1.0;
    float pos_x = frag_pos.x * scale;
    float pos_z = frag_pos.z * scale;

    float fx = pos_x - floor(pos_x);
    float fz = pos_z - floor(pos_z);
    float lower_threshold = line_width;
    float upper_threshold = 1 - lower_threshold;
    if (fx < lower_threshold || fz < lower_threshold
        || fx > upper_threshold || fz > upper_threshold){
        color = vec3(0.1);
    }
    frag_color = vec4(fx, 0.0, fz, 1.0);
    frag_color = vec4(color, 1.0);
}