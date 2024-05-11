#define FLT_MAX 3.402823466e+38
#define FLT_MIN 1.175494351e-38
#define DBL_MAX 1.7976931348623158e+308
#define DBL_MIN 2.2250738585072014e-308

uniform float t_min = 0.1;
uniform float t_max = 100.0;

struct Ray{
    vec3 origin;
    vec3 direction;
};

struct Sphere{
    vec3 origin;
    vec3 color;
    float radius;
};

struct HitInfo{
    vec3 pos;
    vec3 normal;
    bool front_face;
    float t;
};

vec3 rayAt(const Ray r, float t){
    return r.origin + r.direction * t;
}

vec3 sphereNormal(Sphere sphere, float t, Ray ray)
{
    return (rayAt(ray, t) - sphere.origin)/sphere.radius;
}

bool hitSphere(Sphere sphere, Ray ray, inout HitInfo hitinfo)
{
    vec3 center = sphere.origin;
    float radius = sphere.radius;
    vec3 oc = center - ray.origin;
    float a = 1 / dot(ray.direction, ray.direction);

    // GPU friendly version of PQ formula and simplified
    float doc = dot(ray.direction, oc);
    float p = doc * a;
    float q = (dot(oc, oc) - radius * radius) * a;
    float discriminant = (p * p) - q;

    if (discriminant <= 0)
    {
        return false;
    }

    float root = sqrt(discriminant);
    float t = doc - root;
    if(t <= t_min || t>= t_max){
        t = doc + root;
        if(t <= t_min || t>= t_max){
            return false;
        }
        hitinfo.t = t;
    }else{
        hitinfo.t = t;
    }
    hitinfo.pos = rayAt(ray, t);
    hitinfo.normal = sphereNormal(sphere, t, ray);
    if(dot(hitinfo.normal, ray.direction) < 0){
        hitinfo.front_face = true;
    }else{
        hitinfo.front_face = false;
        hitinfo.normal = -hitinfo.normal;
    }
    return true;
}


bool hit(Sphere sphere, Ray ray, inout HitInfo hitinfo){
    return hitSphere(sphere,ray,hitinfo);
}

struct Interval{
    float min;
    float max;
};

const Interval Universe = Interval(FLT_MIN, FLT_MAX);
const Interval Empty = Interval(FLT_MAX, FLT_MIN);

bool intervalContains(Interval interval, float x){
    return interval.min <= x && x <= interval.max;
}

bool intervalSurrounds(Interval interval, float x){
    return interval.min < x && x < interval.max;
}
