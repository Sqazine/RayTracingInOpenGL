#pragma once
#include <vector>
#include "Vector2.h"
#include "Vector3.h"
struct GLVertex
{
	Vector3f position;
	Vector3f normal;
	Vector2f texcoord;
	Vector3f tangent;
	Vector3f binormal;
};

class GLVertexArrayObject
{
public:
	GLVertexArrayObject(const std::vector<GLVertex> &vertices, const std::vector<uint32_t> &indices);
	~GLVertexArrayObject();

	void SetActive(bool isActive);

	std::vector<GLVertex> m_Vertices;
	std::vector<uint32_t> m_Indices;

	uint32_t m_VertexArrayID;
	uint32_t m_VertexBufferID;
	uint32_t m_IndexBufferID;
};
