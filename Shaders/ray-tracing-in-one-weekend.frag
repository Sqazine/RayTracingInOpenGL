#version 450 core

#define PI 3.1415926535

#define OBJECT_COUNT 50
#define LAMBERTIAN_MATERIAL_COUNT 50
#define HALF_LAMBERTIAN_MATERIAL_COUNT 50
#define METAL_MATERIAL_COUNT 50
#define DIELECTRIC_MATERIAL_COUNT 50

#define MATERIAL_LAMBERTIAN 0
#define MATERIAL_HALF_LAMBERTIAN 1
#define MATERIAL_METALLIC 2
#define MATERIAL_DIELECTRIC 3
#define MATERIAL_PBR 4

layout(location=0) out vec4 fragColor;

layout(location=0) in vec2 texcoord;

uniform int spp;
uniform int traceDepth;

uniform vec2 textureExtent;

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

vec3 RandInUnitDisk()
{
   vec3 v=vec3(0.0);
  do
  {
      v=vec3(Rand2()*2.0-1.0,0.0);
  }while(v.x*v.x+v.y*v.y+v.z*v.z>1.0);
  return v;
}


bool IsNearZero(vec3 value)
{
    const float s=1e-8;
    return abs(value.x)<s&&abs(value.y)<s&&abs(value.z)<s;
}

struct Lambertian
{
    vec3 albedo;
};

struct HalfLambertian
{
    vec3 albedo;
};

struct Metallic
{
    vec3 albedo;
    float roughness;
};

struct Dielectric
{
    float ior;
};

struct Ray
{
    vec3 origin;
    vec3 direction;
};

struct Sphere
{
    vec3 origin;
    float radius;
    int materialType;
    int materialIdx;
};

struct World
{
    Sphere[OBJECT_COUNT] objects;
};

struct HitRecord
{
    float t;
    vec3 position;
    vec3 normal;
    bool frontFace;
    int materialType;
    int materialIdx;
};

void SetFrontFace(inout HitRecord hitRecord,Ray ray,vec3 outward_normal)
{
    hitRecord.frontFace=dot(ray.direction,outward_normal)<0;
    hitRecord.normal=hitRecord.frontFace?outward_normal:-outward_normal;
}

struct Camera
{
    vec3 front;
    vec3 right;
    vec3 up;
    float lensRadius;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
};


uniform Camera camera;
uniform World world;
uniform Lambertian lambertianMaterials[LAMBERTIAN_MATERIAL_COUNT];
uniform HalfLambertian halfLambertianMaterials[HALF_LAMBERTIAN_MATERIAL_COUNT];
uniform Metallic metallicMaterials[METAL_MATERIAL_COUNT];
uniform Dielectric dielectricMaterials[DIELECTRIC_MATERIAL_COUNT];

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
    vec3 rd=camera.lensRadius*RandInUnitDisk();
    vec3 offset=camera.right*rd.x+camera.up*rd.y;
    Ray ray=RayCtor(camera.origin+offset,camera.lower_left_corner+uv.x*camera.horizontal+uv.y*camera.vertical-camera.origin-offset);
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
            hitRecord.materialType=sphere.materialType;
            hitRecord.materialIdx=sphere.materialIdx;
            return true;
        }

        temp=(-b+sqrt(delta))/(2.0*a);
        if(temp<t_max&&temp>t_min)
        {
            hitRecord.t=temp;
            hitRecord.position=RayGetPointAt(ray,hitRecord.t);
            vec3 normal=(hitRecord.position-sphere.origin)/sphere.radius;
            SetFrontFace(hitRecord,ray,normal);
            hitRecord.materialType=sphere.materialType;
            hitRecord.materialIdx=sphere.materialIdx;
            return true;
        }
    }
    return false;
}

bool LambertianScatter(in Lambertian lambertian,in Ray incident,in HitRecord hitRecord,out Ray scattered,out vec3 attenuation)
{
    attenuation=lambertian.albedo;
    scattered.origin=hitRecord.position;
    scattered.direction=normalize(hitRecord.normal+RandInUnitVector());
    if(IsNearZero(scattered.direction))
        scattered.direction=hitRecord.normal;
    return true;
}

bool HalfLambertianScatter(in HalfLambertian halfLambertian,in Ray incident,in HitRecord hitRecord,out Ray scattered,out vec3 attenuation)
{
    attenuation=halfLambertian.albedo;
    scattered.origin=hitRecord.position;
    scattered.direction=normalize(RandInHemiSphere(hitRecord.normal));
    if(IsNearZero(scattered.direction))
        scattered.direction=hitRecord.normal;
    return true;
}

bool MetallicScatter(in Metallic metallic,in Ray incident,in HitRecord hitRecord,out Ray scattered,out vec3 attenuation)
{
    attenuation=metallic.albedo;
    scattered.origin=hitRecord.position;
    scattered.direction=reflect(normalize(incident.direction),hitRecord.normal)+metallic.roughness*RandInUnitSphere();
    return dot(scattered.direction,hitRecord.normal)>0;
}

float reflectance(float cosine,float ref_idx)
{
     // Use Schlick's approximation for reflectance.
    float r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}

bool DielectricScatter(in Dielectric dielectric,in Ray incident,in HitRecord hitRecord,out Ray scattered,out vec3 attenuation)
{
    attenuation=vec3(1.0);
    float refraction_ratio=hitRecord.frontFace?(1.0/dielectric.ior):dielectric.ior;

    vec3 unit_dir=normalize(incident.direction);
    float cos_theta=min(dot(-unit_dir,hitRecord.normal),1.0);
    float sin_theta=sqrt(1.0-cos_theta*cos_theta);

    bool cannot_refract=refraction_ratio*sin_theta>1.0;
    vec3 dir;

    if(cannot_refract||reflectance(cos_theta,refraction_ratio)>Rand())
        dir=reflect(unit_dir,hitRecord.normal);
    else
        dir=refract(unit_dir,hitRecord.normal,refraction_ratio);

    scattered=RayCtor(hitRecord.position,dir);
    return true;
}

vec3 GetEnvironmentColor(Ray ray)
{
     vec3 unit_direction=normalize(ray.direction);
        float t=unit_direction.y*0.5+0.5;
        return (1.0-t)*vec3(1.0)+t*vec3(0.5,0.7,1.0);
}

bool MaterialScatter(in Ray incident,in HitRecord hitRecord,out Ray scattered,out vec3 attenuation)
{
    if(hitRecord.materialType==MATERIAL_LAMBERTIAN)
        return LambertianScatter(lambertianMaterials[hitRecord.materialIdx],incident,hitRecord,scattered,attenuation);
    else if(hitRecord.materialType==MATERIAL_HALF_LAMBERTIAN)
        return HalfLambertianScatter(halfLambertianMaterials[hitRecord.materialIdx],incident,hitRecord,scattered,attenuation);
    else if(hitRecord.materialType==MATERIAL_METALLIC)
        return MetallicScatter(metallicMaterials[hitRecord.materialIdx],incident,hitRecord,scattered,attenuation);
    else if(hitRecord.materialType==MATERIAL_DIELECTRIC)
        return DielectricScatter(dielectricMaterials[hitRecord.materialIdx],incident,hitRecord,scattered,attenuation);
    else return false;
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
        {
            return attenuation*GetEnvironmentColor(ray);
        }
    }
    return vec3(0.0);
}

void main()
{
    vec3 finalColor=vec3(0.0);

    for(int i=0;i<spp;++i)
    {
        Ray r=CameraGetRay(camera,texcoord+Rand()/textureExtent);
        finalColor+=WorldTrace(r,traceDepth);
    }
    finalColor/=spp;

    finalColor=pow(finalColor,vec3(1/2.0));

    fragColor=vec4(finalColor,1.0);
}