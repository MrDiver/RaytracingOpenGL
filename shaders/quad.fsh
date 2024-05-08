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
vec3 camera_center = vec3(0, 0, camera_z);

vec3 viewport_u = vec3(viewport_width, 0, 0);
vec3 viewport_v = vec3(0, -viewport_height, 0);
vec3 viewport_uv = viewport_u + viewport_v;
vec3 viewport_upleft = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;

float hitSphere(vec3 center, float radius, Ray ray)
{
    vec3 oc = center - ray.origin;
    float a = 1 / dot(ray.direction, ray.direction);
#ifdef USE_QUADRATIC_FORMULA
    float b = -2.0 * dot(ray.direction, oc);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
#else
    // GPU friendly version of PQ formula
    float p = -2 * dot(ray.direction, oc) * a;
    float q = (dot(oc, oc) - radius) * a;
    float discriminant = (p * p * 0.25) - q;
#endif

    if (discriminant <= 0)
    {
        return -1.0;
    }

#ifdef USE_QUADRATIC_FORMULA
    return (-b - sqrt(discriminant)) / (2.0 * a);
#else
    return -p * 0.5 - sqrt(discriminant);
#endif
}

vec3 sphereNormal(vec3 center, float t, Ray ray)
{
    return normalize(rayAt(ray, t) - center);
}

vec3 rayColor(Ray ray)
{
    float s1 = hitSphere(vec3(0, 0, -1), 0.5, ray);
    if (s1 > 0.0)
    {
        return (sphereNormal(vec3(0, 0, -1), s1, ray) + vec3(1)) * 0.5;
    }
    float s2 = hitSphere(vec3(-2, 0, -2), 0.5, ray);
    if (s2 > 0.0)
    {
        return (sphereNormal(vec3(-2, 0, -2), s2, ray) + 1) * 0.5;
    }
    float a = 0.5 * (ray.direction.y + 1.0);
    return (1.0 - a) * vec3(1.0, 1.0, 1.0) + a * vec3(0.5, 0.7, 1.0);
}

void main()
{
    vec3 pixel_center = vec3(f_uv.xy, 0.0) * viewport_uv + viewport_upleft;
    vec3 dir = normalize(pixel_center - camera_center);
    Ray r = Ray(camera_center, dir);

    FragColor = vec4(rayColor(r), 1.0);
    // FragColor = vec4(f_uv.xy, 0.0, 1.0);
}
