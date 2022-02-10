#include "GLShader.h"
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <SDL2/SDL.h>
GLShader::GLShader(const std::string &vertName, const std::string &fragName)
	: m_VertShaderID(0), m_FragShaderID(0)
{
	m_VertShaderContent = Utils::LoadText(vertName);
	m_FragShaderContent = Utils::LoadText(fragName);
	m_ShaderProgramID = glCreateProgram();
}

GLShader::~GLShader()
{
	glDeleteProgram(m_ShaderProgramID);
}

bool GLShader::Compile()
{

	m_VertShaderID = glCreateShader(GL_VERTEX_SHADER);
	const char *vCode = m_VertShaderContent.c_str();
	glShaderSource(m_VertShaderID, 1, &vCode, nullptr);
	glCompileShader(m_VertShaderID);
	if (!IsCompiled(m_VertShaderID))
	{
		SDL_Log("failed to compile shader %s", m_VertShaderContent.c_str());
		return false;
	}

	m_FragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	const char *fCode = m_FragShaderContent.c_str();
	glShaderSource(m_FragShaderID, 1, &fCode, nullptr);
	glCompileShader(m_FragShaderID);
	if (!IsCompiled(m_FragShaderID))
	{
		SDL_Log("failed to compile shader %s", m_FragShaderContent.c_str());
		return false;
	}

	glAttachShader(m_ShaderProgramID, m_VertShaderID);
	glAttachShader(m_ShaderProgramID, m_FragShaderID);
	glLinkProgram(m_ShaderProgramID);
	if (!IsValidProgram())
	{
		SDL_Log("failed to link shader program");
		return false;
	}

	glDeleteShader(m_VertShaderID);
	glDeleteShader(m_FragShaderID);

	return true;
}

void GLShader::SetActive(bool isActive)
{
	if (isActive)
		glUseProgram(m_ShaderProgramID);
	else
		glUseProgram(0);
}

void GLShader::SetVector4(const std::string &name, const Vector4f &vec)
{
	glUniform4fv(glGetUniformLocation(m_ShaderProgramID, name.c_str()), 1, vec.values.data());
}
void GLShader::SetVector3(const std::string &name, const Vector3f &vec3)
{
	glUniform3fv(glGetUniformLocation(m_ShaderProgramID, name.c_str()), 1, vec3.values.data());
}
void GLShader::SetVector2(const std::string &name, const Vector2f &vec2)
{
	glUniform2fv(glGetUniformLocation(m_ShaderProgramID, name.c_str()), 1, vec2.values.data());
}
void GLShader::SetFloat(const std::string &name, float value)
{
	glUniform1f(glGetUniformLocation(m_ShaderProgramID, name.c_str()), value);
}
void GLShader::SetInt(const std::string &name, int value)
{
	glUniform1i(glGetUniformLocation(m_ShaderProgramID, name.c_str()), value);
}
void GLShader::SetBool(const std::string &name, bool toggle)
{
	glUniform1i(glGetUniformLocation(m_ShaderProgramID, name.c_str()), toggle);
}

bool GLShader::IsCompiled(uint32_t shaderID)
{
	int status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shaderID, 511, nullptr, buffer);
		SDL_Log("GLSL compile failed:\n%s", buffer);
		return false;
	}
	return true;
}

bool GLShader::IsValidProgram()
{
	GLint status;
	glGetProgramiv(m_ShaderProgramID, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(m_ShaderProgramID, 511, nullptr, buffer);
		SDL_Log("GLSL link status:\n%s", buffer);
		return false;
	}
	return true;
}
