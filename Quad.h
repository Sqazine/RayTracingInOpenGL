#pragma once
#include "GLVertexArrayObject.h"
#include <string>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

class Quad
{
public:
    Quad()
    {
        m_VertexArrayObject = std::make_shared<GLVertexArrayObject>(vertices, indices);
    }

    void Draw()
    {
        m_VertexArrayObject->SetActive(true);
        glDrawElements(GL_TRIANGLES, m_VertexArrayObject->m_Indices.size(), GL_UNSIGNED_INT, nullptr);
        m_VertexArrayObject->SetActive(false);
    }

private:
    std::vector<GLVertex> vertices =
        {
            GLVertex{Vector3f(-1.0f, 1.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f), Vector2f(0.0f, 1.0f), Vector3f(1.0, 0.0, 0.0f), Vector3f(0.0f, 1.0f, 0.0f)},
            GLVertex{Vector3f(-1.0f, -1.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f), Vector2f(0.0f, 0.0f), Vector3f(1.0f, 0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f)},
            GLVertex{Vector3f(1.0f, -1.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f), Vector2f(1.0f, 0.0f), Vector3f(1.0f, 0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f)},
            GLVertex{Vector3f(1.0f, 1.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f), Vector2f(1.0f, 1.0f), Vector3f(1.0f, 0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f)}};
    std::vector<uint32_t> indices =
        {
            0, 1, 2,
            0, 2, 3};

    std::shared_ptr<GLVertexArrayObject> m_VertexArrayObject;
};