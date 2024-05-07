#version 330

#include "common.glsl"

in vec3 f_pos;
in vec2 f_uv;
out vec4 FragColor;

uniform float window_width = 800.0;
uniform float window_height = 400.0;
float aspect_ratio = window_width / window_height;

uniform float viewport_height = 2.0;
float viewport_width = viewport_height * aspect_ratio;

uniform float focal_length = 1.0;
uniform float camera_z = 1.0;
vec3 camera_center = vec3(0,0,camera_z);

vec3 viewport_u = vec3(viewport_width, 0, 0);
vec3 viewport_v = vec3(0, -viewport_height, 0);
vec3 viewport_uv = viewport_u + viewport_v;
vec3 viewport_upleft = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;

bool hitSphere(vec3 center, float radius, Ray r) {
    vec3 oc = center - r.origin;
    float a = dot(r.direction, r.direction);
    float b = -2.0 * dot(r.direction, oc);
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    return (discriminant >= 0);
}

vec3 rayColor(Ray r){
    vec3 dir = normalize(r.direction);
    if(hitSphere(vec3(0,0,-1), 0.5, r)){
        return vec3(1.0,0,0);
    }
    if(hitSphere(vec3(-2,0,-2), 0.5, r)){
        return vec3(0,1.0,0);
    }
    float a = 0.5*(dir.y + 1.0);
    return (1.0-a)*vec3(1.0, 1.0, 1.0) + a*vec3(0.5, 0.7, 1.0);
}


void main(){
    vec3 pixel_center = vec3(f_uv.xy, 0.0) * viewport_uv + viewport_upleft;
    vec3 dir = pixel_center - camera_center;
    Ray r = Ray(camera_center, dir);

    FragColor = vec4(rayColor(r), 1.0);
    // FragColor = vec4(f_uv.xy, 0.0, 1.0);
}
