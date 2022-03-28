#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
#include <vector>
#include <memory>
#include "GL/VertexArray.h"
#include "GL/VertexBuffer.h"
#include "GL/IndexBuffer.h"
struct Shape
{
    int materialType;
    int materialIdx;
};

struct Sphere : public Shape
{
    Sphere(Vector3f o, float r) : origin(o), radius(r)
    {
        materialType = -1;
        materialIdx = -1;
    }
    Sphere(Vector3f o, float r, int matType, int matIdx) : origin(o), radius(r)
    {
        materialType = matType;
        materialIdx = matIdx;
    }

    Vector3f origin;
    float radius;
};

enum class MeshType
{
    QUAD,
    SPHERE,
    CUBE,
    CAPSULE,
    CYLINDER,
};

class Mesh : public Shape
{
public:
    Mesh();
    Mesh(MeshType type);
    Mesh(const std::vector<Vector3f> &position,
         const std::vector<Vector3f> &normal,
         const std::vector<Vector2f> &texcoord,
         const std::vector<Vector3f> &tangent,
         const std::vector<Vector3f> &binormal,
         const std::vector<Vector4f> &color,
         const std::vector<uint32_t> &indices);
    Mesh(const Mesh &other);
    Mesh &operator=(const Mesh &other);
    ~Mesh();

    const std::vector<Vector3f> &GetPosition() const;
    const std::vector<Vector3f> &GetNormal() const;
    const std::vector<Vector2f> &GetTexcoord() const;
    const std::vector<Vector3f> &GetTangent() const;
    const std::vector<Vector3f> &GetBiNormal() const;
    const std::vector<Vector4f> &GetColor() const;
    const std::vector<uint32_t> &GetIndex() const;

    void Draw();

    void Bind(int position = -1, int normal = -1, int texcoord = -1, int tangent = -1, int binormal = -1, int color = -1);
    void UnBind(int position = -1, int normal = -1, int texcoord = -1, int tangent = -1, int binormal = -1, int color = -1);

protected:
    void UpdateBuffers();
    void CreateBuiltInQuad();
    void CreateBuiltInCube();
    void CreateBuiltInSphere();
    void CreateBuiltInCapsule();
    void CreateBuiltInCylinder();

    std::vector<Vector3f> mPosition;
    std::vector<Vector3f> mNormal;
    std::vector<Vector2f> mTexcoord;
    std::vector<Vector3f> mTangent;
    std::vector<Vector3f> mBiNormal;
    std::vector<Vector4f> mColor;
    std::vector<uint32_t> mIndices;

    std::shared_ptr<GL::VertexArray> mVertexArray;
    std::shared_ptr<GL::VertexBuffer<Vector3f>> mPositionBuffer;
    std::shared_ptr<GL::VertexBuffer<Vector3f>> mNormalBuffer;
    std::shared_ptr<GL::VertexBuffer<Vector2f>> mTexcoordBuffer;
    std::shared_ptr<GL::VertexBuffer<Vector3f>> mTangentBuffer;
    std::shared_ptr<GL::VertexBuffer<Vector3f>> mBiNormalBuffer;
    std::shared_ptr<GL::VertexBuffer<Vector4f>> mColorBuffer;
    std::shared_ptr<GL::IndexBuffer> mIndexBuffer;
};