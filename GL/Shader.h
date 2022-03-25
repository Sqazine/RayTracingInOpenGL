#pragma once
#include <string>
#include <map>
#include <glad/glad.h>
#include "Uniform.h"
#include "../Vector2.h"
#include "../Vector3.h"
#include "../Vector4.h"

namespace GL
{
    enum ShaderModuleType
    {
        VERTEX = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,
        GEOMETRY= GL_GEOMETRY_SHADER,
        TESSELLATION_CONTROL = GL_TESS_CONTROL_SHADER,
        TESSELLATION_EVAL = GL_TESS_EVALUATION_SHADER,
        COMPUTE = GL_COMPUTE_SHADER,
    };
    class ShaderModule
    {
    public:
        ShaderModule();
        ShaderModule(const ShaderModuleType &type, std::string_view content);
        ~ShaderModule();

        bool Compile(const ShaderModuleType &type, std::string_view content);

        const ShaderModuleType &Type() const;

    private:
        bool IsCompiled();

        friend class ShaderProgram;

        uint32_t mShaderID;
        ShaderModuleType mType;
    };

    class ShaderProgram //shader程序体
    {
    public:
        ShaderProgram();
        ~ShaderProgram();

        void SetActive(bool isActive);

        template <typename T>
        void SetUniformValue(std::string_view name, T value);

        template <typename T>
        void SetUniformArray(std::string_view name, const std::vector<T> &valueArray);

        bool AttachShader(const ShaderModule &shader);

        uint32_t GetAttribute(std::string_view name) const;
        uint32_t GetUniform(std::string_view name) const;

    private:
        void PopulateAttributes();
        void PopulateUniforms();

        std::map<std::string, uint32_t> mActiveAttributes;
        std::map<std::string, uint32_t> mActiveUniforms;

        bool IsValidProgram();
        uint32_t mProgramID;
    };

#define SHADER_PROGRAM_SET_VALUE(type)                                            \
    template <>                                                                   \
    inline void ShaderProgram::SetUniformValue(std::string_view name, type value) \
    {                                                                             \
        Uniform<type>::Set(GetUniform(name.data()), value);                       \
    }

#define SHADER_PROGRAM_SET_VALUEARRAY(type)                                                                \
    template <>                                                                                            \
    inline void ShaderProgram::SetUniformArray(std::string_view name, const std::vector<type> &valueArray) \
    {                                                                                                      \
        Uniform<type>::Set(GetUniform(name.data()), valueArray);                                           \
    }

    SHADER_PROGRAM_SET_VALUE(int32_t)
    SHADER_PROGRAM_SET_VALUE(uint32_t)
    SHADER_PROGRAM_SET_VALUE(float)
    SHADER_PROGRAM_SET_VALUE(double)

    SHADER_PROGRAM_SET_VALUE(Vector2i32)
    SHADER_PROGRAM_SET_VALUE(Vector2u32)
    SHADER_PROGRAM_SET_VALUE(Vector2f)
    SHADER_PROGRAM_SET_VALUE(Vector2d)

    SHADER_PROGRAM_SET_VALUE(Vector3i32)
    SHADER_PROGRAM_SET_VALUE(Vector3u32)
    SHADER_PROGRAM_SET_VALUE(Vector3f)
    SHADER_PROGRAM_SET_VALUE(Vector3d)

    SHADER_PROGRAM_SET_VALUE(Vector4i32)
    SHADER_PROGRAM_SET_VALUE(Vector4u32)
    SHADER_PROGRAM_SET_VALUE(Vector4f)
    SHADER_PROGRAM_SET_VALUE(Vector4d)

    SHADER_PROGRAM_SET_VALUEARRAY(int32_t)
    SHADER_PROGRAM_SET_VALUEARRAY(uint32_t)
    SHADER_PROGRAM_SET_VALUEARRAY(float)
    SHADER_PROGRAM_SET_VALUEARRAY(double)

    SHADER_PROGRAM_SET_VALUEARRAY(Vector2i32)
    SHADER_PROGRAM_SET_VALUEARRAY(Vector2u32)
    SHADER_PROGRAM_SET_VALUEARRAY(Vector2f)
    SHADER_PROGRAM_SET_VALUEARRAY(Vector2d)

    SHADER_PROGRAM_SET_VALUEARRAY(Vector3i32)
    SHADER_PROGRAM_SET_VALUEARRAY(Vector3u32)
    SHADER_PROGRAM_SET_VALUEARRAY(Vector3f)
    SHADER_PROGRAM_SET_VALUEARRAY(Vector3d)

    SHADER_PROGRAM_SET_VALUEARRAY(Vector4i32)
    SHADER_PROGRAM_SET_VALUEARRAY(Vector4u32)
    SHADER_PROGRAM_SET_VALUEARRAY(Vector4f)
    SHADER_PROGRAM_SET_VALUEARRAY(Vector4d)
}