#version 450 core

#define PI 3.1415926535

layout(location=0) out vec4 fragColor;

layout(location=0) in vec2 texcoord;


struct Ray
{
    vec3 origin;
    vec3 direction;
};

struct Sphere
{
    vec3 origin;
    float radius;
};

struct Camera
{
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
};


uniform Camera camera;
uniform Sphere sphere;

Ray RayCtor(vec3 o,vec3 d)
{   
    Ray r;
    r.origin=o;
    r.direction=d;
    return r;
}

vec3 RayGetPointAt(Ray r,float t)
{
    return r.origin+r.direction*t;
}

Ray CameraGetRay(Camera camera,vec2 uv)
{
    Ray ray=RayCtor(camera.origin,camera.lower_left_corner+uv.x*camera.horizontal+uv.y*camera.vertical-camera.origin);
    return ray;
}

float SphereHit(Sphere sphere,Ray ray)
{
    vec3 oc=ray.origin-sphere.origin;

    float a=dot(ray.direction,ray.direction);
    float b=2.0*dot(oc,ray.direction);
    float c=dot(oc,oc)-sphere.radius*sphere.radius;

    float delta=b*b-4.0*a*c;
    if(delta<0)
        return -1.0; 
    return (-b-sqrt(delta))/(2.0*a);
}


vec3 RayColor(Ray ray)
{
   float t=SphereHit(sphere,ray);
   if(t>0.0)
    {
        vec3 n=normalize(RayGetPointAt(ray,t)-vec3(0.0,0.0,-1.0));
        return 0.5*n+0.5;
    }
    vec3 unit_direction=normalize(ray.direction);
    t=unit_direction.y*0.5+0.5;
    return (1.0-t)*vec3(1.0)+t*vec3(0.5,0.7,1.0);
}

void main()
{
    Ray r=CameraGetRay(camera,texcoord);   
    fragColor=vec4(RayColor(r),1.0);
}