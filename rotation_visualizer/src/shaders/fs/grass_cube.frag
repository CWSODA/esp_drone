#version 330 core

in vec3 local_pos;
out vec4 FragColor;

void main(){
    // Settings
    vec4 top_color = vec4(0, 0.6, 0, 1.0);
    vec4 bottom_color = vec4(156/256.0, 96/256.0, 12/256.0, 1.0);
    float top = 0.8;
    float bottom = 0.7;
    float edge_thickness = 0.005;

    // remap to 0-->1
    float yaxis = local_pos.y + 0.5;

    float mix_strength;
    if (yaxis > top) {mix_strength = 1;}
    else if (yaxis < bottom) {mix_strength = 0;}
    else {
        mix_strength = (yaxis - bottom)/(top-bottom);
    }

    if (((local_pos.x > 0.5 - edge_thickness || local_pos.x < -0.5 + edge_thickness) 
        && (local_pos.y > 0.5 - edge_thickness || local_pos.y < -0.5 + edge_thickness))
        || ((local_pos.x > 0.5 - edge_thickness || local_pos.x < -0.5 + edge_thickness) 
        && (local_pos.z > 0.5 - edge_thickness || local_pos.z < -0.5 + edge_thickness))
        || ((local_pos.z > 0.5 - edge_thickness || local_pos.z < -0.5 + edge_thickness) 
        && (local_pos.y > 0.5 - edge_thickness || local_pos.y < -0.5 + edge_thickness))
    ){
        FragColor = vec4(0.0, 0.0, 0.0, 1);
    }else {
        FragColor = mix(bottom_color, top_color, mix_strength);
    }
}