#version 430

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
vec3 camera_center = vec3(0, 0, camera_z);

vec3 viewport_u = vec3(viewport_width, 0, 0);
vec3 viewport_v = vec3(0, -viewport_height, 0);
vec3 viewport_uv = viewport_u + viewport_v;
vec3 viewport_upleft = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
vec3 pixel_size = vec3(1/window_width, 1/window_height, 0);

uniform int numSpheres = 0;
uniform int max_ray_reflections = 10;
uniform int samples = 1;

int _r = 0;
float random_float(){
    _r++;
    return random(vec2(f_uv) + _r);
}

float random_minmax(float min, float max){
    return random_float()*(max-min)+min;
}

vec3 random_vec3(float min, float max){
    return vec3(random_minmax(min, max), random_minmax(min, max), random_minmax(min, max));
}

vec3 random_on_hemisphere(const vec3 normal) {
    vec3 on_unit_sphere = normalize(random_vec3(-1, 1));
    if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

layout(std430, binding = 3) buffer sphereBuffer
{
    Sphere spheres[];
};

void getWorldHit(const Ray ray, inout HitInfo hitinfo, inout int index){
    HitInfo lastHitInfo;
    int lastIndex = -1;
    lastHitInfo.t = t_max;

    for (int i = 0; i < numSpheres; i++)
    {
        Sphere sphere = spheres[i];
        bool isHit = hit(sphere, ray, Interval(t_min, lastHitInfo.t), hitinfo);
        if (isHit && hitinfo.t <= lastHitInfo.t)
        {
            lastHitInfo = hitinfo;
            lastIndex = i;
        }
    }
    hitinfo = lastHitInfo;
    index = lastIndex;
}

HitInfo hitinfo;
vec3 colorAcc;
float factor;
Ray ray;

vec3 rayColor(Ray iray)
{
    colorAcc = vec3(1);
    factor = 1.0;
    ray = iray;
    for(int step = 0; step < max_ray_reflections; step++){
        float lastDistance = t_max;
        int sphereIdx = -1;

        getWorldHit(ray, hitinfo, sphereIdx);

        if(hitinfo.t < t_max){
            if(step >= max_ray_reflections -1){
                colorAcc = vec3(0);
            }else{
                ray = Ray(hitinfo.pos, random_on_hemisphere(hitinfo.normal));
            }
        }else{
            float a = 0.5 * (ray.direction.y + 1.0);
            vec3 tmp = (1.0 - a) * vec3(1.0, 1.0, 1.0) + a * vec3(0.5, 0.7, 1.0);
            colorAcc = tmp;
            break;
        }
        factor *= 0.5;
    }
    return factor * colorAcc;
}

void main()
{
    vec3 accumulatedColor = vec3(0);

    for(int i = 0; i < samples; i++){
        // float rand = random_float();
        float rand = random_float();
        vec2 offset = vec2(rand/window_width, rand/window_height);
        vec3 pixel_center = vec3(f_uv.xy + offset, 0.0) * viewport_uv + viewport_upleft;
        vec3 dir = normalize(pixel_center - camera_center);
        Ray r = Ray(camera_center, dir+pixel_size);
        accumulatedColor += rayColor(r)/samples;
    }

    FragColor = vec4(accumulatedColor, 1.0);
    // FragColor = vec4(vec3(random_float()), 1.0);
    // FragColor = vec4(random_vec3(-1.0, 1.0), 1.0);
}
