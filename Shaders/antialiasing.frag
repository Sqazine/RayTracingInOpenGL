#version 450 core

#define PI 3.1415926535

#define OBJECT_COUNT 2

layout(location=0) out vec4 fragColor;

layout(location=0) in vec2 texcoord;

uniform int spp;

//utilities
uniform vec4 rdSeed;
int rdCnt = 0;
float Rand(float x, float y){
	return fract(cos(x * (12.9898) + y * (4.1414)) * 43758.5453);
}
float Rand(){
	float a = Rand(texcoord.x, rdSeed.x);
    float b = Rand(rdSeed.y, texcoord.y);
    float c = Rand(rdCnt++, rdSeed.z);
    float d = Rand(rdSeed.w, a);
    float e = Rand(b, c);
    float f = Rand(d, e);

    return f;
}

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

struct HitRecord
{
    float t;
    vec3 position;
    vec3 normal;
    bool frontFace;
};

void SetFrontFace(inout HitRecord hitRecord,Ray ray,vec3 outward_normal)
{
    hitRecord.frontFace=dot(ray.direction,outward_normal)<0;
    hitRecord.normal=hitRecord.frontFace?outward_normal:-outward_normal;
}

struct World
{
    Sphere[OBJECT_COUNT] objects;
};

uniform Camera camera;
uniform Sphere sphere;
uniform World world;
uniform vec2 textureExtent;


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

bool SphereHit(Sphere sphere,Ray ray,float t_min,float t_max,inout HitRecord hitRecord)
{
    vec3 oc=ray.origin-sphere.origin;

    float a=dot(ray.direction,ray.direction);
    float b=2.0*dot(oc,ray.direction);
    float c=dot(oc,oc)-sphere.radius*sphere.radius;

    float delta=b*b-4.0*a*c;
  
    if(delta>0)
    {
        float temp=(-b-sqrt(delta))/(2.0*a);
        if(temp<t_max&&temp>t_min)
        {
            hitRecord.t=temp;
            hitRecord.position=RayGetPointAt(ray,hitRecord.t);
            vec3 normal=(hitRecord.position-sphere.origin)/sphere.radius;
            SetFrontFace(hitRecord,ray,normal);
            return true;
        }

        temp=(-b+sqrt(delta))/(2.0*a);
        if(temp<t_max&&temp>t_min)
        {
            hitRecord.t=temp;
            hitRecord.position=RayGetPointAt(ray,hitRecord.t);
            vec3 normal=(hitRecord.position-sphere.origin)/sphere.radius;
            SetFrontFace(hitRecord,ray,normal);
            return true;
        }
    }
    return false;
}


bool WorldHit(Ray ray,float t_min,float t_max,inout HitRecord rec)
{
    bool isHit=false;
    HitRecord tempRec;
    float cloestSoFar=t_max;
    for(int i=0;i<OBJECT_COUNT;++i)
    {
        if(SphereHit(world.objects[i],ray,t_min,cloestSoFar,tempRec))
        {
            rec=tempRec;
            cloestSoFar=tempRec.t;
            isHit=true;
        }
    }
    return isHit;
}

vec3 WorldTrace(Ray ray)
{
    HitRecord hitRecord;
    vec3 attenuation=vec3(1.0);
    vec3 color=vec3(0.0);

        if(WorldHit(ray,0.001,100000.0,hitRecord))
        {
            return 0.5*(hitRecord.normal+vec3(1.0,1.0,1.0));
        }
    vec3 unit_direction=normalize(ray.direction);
    float t=unit_direction.y*0.5+0.5;
    return (1.0-t)*vec3(1.0)+t*vec3(0.5,0.7,1.0);
}

void main()
{
    vec3 finalColor=vec3(0.0);

    for(int i=0;i<spp;++i)
    {
        Ray r=CameraGetRay(camera,texcoord+Rand()/textureExtent);
        finalColor+=WorldTrace(r);
    }
    finalColor/=spp;

    fragColor=vec4(finalColor,1.0);
}