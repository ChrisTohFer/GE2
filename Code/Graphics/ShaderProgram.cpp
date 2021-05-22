#include "ShaderProgram.h"
#include "glad/glad.h"

#include <iostream>

namespace ge2::gfx
{

    ShaderProgram::ShaderProgram(const char* vertexSource, const char* fragmentSource)
        : m_id(0)
    {
        //Vertex
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
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

    bool ShaderProgram::CompiledWithoutError()
    {
        return m_success;
    }

    unsigned int ShaderProgram::Id() const
    {
        return m_id;
    }

    void ShaderProgram::MakeActive()
    {
        glUseProgram(m_id);
    }

    void ShaderProgram::SetUniform(const char* name, float value)
    {
        auto uniformLocation = glGetUniformLocation(m_id, name);
        glUseProgram(m_id);
        glUniform1f(uniformLocation, value);
    }

    void ShaderProgram::SetUniform(const char* name, bool value)
    {
        auto uniformLocation = glGetUniformLocation(m_id, name);
        glUseProgram(m_id);
        glUniform1i(uniformLocation, int(value));
    }

    void ShaderProgram::SetUniform(const char* name, int value)
    {
        auto uniformLocation = glGetUniformLocation(m_id, name);
        glUseProgram(m_id);
        glUniform1i(uniformLocation, value);
    }

    void ShaderProgram::SetUniform(const char* name, unsigned int value)
    {
        auto uniformLocation = glGetUniformLocation(m_id, name);
        glUseProgram(m_id);
        glUniform1ui(uniformLocation, value);
    }

    void ShaderProgram::SetUniform(const char* name, double value)
    {
        auto uniformLocation = glGetUniformLocation(m_id, name);
        glUseProgram(m_id);
        glUniform1d(uniformLocation, value);
    }

}