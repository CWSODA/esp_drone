#version 330 core

struct Material{
    vec3 diffuse;
    float shininess;
};

struct Light{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 frag_input_normal;
in vec3 viewspace_frag_pos;
in vec3 viewspace_light_pos;

out vec4 FragColor;

uniform Material material;
uniform Light light;

void main(){
    vec3 normal = normalize(frag_input_normal);

    vec3 ambient = light.ambient * material.diffuse;

    vec3 light_dir = normalize(viewspace_light_pos - viewspace_frag_pos);
    vec3 diffuse = light.diffuse * material.diffuse * max(dot(light_dir, normal), 0);

    vec3 view_dir = normalize(-viewspace_frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    vec3 specular = light.specular * pow(max(dot(reflect_dir, view_dir), 0.0), material.shininess);

    FragColor = vec4((ambient+diffuse+specular), 1.0);
}