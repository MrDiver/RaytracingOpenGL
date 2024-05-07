
struct Ray{
    vec3 origin;
    vec3 direction;
};

vec3 rayAt(const Ray r, float t){
    return r.origin + r.direction * t;
}
