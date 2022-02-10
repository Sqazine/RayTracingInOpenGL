#include "GLVertexArrayObject.h"
#include <glad/glad.h>
GLVertexArrayObject::GLVertexArrayObject(const std::vector<GLVertex> &vertices, const std::vector<uint32_t> &indices)
	: m_Vertices(vertices), m_Indices(indices)
{
	glGenVertexArrays(1, &m_VertexArrayID);
	glBindVertexArray(m_VertexArrayID);

	glGenBuffers(1, &m_VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(GLVertex), m_Vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_IndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(uint32_t), m_Indices.data(), GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLVertex), reinterpret_cast<void *>(offsetof(GLVertex, position)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLVertex), reinterpret_cast<void *>(offsetof(GLVertex, normal)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLVertex), reinterpret_cast<void *>(offsetof(GLVertex, texcoord)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(GLVertex), reinterpret_cast<void *>(offsetof(GLVertex, tangent)));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(GLVertex), reinterpret_cast<void *>(offsetof(GLVertex, binormal)));

	glBindVertexArray(0);
}

GLVertexArrayObject::~GLVertexArrayObject()
{
	glDeleteBuffers(1, &m_VertexBufferID);
	glDeleteBuffers(1, &m_IndexBufferID);
	glDeleteVertexArrays(1, &m_VertexArrayID);
}

void GLVertexArrayObject::SetActive(bool isActive)
{
	if (isActive)
		glBindVertexArray(m_VertexArrayID);
	else
		glBindVertexArray(0);
}
