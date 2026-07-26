#version 330 core

in vec3 frag_input_normal;
in vec3 frag_pos;

out vec4 frag_color;

uniform vec3 object_color;
uniform vec3 light_color;

uniform vec3 light_pos;
uniform vec3 view_pos;

void main(){
    vec3 normal = normalize(frag_input_normal);

    float ambient = 0.1;

    vec3 light_dir = normalize(light_pos - frag_pos);
    float diffuse = max(dot(light_dir, normal), 0);

    float specular_strength = 0.5;
    float shininess = 2^5;
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float specular = pow(max(dot(reflect_dir, view_dir), 0.0), shininess) * specular_strength;

    frag_color = vec4(light_color*object_color*(ambient+diffuse+specular), 1.0);
}