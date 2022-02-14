#include "Shader.h"
#include <GL/glcorearb.h>
#include <SDL2/SDL.h>
#include "Context.h"
#include "Uniform.h"

namespace GL
{

	ShaderModule::ShaderModule()
		: m_ShaderID(-1), m_Type(ShaderModuleType::VERTEX)
	{
	}

	ShaderModule::ShaderModule(const ShaderModuleType &type, std::string_view content)
	{
		Compile(type, content);
	}

	ShaderModule::~ShaderModule()
	{
		glDeleteShader(m_ShaderID);
	}

	bool ShaderModule::Compile(const ShaderModuleType &type, std::string_view content)
	{
		m_Type = type;
		m_ShaderID = glCreateShader(type);
		const char *vCode = content.data();
		glShaderSource(m_ShaderID, 1, &vCode, nullptr);
		glCompileShader(m_ShaderID);
		return IsCompiled();
	}

	const ShaderModuleType &ShaderModule::Type() const
	{
		return m_Type;
	}

	bool ShaderModule::IsCompiled()
	{
		int status;
		glGetShaderiv(m_ShaderID, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE)
		{
			char buffer[512];
			memset(buffer, 0, 512);
			glGetShaderInfoLog(m_ShaderID, 511, nullptr, buffer);
			SDL_Log("GLSL compile failed:%s\n", buffer);
			return false;
		}
		return true;
	}

	ShaderProgram::ShaderProgram()
	{
		m_ProgramID = glCreateProgram();
	}
	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_ProgramID);
	}

	void ShaderProgram::SetActive(bool isActive)
	{
		if (isActive)
			glUseProgram(m_ProgramID);
		else
			glUseProgram(0);
	}

	bool ShaderProgram::AttachShader(const ShaderModule &shader)
	{
		glAttachShader(m_ProgramID, shader.m_ShaderID);
		glLinkProgram(m_ProgramID);

		bool validFlag = IsValidProgram();

		if (validFlag)
		{
			PopulateAttributes();
			PopulateUniforms();
		}

		return validFlag;
	}

	uint32_t ShaderProgram::GetAttribute(std::string_view name) const
	{
		auto iter = m_ActiveAttributes.find(name.data());
		if (iter == std::end(m_ActiveAttributes))
		{
			SDL_Log("bad attrib index:%s", name.data());
			return 0;
		}
		return iter->second;
	}
	uint32_t ShaderProgram::GetUniform(std::string_view name) const
	{
		auto iter = m_ActiveUniforms.find(name.data());
		if (iter == std::end(m_ActiveUniforms))
		{
			SDL_Log("bad uniform index:%s", name.data());
			return 0;
		}
		return iter->second;
	}

	void ShaderProgram::PopulateAttributes()
	{
		m_ActiveAttributes.clear();

		int32_t count = -1;
		int32_t length;
		char attribName[128];
		int32_t size;
		uint32_t type;

		SetActive(true);

		glGetProgramiv(m_ProgramID, GL_ACTIVE_ATTRIBUTES, &count);

		for (size_t i = 0; i < count; ++i)
		{
			memset(attribName, 0, sizeof(char) * 128);
			glGetActiveAttrib(m_ProgramID, i, 128, &length, &size, &type, attribName);
			int attribIndex = glGetAttribLocation(m_ProgramID, attribName);

			if (attribIndex >= 0)
				m_ActiveAttributes[attribName] = attribIndex;
		}

		SetActive(false);
	}

	void ShaderProgram::PopulateUniforms()
	{
		m_ActiveUniforms.clear();

		int32_t count = -1;
		int32_t length;
		char uniformName[128];
		int32_t size;
		uint32_t type;

		SetActive(true);

		glGetProgramiv(m_ProgramID, GL_ACTIVE_UNIFORMS, &count);

		for (size_t i = 0; i < count; ++i)
		{
			memset(uniformName, 0, sizeof(char) * 128);
			glGetActiveUniform(m_ProgramID, i, 128, &length, &size, &type, uniformName);
			int uniformIndex = glGetUniformLocation(m_ProgramID, uniformName);
			if (uniformIndex >= 0)
			{
				m_ActiveUniforms[uniformName] = uniformIndex;
				
				std::string name = uniformName;
				std::size_t found = name.find('[');
				if (found != std::string::npos) //uniform array
				{
					name.erase(name.begin() + found, name.end());
					uint32_t index = 0;
					char subName[256];
					while (1)
					{
						memset(subName, 0, sizeof(char) * 256);
						sprintf_s(subName, "%s[%d]", name.c_str(), index++);
						int32_t location = glGetUniformLocation(m_ProgramID, subName);
						if (location < 0)
							break;
						m_ActiveUniforms[subName] = location;
					}
				}
			}
		}

		SetActive(false);
	}

	bool ShaderProgram::IsValidProgram()
	{
		GLint status;
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &status);
		if (status != GL_TRUE)
		{
			char buffer[512];
			memset(buffer, 0, 512);
			glGetProgramInfoLog(m_ProgramID, 511, nullptr, buffer);
			SDL_Log("GLSL link status:%s\n", buffer);
			return false;
		}
		return true;
	}

}