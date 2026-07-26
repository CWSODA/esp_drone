#version 330 core

struct Material{
    sampler2D diffuse_map;
    sampler2D specular_map;
    sampler2D emission_map;
    float shininess;
    float emission_brightness;
};

struct Light{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float inner_cutoff;
    float outer_cutoff;
    float quadratic, linear, constant;
};

in vec3 vs_normal;
in vec3 vs_frag_pos;
in vec3 vs_light_pos;
in vec3 vs_spot_dir;

in vec2 tex_coord;

out vec4 frag_color;

uniform Material material;
uniform Light light;

uniform bool has_spec_map;
uniform bool has_emission_map;

void main(){
    vec3 texture_color = vec3(texture(material.diffuse_map, tex_coord));
    float dist = length(vs_light_pos-vs_frag_pos);
    float attenuation = 1.0/(dist*dist*light.quadratic + dist*light.linear + light.constant);

    vec3 ambient = light.ambient * texture_color;

    vec3 emission = vec3(0.0);
    if (has_emission_map){
        emission = vec3(texture(material.emission_map, tex_coord));
        emission *= material.emission_brightness;
    }

    vec3 light_dir = normalize(vs_light_pos - vs_frag_pos);
    float cutoff_factor = 1.0;
    float cosine_val = dot(vs_spot_dir, -light_dir);
    if (cosine_val <= light.outer_cutoff){
        frag_color = vec4(ambient*attenuation+emission, 1.0);
        return;
    } else if (cosine_val <= light.inner_cutoff){
        cutoff_factor = cosine_val-light.outer_cutoff;
        cutoff_factor /= light.inner_cutoff-light.outer_cutoff;
    }

    vec3 normal = normalize(vs_normal);
    vec3 diffuse = light.diffuse * texture_color * max(dot(light_dir, normal), 0);

    vec3 view_dir = normalize(-vs_frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec_factor = dot(reflect_dir, view_dir);
    if (spec_factor <= 0.0){
        spec_factor = 0.0;
    } else{
        spec_factor = pow(spec_factor, material.shininess);
    }
    vec3 specular = light.specular * spec_factor;
    if (has_spec_map){
        specular *= texture(material.specular_map, tex_coord).rgb;
    }

    vec3 phong = (ambient+diffuse+specular);
    phong *= attenuation;
    phong *= cutoff_factor;
    frag_color = vec4(phong+emission, 1.0);
}