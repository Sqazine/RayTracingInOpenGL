#include "Shape.h"
#include "GL/Renderer.h"
#include <iostream>
Mesh::Mesh()
{
}

Mesh::Mesh(MeshType type)
{
    switch (type)
    {
    case MeshType::QUAD:
        CreateBuiltInQuad();
        break;
    case MeshType::CUBE:
        CreateBuiltInCube();
        break;
    case MeshType::SPHERE:
        CreateBuiltInSphere();
        break;
    case MeshType::CAPSULE:
        CreateBuiltInCapsule();
        break;
    case MeshType::CYLINDER:
        CreateBuiltInCylinder();
        break;
    default:
        std::cout << "MODEL::ERROR:failed to create model" << std::endl;
        break;
    }
}

Mesh::Mesh(const std::vector<Vector3f> &position,
           const std::vector<Vector3f> &normal,
           const std::vector<Vector2f> &texcoord,
           const std::vector<Vector3f> &tangent,
           const std::vector<Vector3f> &binormal,
           const std::vector<Vector4f> &color,
           const std::vector<uint32_t> &indices)
    : mPosition(position),
      mNormal(normal),
      mTexcoord(texcoord),
      mTangent(tangent),
      mBiNormal(binormal),
      mColor(color),
      mIndices(indices)
{
    mVertexArray = std::make_shared<GL::VertexArray>();
    mPositionBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(mPosition);
    mNormalBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(mNormal);
    mTexcoordBuffer = std::make_shared<GL::VertexBuffer<Vector2f>>(mTexcoord);
    mTangentBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(mTangent);
    mBiNormalBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(mBiNormal);
    mColorBuffer = std::make_shared<GL::VertexBuffer<Vector4f>>(mColor);
    mIndexBuffer = std::make_shared<GL::IndexBuffer>(mIndices);
}

Mesh::Mesh(const Mesh &other)
{
    *this = other;
}
Mesh &Mesh::operator=(const Mesh &other)
{
    if (this == &other)
        return *this;

    mPosition = other.mPosition;
    mNormal = other.mNormal;
    mTexcoord = other.mTexcoord;
    mIndices = other.mIndices;

    mVertexArray = std::make_shared<GL::VertexArray>();
    mPositionBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>();
    mNormalBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>();
    mTexcoordBuffer = std::make_shared<GL::VertexBuffer<Vector2f>>();
    mTangentBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>();
    mBiNormalBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>();
    mColorBuffer = std::make_shared<GL::VertexBuffer<Vector4f>>();
    mIndexBuffer = std::make_shared<GL::IndexBuffer>();

    UpdateBuffers();
    return *this;
}

Mesh::~Mesh()
{
}

const std::vector<Vector3f> &Mesh::GetPosition() const
{
    return mPosition;
}

const std::vector<Vector3f> &Mesh::GetNormal() const
{
    return mNormal;
}
const std::vector<Vector2f> &Mesh::GetTexcoord() const
{
    return mTexcoord;
}
const std::vector<Vector3f> &Mesh::GetTangent() const
{
    return mTangent;
}
const std::vector<Vector3f> &Mesh::GetBiNormal() const
{
    return mBiNormal;
}
const std::vector<Vector4f> &Mesh::GetColor() const
{
    return mColor;
}

const std::vector<uint32_t> &Mesh::GetIndex() const
{
    return mIndices;
}

void Mesh::UpdateBuffers()
{
    if (mPosition.size() > 0)
        mPositionBuffer->Set(mPosition);
    if (mNormal.size() > 0)
        mNormalBuffer->Set(mNormal);
    if (mTexcoord.size() > 0)
        mTexcoordBuffer->Set(mTexcoord);
    if (mTangent.size() > 0)
        mTangentBuffer->Set(mTangent);
    if (mBiNormal.size() > 0)
        mBiNormalBuffer->Set(mBiNormal);
    if (mColor.size() > 0)
        mColorBuffer->Set(mColor);
    if (mIndices.size() > 0)
        mIndexBuffer->Set(mIndices);
}

void Mesh::Bind(int position, int normal, int texcoord, int tangent, int binormal, int color)
{
    mVertexArray->SetActive(true);
    if (position >= 0)
        mPositionBuffer->BindTo(position);
    if (normal >= 0)
        mNormalBuffer->BindTo(normal);
    if (texcoord >= 0)
        mTexcoordBuffer->BindTo(texcoord);
    if (tangent >= 0)
        mTangentBuffer->BindTo(tangent);
    if (binormal >= 0)
        mBiNormalBuffer->BindTo(binormal);
    if (color >= 0)
        mColorBuffer->BindTo(color);
}
void Mesh::UnBind(int position, int normal, int texcoord, int tangent, int binormal, int color)
{
    if (position >= 0)
        mPositionBuffer->UnBindFrom(position);
    if (normal >= 0)
        mNormalBuffer->UnBindFrom(normal);
    if (texcoord >= 0)
        mTexcoordBuffer->UnBindFrom(texcoord);
    if (tangent >= 0)
        mTangentBuffer->UnBindFrom(tangent);
    if (binormal >= 0)
        mBiNormalBuffer->UnBindFrom(binormal);
    if (color >= 0)
        mColorBuffer->UnBindFrom(color);

    mVertexArray->SetActive(false);
}

void Mesh::Draw()
{
    if(mIndices.size()>0)
        GL::Renderer::Render(mIndexBuffer, GL::RenderType::TRIANGLES);
    else
        GL::Renderer::Render(mPosition.size(), GL::RenderType::TRIANGLES);
}

void Mesh::CreateBuiltInQuad()
{
    mVertexArray = std::make_shared<GL::VertexArray>();
    mVertexArray->SetActive(true);

    mPosition =
        {
            Vector3f(-1.0f, 1.0f, 0.0f),
            Vector3f(-1.0f, -1.0f, 0.0f),
            Vector3f(1.0f, -1.0f, 0.0f),
            Vector3f(1.0f, 1.0f, 0.0f)};

    mNormal =
        {
            Vector3f(0.0f, 0.0f, 1.0f),
            Vector3f(0.0f, 0.0f, 1.0f),
            Vector3f(0.0f, 0.0f, 1.0f),
            Vector3f(0.0f, 0.0f, 1.0f)

        };

    mTexcoord =
        {
            Vector2f(0.0f, 1.0f),
            Vector2f(0.0f, 0.0f),
            Vector2f(1.0f, 0.0f),
            Vector2f(1.0f, 1.0f)};

    mTangent.resize(4, Vector3f::ZERO);
    mBiNormal.resize(4, Vector3f::ZERO);
    mColor.resize(4, Vector4f(1.0f));

    mPositionBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(mPosition);
    mNormalBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(mNormal);
    mTexcoordBuffer = std::make_shared<GL::VertexBuffer<Vector2f>>(mTexcoord);
    mTangentBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(mTangent);
    mBiNormalBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(mBiNormal);
    mColorBuffer = std::make_shared<GL::VertexBuffer<Vector4f>>(mColor);

    mIndices =
        {
            0, 1, 2,
            0, 2, 3};
    mIndexBuffer = std::make_shared<GL::IndexBuffer>(mIndices);
}

void Mesh::CreateBuiltInSphere()
{
    mVertexArray = std::make_shared<GL::VertexArray>();
    mVertexArray->SetActive(true);

    float latitudeBands = 20.0f;
    float longitudeBands = 20.0f;
    float radius = 1.0f;

    for (float latNumber = 0; latNumber <= latitudeBands; latNumber++)
    {
        float theta = latNumber * 3.14 / latitudeBands;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (float longNumber = 0; longNumber <= longitudeBands; longNumber++)
        {
            float phi = longNumber * 2 * 3.147 / longitudeBands;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            Vector3f tmp = Vector3f(cosPhi * sinTheta, cosTheta, sinPhi * sinTheta);

            mPosition.emplace_back(tmp * radius);
            mNormal.emplace_back(tmp);
            mTexcoord.emplace_back(Vector2f((longNumber / longitudeBands), (latNumber / latitudeBands)));

            Vector3f tangent = Vector3f(cosPhi, 0.0f, -sinPhi);

            mTangent.emplace_back(tangent);
            mBiNormal.emplace_back(Vector3f::Cross(tmp, tangent));
        }
    }

    for (uint32_t latNumber = 0; latNumber < latitudeBands; latNumber++)
    {
        for (uint32_t longNumber = 0; longNumber < longitudeBands; longNumber++)
        {
            uint32_t first = (latNumber * (longitudeBands + 1)) + longNumber;
            uint32_t second = first + longitudeBands + 1;

            mIndices.emplace_back(first);
            mIndices.emplace_back(second);
            mIndices.emplace_back(first + 1);

            mIndices.emplace_back(second);
            mIndices.emplace_back(second + 1);
            mIndices.emplace_back(first + 1);
        }
    }

    mColor.resize(mPosition.size(), Vector4f(1.0f));

    mPositionBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(mPosition);
    mNormalBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(mNormal);
    mTexcoordBuffer = std::make_shared<GL::VertexBuffer<Vector2f>>(mTexcoord);
    mTangentBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(mTangent);
    mBiNormalBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(mBiNormal);
    mColorBuffer = std::make_shared<GL::VertexBuffer<Vector4f>>(mColor);

    mIndexBuffer = std::make_shared<GL::IndexBuffer>(mIndices);
}

void Mesh::CreateBuiltInCube()
{
    mVertexArray = std::make_shared<GL::VertexArray>();
    mVertexArray->SetActive(true);

    mPosition =
        {
            //+z
            Vector3f(-0.5f, 0.5f, 0.5f),
            Vector3f(-0.5f, -0.5f, 0.5f),
            Vector3f(0.5f, -0.5f, 0.5f),
            Vector3f(0.5f, 0.5f, 0.5f),
            //-z
            Vector3f(0.5f, 0.5f, -0.5f),
            Vector3f(0.5f, -0.5f, -0.5f),
            Vector3f(-0.5f, -0.5f, -0.5f),
            Vector3f(-0.5f, 0.5f, -0.5f),
            //+x
            Vector3f(0.5f, 0.5f, 0.5f),
            Vector3f(0.5f, -0.5f, 0.5f),
            Vector3f(0.5f, -0.5f, -0.5f),
            Vector3f(0.5f, 0.5f, -0.5f),
            //-x
            Vector3f(-0.5f, 0.5f, -0.5f),
            Vector3f(-0.5f, -0.5f, -0.5f),
            Vector3f(-0.5f, -0.5f, 0.5f),
            Vector3f(-0.5f, 0.5f, 0.5f),
            //+y
            Vector3f(-0.5f, 0.5f, -0.5f),
            Vector3f(-0.5f, 0.5f, 0.5f),
            Vector3f(0.5f, 0.5f, 0.5f),
            Vector3f(0.5f, 0.5f, -0.5f),
            //-y
            Vector3f(-0.5f, -0.5f, 0.5f),
            Vector3f(-0.5f, -0.5f, -0.5f),
            Vector3f(0.5f, -0.5f, -0.5f),
            Vector3f(0.5f, -0.5f, 0.5f),
        };

    mNormal =
        {
            //+z
            Vector3f(0.0f, 0.0f, 1.0f),
            Vector3f(0.0f, 0.0f, 1.0f),
            Vector3f(0.0f, 0.0f, 1.0f),
            Vector3f(0.0f, 0.0f, 1.0f),
            //-z
            Vector3f(0.0f, 0.0f, -1.0f),
            Vector3f(0.0f, 0.0f, -1.0f),
            Vector3f(0.0f, 0.0f, -1.0f),
            Vector3f(0.0f, 0.0f, -1.0f),
            //+x
            Vector3f(1.0f, 0.0f, 0.0f),
            Vector3f(1.0f, 0.0f, 0.0f),
            Vector3f(1.0f, 0.0f, 0.0f),
            Vector3f(1.0f, 0.0f, 0.0f),
            //-x
            Vector3f(-1.0f, 0.0f, 0.0f),
            Vector3f(-1.0f, 0.0f, 0.0f),
            Vector3f(-1.0f, 0.0f, 0.0f),
            Vector3f(-1.0f, 0.0f, 0.0f),
            //+y
            Vector3f(0.0f, 1.0f, 0.0f),
            Vector3f(0.0f, 1.0f, 0.0f),
            Vector3f(0.0f, 1.0f, 0.0f),
            Vector3f(0.0f, 1.0f, 0.0f),
            //-y
            Vector3f(0.0f, -1.0f, 0.0f),
            Vector3f(0.0f, -1.0f, 0.0f),
            Vector3f(0.0f, -1.0f, 0.0f),
            Vector3f(0.0f, -1.0f, 0.0f),
        };

    mTexcoord =
        {
            //+z
            Vector2f(0.0f, 1.0f),
            Vector2f(0.0f, 0.0f),
            Vector2f(1.0f, 0.0f),
            Vector2f(1.0f, 1.0f),
            //-z
            Vector2f(0.0f, 1.0f),
            Vector2f(0.0f, 0.0f),
            Vector2f(1.0f, 0.0f),
            Vector2f(1.0f, 1.0f),
            //+x
            Vector2f(0.0f, 1.0f),
            Vector2f(0.0f, 0.0f),
            Vector2f(1.0f, 0.0f),
            Vector2f(1.0f, 1.0f),
            //-x
            Vector2f(0.0f, 1.0f),
            Vector2f(0.0f, 0.0f),
            Vector2f(1.0f, 0.0f),
            Vector2f(1.0f, 1.0f),
            //+y
            Vector2f(0.0f, 1.0f),
            Vector2f(0.0f, 0.0f),
            Vector2f(1.0f, 0.0f),
            Vector2f(1.0f, 1.0f),
            //-y
            Vector2f(0.0f, 1.0f),
            Vector2f(0.0f, 0.0f),
            Vector2f(1.0f, 0.0f),
            Vector2f(1.0f, 1.0f),
        };

    mTangent =
        {
            //+z
            Vector3f(1.0f, 0.0f, 0.0f),
            Vector3f(1.0f, 0.0f, 0.0f),
            Vector3f(1.0f, 0.0f, 0.0f),
            Vector3f(1.0f, 0.0f, 0.0f),
            //-z
            Vector3f(-1.0f, 0.0f, 0.0f),
            Vector3f(-1.0f, 0.0f, 0.0f),
            Vector3f(-1.0f, 0.0f, 0.0f),
            Vector3f(-1.0f, 0.0f, 0.0f),
            //+x
            Vector3f(0.0f, 0.0f, -1.0f),
            Vector3f(0.0f, 0.0f, -1.0f),
            Vector3f(0.0f, 0.0f, -1.0f),
            Vector3f(0.0f, 0.0f, -1.0f),
            //-x
            Vector3f(0.0f, 0.0f, 1.0f),
            Vector3f(0.0f, 0.0f, 1.0f),
            Vector3f(0.0f, 0.0f, 1.0f),
            Vector3f(0.0f, 0.0f, 1.0f),
            //+y
            Vector3f(1.0f, 0.0f, 0.0f),
            Vector3f(1.0f, 0.0f, 0.0f),
            Vector3f(1.0f, 0.0f, 0.0f),
            Vector3f(1.0f, 0.0f, 0.0f),
            //-y
            Vector3f(1.0f, 0.0f, 0.0f),
            Vector3f(1.0f, 0.0f, 0.0f),
            Vector3f(1.0f, 0.0f, 0.0f),
            Vector3f(1.0f, 0.0f, 0.0f),
        };

    mBiNormal = {
        //+z
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f),
        //-z
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f),
        //+x
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f),
        //-x
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f),
        //+y
        Vector3f(0.0f, 0.0f, -1.0f),
        Vector3f(0.0f, 0.0f, -1.0f),
        Vector3f(0.0f, 0.0f, -1.0f),
        Vector3f(0.0f, 0.0f, -1.0f),
        //-y
        Vector3f(0.0f, 0.0f, 1.0f),
        Vector3f(0.0f, 0.0f, 1.0f),
        Vector3f(0.0f, 0.0f, 1.0f),
        Vector3f(0.0f, 0.0f, 1.0f),
    };

    mColor.resize(mPosition.size(), Vector4f(1.0f));

    std::vector<uint32_t> indices =
        {
            0,
            1,
            2,
            0,
            2,
            3,

            4,
            5,
            6,
            4,
            6,
            7,

            8,
            9,
            10,
            8,
            10,
            11,

            12,
            13,
            14,
            12,
            14,
            15,

            16,
            17,
            18,
            16,
            18,
            19,

            20,
            21,
            22,
            20,
            22,
            23,
        };

    mPositionBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(mPosition);
    mNormalBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(mNormal);
    mTexcoordBuffer = std::make_shared<GL::VertexBuffer<Vector2f>>(mTexcoord);
    mTangentBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(mTangent);
    mBiNormalBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(mBiNormal);
    mColorBuffer = std::make_shared<GL::VertexBuffer<Vector4f>>(mColor);

    mIndexBuffer = std::make_shared<GL::IndexBuffer>(mIndices);
}
void Mesh::CreateBuiltInCapsule()
{
}
void Mesh::CreateBuiltInCylinder()
{
}