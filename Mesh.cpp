#include "Mesh.h"
#include <vector>
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
	: m_Position(position),
	  m_Normal(normal),
	  m_Texcoord(texcoord),
	  m_Tangent(tangent),
	  m_BiNormal(binormal),
	  m_Color(color),
	  m_Indices(indices)
{
	m_VertexArray = std::make_shared<GL::VertexArray>();
	m_PositionBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(m_Position);
	m_NormalBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(m_Normal);
	m_TexcoordBuffer = std::make_shared<GL::VertexBuffer<Vector2f>>(m_Texcoord);
	m_TangentBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(m_Tangent);
	m_BiNormalBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(m_BiNormal);
	m_ColorBuffer = std::make_shared<GL::VertexBuffer<Vector4f>>(m_Color);
	m_IndexBuffer = std::make_shared<GL::IndexBuffer>(m_Indices);
}

Mesh::Mesh(const Mesh &other)
{
	*this = other;
}
Mesh &Mesh::operator=(const Mesh &other)
{
	if (this == &other)
		return *this;

	m_Position = other.m_Position;
	m_Normal = other.m_Normal;
	m_Texcoord = other.m_Texcoord;
	m_Indices = other.m_Indices;

	m_VertexArray = std::make_shared<GL::VertexArray>();
	m_PositionBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>();
	m_NormalBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>();
	m_TexcoordBuffer = std::make_shared<GL::VertexBuffer<Vector2f>>();
	m_TangentBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>();
	m_BiNormalBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>();
	m_ColorBuffer = std::make_shared<GL::VertexBuffer<Vector4f>>();
	m_IndexBuffer = std::make_shared<GL::IndexBuffer>();

	UpdateBuffers();
	return *this;
}

Mesh::~Mesh()
{
}

const std::vector<Vector3f> &Mesh::GetPosition() const
{
	return m_Position;
}

const std::vector<Vector3f> &Mesh::GetNormal() const
{
	return m_Normal;
}
const std::vector<Vector2f> &Mesh::GetTexcoord() const
{
	return m_Texcoord;
}
const std::vector<Vector3f> &Mesh::GetTangent() const
{
	return m_Tangent;
}
const std::vector<Vector3f> &Mesh::GetBiNormal() const
{
	return m_BiNormal;
}
const std::vector<Vector4f> &Mesh::GetColor() const
{
	return m_Color;
}

const std::vector<uint32_t> &Mesh::GetIndex() const
{
	return m_Indices;
}

void Mesh::UpdateBuffers()
{
	if (m_Position.size() > 0)
		m_PositionBuffer->Set(m_Position);
	if (m_Normal.size() > 0)
		m_NormalBuffer->Set(m_Normal);
	if (m_Texcoord.size() > 0)
		m_TexcoordBuffer->Set(m_Texcoord);
	if (m_Tangent.size() > 0)
		m_TangentBuffer->Set(m_Tangent);
	if (m_BiNormal.size() > 0)
		m_BiNormalBuffer->Set(m_BiNormal);
	if (m_Color.size() > 0)
		m_ColorBuffer->Set(m_Color);
	if (m_Indices.size() > 0)
		m_IndexBuffer->Set(m_Indices);
}

void Mesh::Bind(int position, int normal, int texcoord, int tangent, int binormal, int color)
{
	m_VertexArray->SetActive(true);
	if (position >= 0)
		m_PositionBuffer->BindTo(position);
	if (normal >= 0)
		m_NormalBuffer->BindTo(normal);
	if (texcoord >= 0)
		m_TexcoordBuffer->BindTo(texcoord);
	if (tangent >= 0)
		m_TangentBuffer->BindTo(tangent);
	if (binormal >= 0)
		m_BiNormalBuffer->BindTo(binormal);
	if (color >= 0)
		m_ColorBuffer->BindTo(color);
}
void Mesh::UnBind(int position, int normal, int texcoord, int tangent, int binormal, int color)
{
	if (position >= 0)
		m_PositionBuffer->UnBindFrom(position);
	if (normal >= 0)
		m_NormalBuffer->UnBindFrom(normal);
	if (texcoord >= 0)
		m_TexcoordBuffer->UnBindFrom(texcoord);
	if (tangent >= 0)
		m_TangentBuffer->UnBindFrom(tangent);
	if (binormal >= 0)
		m_BiNormalBuffer->UnBindFrom(binormal);
	if (color >= 0)
		m_ColorBuffer->UnBindFrom(color);

	m_VertexArray->SetActive(false);
}

void Mesh::Draw()
{
	if (m_Indices.size() > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer->GetID());
		glDrawElements(GL_TRIANGLES, m_IndexBuffer->Size(), m_IndexBuffer->GetDataType(), nullptr);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else
		glDrawArrays(GL_TRIANGLES, 0, m_Position.size());
}

void Mesh::CreateBuiltInQuad()
{
	m_VertexArray = std::make_shared<GL::VertexArray>();
	m_VertexArray->SetActive(true);

	m_Position =
		{
			Vector3f(-1.0f, 1.0f, 0.0f),
			Vector3f(-1.0f, -1.0f, 0.0f),
			Vector3f(1.0f, -1.0f, 0.0f),
			Vector3f(1.0f, 1.0f, 0.0f)};

	m_Normal =
		{
			Vector3f(0.0f, 0.0f, 1.0f),
			Vector3f(0.0f, 0.0f, 1.0f),
			Vector3f(0.0f, 0.0f, 1.0f),
			Vector3f(0.0f, 0.0f, 1.0f)

		};

	m_Texcoord =
		{
			Vector2f(0.0f, 1.0f),
			Vector2f(0.0f, 0.0f),
			Vector2f(1.0f, 0.0f),
			Vector2f(1.0f, 1.0f)};

	m_Tangent.resize(4, Vector3f::ZERO);
	m_BiNormal.resize(4, Vector3f::ZERO);
	m_Color.resize(4, Vector4f(1.0f));

	m_PositionBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(m_Position);
	m_NormalBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(m_Normal);
	m_TexcoordBuffer = std::make_shared<GL::VertexBuffer<Vector2f>>(m_Texcoord);
	m_TangentBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(m_Tangent);
	m_BiNormalBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(m_BiNormal);
	m_ColorBuffer = std::make_shared<GL::VertexBuffer<Vector4f>>(m_Color);

	m_Indices =
		{
			0, 1, 2,
			0, 2, 3};
	m_IndexBuffer = std::make_shared<GL::IndexBuffer>(m_Indices);
}

void Mesh::CreateBuiltInSphere()
{
	m_VertexArray = std::make_shared<GL::VertexArray>();
	m_VertexArray->SetActive(true);

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

			m_Position.emplace_back(tmp * radius);
			m_Normal.emplace_back(tmp);
			m_Texcoord.emplace_back(Vector2f((longNumber / longitudeBands), (latNumber / latitudeBands)));

			Vector3f tangent = Vector3f(cosPhi, 0.0f, -sinPhi);

			m_Tangent.emplace_back(tangent);
			m_BiNormal.emplace_back(Vector3f::Cross(tmp, tangent));
		}
	}

	for (uint32_t latNumber = 0; latNumber < latitudeBands; latNumber++)
	{
		for (uint32_t longNumber = 0; longNumber < longitudeBands; longNumber++)
		{
			uint32_t first = (latNumber * (longitudeBands + 1)) + longNumber;
			uint32_t second = first + longitudeBands + 1;

			m_Indices.emplace_back(first);
			m_Indices.emplace_back(second);
			m_Indices.emplace_back(first + 1);

			m_Indices.emplace_back(second);
			m_Indices.emplace_back(second + 1);
			m_Indices.emplace_back(first + 1);
		}
	}

	m_Color.resize(m_Position.size(), Vector4f(1.0f));

	m_PositionBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(m_Position);
	m_NormalBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(m_Normal);
	m_TexcoordBuffer = std::make_shared<GL::VertexBuffer<Vector2f>>(m_Texcoord);
	m_TangentBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(m_Tangent);
	m_BiNormalBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(m_BiNormal);
	m_ColorBuffer = std::make_shared<GL::VertexBuffer<Vector4f>>(m_Color);

	m_IndexBuffer = std::make_shared<GL::IndexBuffer>(m_Indices);
}

void Mesh::CreateBuiltInCube()
{
	m_VertexArray = std::make_shared<GL::VertexArray>();
	m_VertexArray->SetActive(true);

	m_Position =
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

	m_Normal =
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

	m_Texcoord =
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

	m_Tangent =
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

	m_BiNormal = {
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

	m_Color.resize(m_Position.size(), Vector4f(1.0f));

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

	m_PositionBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(m_Position);
	m_NormalBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(m_Normal);
	m_TexcoordBuffer = std::make_shared<GL::VertexBuffer<Vector2f>>(m_Texcoord);
	m_TangentBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(m_Tangent);
	m_BiNormalBuffer = std::make_shared<GL::VertexBuffer<Vector3f>>(m_BiNormal);
	m_ColorBuffer = std::make_shared<GL::VertexBuffer<Vector4f>>(m_Color);

	m_IndexBuffer = std::make_shared<GL::IndexBuffer>(m_Indices);
}
void Mesh::CreateBuiltInCapsule()
{
}
void Mesh::CreateBuiltInCylinder()
{
}