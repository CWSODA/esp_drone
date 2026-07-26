#version 330 core

struct Material{
    sampler2D texture_diffuse0;
    sampler2D texture_specular0;

    float min_ambient;
    float shininess;
};

struct PointLight{
    vec3 ambient, diffuse, specular;
    vec3 pos;

    float quadratic, linear, constant;
};

struct DirLight{
    vec3 ambient, diffuse, specular;
    vec3 dir;
};

struct Spotlight{
    vec3 diffuse, specular;
    vec3 pos, dir;
    float inner_cos, outer_cos;
    float quadratic, linear, constant;
};

struct FragInfo{
    vec3 n_normal;
    vec3 n_view_dir;
    vec3 diff_map, spec_map;
};

vec3 calc_dir_light(DirLight light, FragInfo frag_info);
vec3 calc_point_light(PointLight light, FragInfo frag_info);
vec3 calc_spotlight(Spotlight light, FragInfo frag_info);

in vec3 frag_pos;
in vec2 tex_coord;
in vec3 normal;

uniform vec3 cam_pos;
uniform Material material;
uniform DirLight dir_light;
uniform Spotlight spotlight;

#define N_POINT_LIGHTS 4
uniform PointLight point_lights[N_POINT_LIGHTS];

out vec4 frag_color;

void main(){
    vec3 rosy = vec3(1.0,0.5,0.5);
    vec3 bluey = vec3(0.5,0.5,1.0);
    FragInfo frag_info;
    frag_info.n_normal = normalize(normal);
    frag_info.n_view_dir = normalize(frag_pos - cam_pos);
    frag_info.diff_map = texture(material.texture_diffuse0, tex_coord).rgb;
    frag_info.spec_map = texture(material.texture_specular0, tex_coord).rgb;

    vec3 light_output = calc_dir_light(dir_light, frag_info);
    light_output += calc_spotlight(spotlight, frag_info);

    for (int i = 0; i < N_POINT_LIGHTS; i++){
        light_output += calc_point_light(point_lights[i], frag_info);
    }

    frag_color = vec4(light_output, 1.0);
}

vec3 calc_dir_light(DirLight light, FragInfo frag_info){
    // light dir should point out from light
    vec3 n_light_dir = normalize(light.dir);
    
    vec3 ambient = light.ambient * frag_info.diff_map;

    vec3 diffuse = vec3(0.0), specular = vec3(0.0);

    // no specular unless light is incident on normal
    float diff = max(dot(-n_light_dir, frag_info.n_normal), 0.0);
    if (diff > 0){
        diffuse = light.diffuse * frag_info.diff_map * diff;

        vec3 reflect_dir = reflect(n_light_dir, frag_info.n_normal);
        float spec = max(dot(reflect_dir, -frag_info.n_view_dir), 0.0);
        specular = light.specular * frag_info.spec_map;
        specular *= pow(spec, material.shininess);
    }
    
    return (ambient + diffuse + specular);
}

vec3 calc_point_light(PointLight light, FragInfo frag_info){
    vec3 ambient = light.ambient * frag_info.diff_map;

    // points to light
    vec3 n_light_dir = normalize(light.pos - frag_pos);

    vec3 diffuse = vec3(0.0), specular = vec3(0.0);

    float diff = max(dot(n_light_dir, frag_info.n_normal), 0.0);
    if (diff > 0){
        diffuse = light.diffuse * frag_info.diff_map * diff;

        vec3 reflect_dir = reflect(-n_light_dir, frag_info.n_normal);
        float spec = max(dot(reflect_dir, -frag_info.n_view_dir), 0.0);
        specular =  light.specular * frag_info.spec_map;
        specular *= pow(spec, material.shininess);
    }

    float dist = length(light.pos - frag_pos);
    float attenuation = dist * dist * light.quadratic;
    attenuation += dist * light.linear + light.constant;
    attenuation = 1.0/attenuation;
    
    return (ambient + diffuse + specular) * attenuation;
}

vec3 calc_spotlight(Spotlight light, FragInfo frag_info){
    vec3 n_spot_dir = normalize(light.dir);
    // points out from normal
    vec3 n_light_dir = normalize(light.pos-frag_pos);

    float spot = dot(-n_light_dir, n_spot_dir);
    if (spot < light.outer_cos){
        return vec3(0.0);
    }

    if (spot > light.inner_cos){
        spot = 1.0;
    } else{
        spot = spot - light.outer_cos;
        spot /= light.inner_cos - light.outer_cos;
    }

    vec3 diffuse = vec3(0.0), specular = vec3(0.0);
    float diff = max(dot(n_light_dir, frag_info.n_normal), 0.0);
    if (diff > 0.0){
        diffuse = frag_info.diff_map * diff * light.diffuse;

        vec3 reflect_dir = reflect(-n_light_dir, frag_info.n_normal);
        float spec = max(dot(reflect_dir, -frag_info.n_view_dir), 0.0);
        specular = frag_info.spec_map * light.specular;
        specular *= pow(spec, material.shininess);
    }
    float dist = length(light.pos - frag_pos);
    float attenuation = dist * dist * light.quadratic;
    attenuation += dist * light.linear + light.constant;
    attenuation = 1.0/attenuation;
    attenuation *= spot;
    return (diffuse + specular) * attenuation;
}