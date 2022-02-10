#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
class GLShader
{
public:
	GLShader(const std::string& vertName, const std::string& fragName);
	~GLShader();

	bool Compile();

	void SetActive(bool isActive);

	void SetVector4(const std::string& name, const Vector4f& vec);
	void SetVector3(const std::string& name, const Vector3f& vec3);
	void SetVector2(const std::string& name, const Vector2f& vec2);
	void SetFloat(const std::string& name, float value);
	void SetInt(const std::string& name, int value);
	void SetBool(const std::string & name, bool toggle);
private:
	bool IsCompiled(uint32_t shaderID);
	bool IsValidProgram();

	uint32_t m_VertShaderID;
	uint32_t m_FragShaderID;

	uint32_t m_ShaderProgramID;

	std::string m_VertShaderContent;
	std::string m_FragShaderContent;
};

