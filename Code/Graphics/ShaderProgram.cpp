#include "ShaderProgram.h"
#include "glad/glad.h"
#include "AssetManager/CommonLoaders.h"
#include "AssetManager/Metadata.h"

#include <iostream>
#include <map>

namespace ge2::gfx
{

    ShaderProgram::ShaderProgram(const char* vertexSource, const char* fragmentSource)
        : m_id(0)
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

    namespace
    {
        struct ShaderProgramMap
        {
            ~ShaderProgramMap()
            {
                for (auto& element : shaderPrograms)
                {
                    delete element.second;
                }
            }

            std::map<std::pair<GUID/*vert*/, GUID/*frag*/>, ShaderProgram*> shaderPrograms;
        };
        
        ShaderProgramMap shaders;
    }

    ShaderProgram const* ShaderProgramFromGuid(GUID vertGuid, GUID fragGuid)
    {
        //Check shaders exist
        if (vertGuid == NULL_GUID || fragGuid == NULL_GUID)
        {
            _ASSERT(false);         //Attempted to load a shader using a null guid - might have an incorrect filename
            return nullptr;
        }

        auto& shaderPrograms = shaders.shaderPrograms;

        auto iter = shaderPrograms.find(std::make_pair(vertGuid, fragGuid));
        if (iter == shaderPrograms.end())
        {
            //Need to create the shaderprogram
            auto vertSource = assets::TextLoader::Instance().File(vertGuid);
            auto fragSource = assets::TextLoader::Instance().File(fragGuid);
            
            //Exit if source is missing (file 
            if (vertSource == nullptr || fragSource == nullptr)
            {
                _ASSERT(false);     //Attmpted to load a shader with invalid guid - file doesn't exist or is wrong type
                return nullptr;
            }

            //Create
            shaderPrograms.emplace(std::make_pair(vertGuid, fragGuid), new ShaderProgram(vertSource->c_str(), fragSource->c_str()));
            iter = shaderPrograms.find(std::make_pair(vertGuid, fragGuid));
        }

        return iter->second;
    }

    ShaderProgram const* ShaderProgramFromFilename(std::wstring_view const& vertexShader, std::wstring_view const& fragShader)
    {
        auto vertGuid = assets::GUIDFromFilename(vertexShader);
        auto fragGuid = assets::GUIDFromFilename(fragShader);
        return ShaderProgramFromGuid(vertGuid, fragGuid);
    }

}