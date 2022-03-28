#version 450 core

#define PI 3.1415926535

layout(location=0) out vec4 fragColor;

layout(location=0) in vec2 texcoord;


struct Ray
{
    vec3 origin;
    vec3 direction;
};


struct Camera
{
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
};


uniform Camera camera;

Ray RayCtor(vec3 o,vec3 d)
{   
    Ray r;
    r.origin=o;
    r.direction=d;
    return r;
}

Ray CameraGetRay(Camera camera,vec2 uv)
{
    Ray ray=RayCtor(camera.origin,camera.lower_left_corner+uv.x*camera.horizontal+uv.y*camera.vertical-camera.origin);
    return ray;
}


vec3 RayColor(Ray ray)
{
    vec3 unit_direction=normalize(ray.direction);
    float t=unit_direction.y*0.5+0.5;
    return (1.0-t)*vec3(1.0)+t*vec3(0.5,0.7,1.0);
}

void main()
{
    Ray r=CameraGetRay(camera,texcoord);   
    fragColor=vec4(RayColor(r),1.0);
}