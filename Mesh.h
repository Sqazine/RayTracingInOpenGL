#pragma once
#include <memory>
#include <vector>
#include "GL/VertexArray.h"
#include "GL/VertexBuffer.h"
#include "GL/IndexBuffer.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
enum class MeshType
{
	QUAD,
	SPHERE,
	CUBE,
	CAPSULE,
	CYLINDER,
};

class Mesh
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

	std::vector<Vector3f> m_Position;
	std::vector<Vector3f> m_Normal;
	std::vector<Vector2f> m_Texcoord;
	std::vector<Vector3f> m_Tangent;
	std::vector<Vector3f> m_BiNormal;
	std::vector<Vector4f> m_Color;
	std::vector<uint32_t> m_Indices;

	std::shared_ptr<GL::VertexArray> m_VertexArray;
	std::shared_ptr<GL::VertexBuffer<Vector3f>> m_PositionBuffer;
	std::shared_ptr<GL::VertexBuffer<Vector3f>> m_NormalBuffer;
	std::shared_ptr<GL::VertexBuffer<Vector2f>> m_TexcoordBuffer;
	std::shared_ptr<GL::VertexBuffer<Vector3f>> m_TangentBuffer;
	std::shared_ptr<GL::VertexBuffer<Vector3f>> m_BiNormalBuffer;
	std::shared_ptr<GL::VertexBuffer<Vector4f>> m_ColorBuffer;
	std::shared_ptr<GL::IndexBuffer> m_IndexBuffer;
};