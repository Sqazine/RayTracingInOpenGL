#version 450 core

#define PI 3.1415926535

#define OBJECT_COUNT 2

#define MATERIAL_SIMPLE_DIFFUSE 0
#define MATERIAL_GAMMA_CORREDTED_DIFFUSE 1
#define MATERIAL_LAMBERTIAN 2
#define MATERIAL_HALF_LAMBERTIAN 3

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

vec2 Rand2()
{
    return vec2(Rand(),Rand());
}

vec3 Rand3()
{
    return vec3(Rand(),Rand(),Rand());
}

vec3 RandInUnitSphere()
{
  vec3 v=vec3(0.0);
  do
  {
      v=Rand3()*2.0-1.0;
  }while(v.x*v.x+v.y*v.y+v.z*v.z>1.0);
  return v;
}

vec3 RandInUnitVector()
{
    return normalize(RandInUnitSphere());
}

vec3 RandInHemiSphere(in vec3 normal)
{
    vec3 randInSphere=RandInUnitSphere();
    if(dot(randInSphere,normal)>0)
        return randInSphere;
    else return -randInSphere;
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
uniform World world;
uniform vec2 textureExtent;
uniform int curMaterialType;
uniform int maxDepth;

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

bool DiffuseScatter(in Ray incident,in HitRecord hitRecord,out Ray scattered,out vec3 attenuation)
{
    attenuation=vec3(0.5);
    scattered.origin=hitRecord.position;
    scattered.direction=normalize(hitRecord.normal+RandInUnitSphere());
    return true;
}

bool LambertianScatter(in Ray incident,in HitRecord hitRecord,out Ray scattered,out vec3 attenuation)
{
    attenuation=vec3(0.5);
    scattered.origin=hitRecord.position;
    scattered.direction=normalize(hitRecord.normal+RandInUnitVector());
    return true;
}

bool HalfLambertianScatter(in Ray incident,in HitRecord hitRecord,out Ray scattered,out vec3 attenuation)
{
    attenuation=vec3(0.5);
    scattered.origin=hitRecord.position;
    scattered.direction=normalize(RandInHemiSphere(hitRecord.normal));
    return true;
}

bool MaterialScatter(in Ray incident,in HitRecord hitRecord,out Ray scattered,out vec3 attenuation)
{
    if(curMaterialType==MATERIAL_SIMPLE_DIFFUSE||curMaterialType==MATERIAL_GAMMA_CORREDTED_DIFFUSE)
        return DiffuseScatter(incident,hitRecord,scattered,attenuation);
    else if(curMaterialType==MATERIAL_LAMBERTIAN)
        return LambertianScatter(incident,hitRecord,scattered,attenuation);
        else if(curMaterialType==MATERIAL_HALF_LAMBERTIAN)
            return HalfLambertianScatter(incident,hitRecord,scattered,attenuation);
    else return false;
}

vec3 GetEnvironmentColor(Ray ray)
{
     vec3 unit_direction=normalize(ray.direction);
        float t=unit_direction.y*0.5+0.5;
        return (1.0-t)*vec3(1.0)+t*vec3(0.5,0.7,1.0);
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

vec3 WorldTrace(Ray ray,int depth)
{
    HitRecord hitRecord;
    vec3 attenuation=vec3(1.0);
    vec3 color=vec3(0.0);
    while(depth>0)
    {
        depth--;

        if(WorldHit(ray,0.001,100000.0,hitRecord))
        {
            Ray scattered;
            vec3 cur_attenuation;
            if(!MaterialScatter(ray,hitRecord,scattered,cur_attenuation))
                break;
            attenuation*=cur_attenuation;
            ray=scattered;
        }
        else 
            return attenuation*GetEnvironmentColor(ray);
    }
    return vec3(0.0);
}

void main()
{
    vec3 finalColor=vec3(0.0);

    for(int i=0;i<spp;++i)
    {
        Ray r=CameraGetRay(camera,texcoord+Rand()/textureExtent);
        finalColor+=WorldTrace(r,maxDepth);
    }
    finalColor/=spp;

    if(curMaterialType!=MATERIAL_SIMPLE_DIFFUSE)
        finalColor=pow(finalColor,vec3(1/2.0));

    fragColor=vec4(finalColor,1.0);
}