#include "ShaderProgram.h"
#include "glad/glad.h"
#include "AssetManager/CommonLoaders.h"
#include "AssetManager/Metadata.h"

#include <fstream>
#include <iostream>
#include <map>

namespace
{
    using namespace ge2;
    using namespace gfx;

    ShaderProgram EmptyShader()
    {
        return ShaderProgram(NULL_GUID, NULL_GUID, "", "");
    }

    ShaderProgram ShaderProgramFromGuid(GUID vertGuid, GUID fragGuid)
    {
        //Check shaders exist
        if (vertGuid == NULL_GUID || fragGuid == NULL_GUID)
        {
            _ASSERT(false);         //Attempted to load a shader using a null guid - might have an incorrect filename
            return EmptyShader();
        }

        //Need to create the shaderprogram
        auto vertSource = assets::TextLoader::Instance().File(vertGuid);
        auto fragSource = assets::TextLoader::Instance().File(fragGuid);

        //Exit if source is missing (file doesn't exist or guid is wrong)
        if (vertSource == nullptr || fragSource == nullptr)
        {
            _ASSERT(false);     //Attmpted to load a shader with invalid guid - file doesn't exist or is wrong type
            return EmptyShader();
        }

        //Create
        return ShaderProgram(vertGuid, fragGuid, vertSource->c_str(), fragSource->c_str());
    }
}

namespace ge2::gfx
{

    ShaderProgram::ShaderProgram(GUID vertGuid, GUID fragGuid, const char* vertexSource, const char* fragmentSource)
        : m_vertGuid(vertGuid)
        , m_fragGuid(fragGuid)
        , m_id(0)
        , m_success(false)
    {
        //Vertex
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        if (vertexShader == 0)
        {
            std::cout << "Failed to create shader buffer - does this thread have access to openGL?\n";
            return;
        }

        glShaderSource(vertexShader, 1, &vertexSource, nullptr);
        glCompileShader(vertexShader);

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &m_success);
        if (!m_success)
        {
            char m_log[512];
            glGetShaderInfoLog(vertexShader, 512, NULL, m_log);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << m_log << std::endl;
            return;
        }

        //Fragment
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &m_success);
        if (!m_success)
        {
            char m_log[512];
            glGetShaderInfoLog(fragmentShader, 512, NULL, m_log);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << m_log << std::endl;
            return;
        }

        //Shaderprogram
        m_id = glCreateProgram();
        glAttachShader(m_id, vertexShader);
        glAttachShader(m_id, fragmentShader);
        glLinkProgram(m_id);

        glGetProgramiv(m_id, GL_LINK_STATUS, &m_success);
        if (!m_success) {
            char m_log[512];
            glGetProgramInfoLog(m_id, 512, NULL, m_log);
            std::cout << "ERROR::SHADER::PROGRAM_FAILED\n" << m_log << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
        : m_id(other.m_id)
        , m_success(other.m_success)
    {
        other.m_id = 0;
        other.m_success = false;
    }

    ShaderProgram::~ShaderProgram()
    {
        if (m_id != 0)
        {
            glDeleteProgram(m_id);
        }
    }

    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
    {
        if (m_id != 0)
        {
            glDeleteProgram(m_id);
        }

        m_id      = other.m_id;
        m_success = other.m_success;

        other.m_id = 0;
        other.m_success = false;

        return *this;
    }

    bool ShaderProgram::CompiledWithoutError() const
    {
        return m_success;
    }

    unsigned int ShaderProgram::Id() const
    {
        return m_id;
    }

    GUID ShaderProgram::VertGuid() const
    {
        return m_vertGuid;
    }

    GUID ShaderProgram::FragGuid() const
    {
        return m_fragGuid;
    }

    void ShaderProgram::MakeActive() const
    {
        glUseProgram(m_id);
    }

    void ShaderProgram::SetUniform(const char* name, float value) const
    {
        auto uniformLocation = glGetUniformLocation(m_id, name);
        glUseProgram(m_id);
        glUniform1f(uniformLocation, value);
    }

    void ShaderProgram::SetUniform(const char* name, bool value) const
    {
        auto uniformLocation = glGetUniformLocation(m_id, name);
        glUseProgram(m_id);
        glUniform1i(uniformLocation, int(value));
    }

    void ShaderProgram::SetUniform(const char* name, int value) const
    {
        auto uniformLocation = glGetUniformLocation(m_id, name);
        glUseProgram(m_id);
        glUniform1i(uniformLocation, value);
    }

    void ShaderProgram::SetUniform(const char* name, unsigned int value) const
    {
        auto uniformLocation = glGetUniformLocation(m_id, name);
        glUseProgram(m_id);
        glUniform1ui(uniformLocation, value);
    }

    void ShaderProgram::SetUniform(const char* name, double value) const
    {
        auto uniformLocation = glGetUniformLocation(m_id, name);
        glUseProgram(m_id);
        glUniform1d(uniformLocation, value);
    }

    //Shaderloader

    ShaderLoader ShaderLoader::g_loader;

    ShaderLoader::ShaderLoader()
        : Loader({ L".shad" })
    {
    }

    ShaderLoader const& ShaderLoader::Instance()
    {
        return g_loader;
    }

    ShaderProgram ShaderLoader::Load(std::wstring const& file) const
    {
        std::ifstream fileIn(file);

        if (fileIn.bad())
        {
            return EmptyShader();
        }

        GUID vertGuid, fragGuid;
        fileIn >> vertGuid >> fragGuid;

        return ShaderProgramFromGuid(vertGuid, fragGuid);
    }

}