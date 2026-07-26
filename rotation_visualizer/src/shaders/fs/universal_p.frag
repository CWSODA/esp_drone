#version 330 core

struct Material{
    sampler2D diffuse_map;
    sampler2D specular_map;
    sampler2D emission_map;
    float shininess;
};

struct Light{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float brightness;
    float quadratic, linear, constant;
};

in vec3 frag_input_normal;
in vec3 viewspace_frag_pos;
in vec3 viewspace_light_pos;

in vec2 tex_coord;

out vec4 frag_color;

uniform Material material;
uniform Light light;

uniform bool has_spec_map;
uniform bool has_emission_map;

void main(){
    vec3 texture_color = vec3(texture(material.diffuse_map, tex_coord));

    vec3 normal = normalize(frag_input_normal);

    vec3 ambient = light.ambient * texture_color;

    vec3 light_dir = normalize(viewspace_light_pos - viewspace_frag_pos);
    vec3 diffuse = light.diffuse * texture_color * max(dot(light_dir, normal), 0);

    vec3 view_dir = normalize(-viewspace_frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    vec3 specular = light.specular * pow(max(dot(reflect_dir, view_dir), 0.0), material.shininess);

    vec3 emission = vec3(0.0);
    if (has_spec_map){
        specular *= texture(material.specular_map, tex_coord).rgb;
    }
    if (has_emission_map){
        emission = vec3(texture(material.emission_map, tex_coord));
    }

    float dist = length(viewspace_light_pos-viewspace_frag_pos);
    float dist_falloff = 1.0/(dist*dist*quadratic + dist*b*linear + constant);
    vec3 color = (ambient+diffuse+specular+emission);
    frag_color = vec4(color*dist_falloff*light.brightness, 1.0);
}