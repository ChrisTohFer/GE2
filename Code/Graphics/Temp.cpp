#include "Temp.h"

#include "glad/glad.h"
#include "SFML/Window.hpp"
#include "stb_image.h"

#include <iostream>

namespace ge2::gfx
{
    using namespace plat;

    void Init()
    {
        gladLoadGLLoader((GLADloadproc)sf::Context::getFunction);
    }

    void ClearColour()
    {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Display(Window::WindowKey const& key)
    {
        key.Window()->display();
    }

    void Update(int sizeX, int sizeY)
    {
        glViewport(0, 0, sizeX, sizeY);
    }

    namespace {
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        ShaderProgram* shaderProgram = nullptr;
        Texture2D*     texture = nullptr;
        Texture2D*     texture2 = nullptr;
    }
    void LoadTriangle()
    {
        constexpr float vertices[] = {
            // positions          // texture coords
             0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
             0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
        };
        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    void LoadShaderProgram()
    {
        const char* vertexSource = "#version 460 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (location = 1) in vec2 aTexCoord;"
            "out vec2 TexCoord;"
            "void main()"
            "{"
            "TexCoord = aTexCoord;"
            "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
            "}\0";
        const char* fragmentSource = "#version 460 core\n"
            "out vec4 FragColor;"
            "in  vec2 TexCoord;"
            "uniform sampler2D texture1;"
            "uniform sampler2D texture2;"
            "void main()"
            "{"
            "FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);"
            "}\0";
        shaderProgram = new ShaderProgram(vertexSource, fragmentSource);
        shaderProgram->SetUniform("texture1", 0);
        shaderProgram->SetUniform("texture2", 1);
    }

    void LoadTexture()
    {
        texture = new Texture2D("Textures\\container.jpg", false);
        texture2 = new Texture2D("Textures\\awesomeface.png", true);
    }

    void DrawTriangle()
    {
        shaderProgram->MakeActive();
        texture->MakeActive(0);
        texture2->MakeActive(1);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    ShaderProgram::ShaderProgram(const char* vertexSource, const char* fragmentSource)
    {
        //Vertex
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        glCompileShader(vertexShader);

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &m_success);
        if (!m_success)
        {
            m_log = new char[512];
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
            m_log = new char[512];
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
            m_log = new char[512];
            glGetProgramInfoLog(m_id, 512, NULL, m_log);
            std::cout << "ERROR::SHADER::PROGRAM_FAILED\n" << m_log << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(m_id);
        delete m_log;
    }

    bool ShaderProgram::CompiledWithoutError()
    {
        return m_success;
    }

    const char* ShaderProgram::ErrorLog()
    {
        return m_log ? m_log : "";
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

    Texture2D::Texture2D(const char* filename, bool hasAlphaChannel)
    {
        stbi_set_flip_vertically_on_load(1);
        unsigned char* data = stbi_load(filename, &m_width, &m_height, &m_nChannels, 0);

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (data)
        {
            m_success = true;

            if (hasAlphaChannel)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            m_success = false;
            
            std::cout << "ERROR::TEXTURE::LOAD_FAILED\n" << stbi_failure_reason() << std::endl;
        }

        stbi_image_free(data);
    }

    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &m_id);
    }

    void Texture2D::MakeActive(int unit)
    {
        _ASSERT(unit >= 0 && unit < 16);    //Texture unit might be out of bounds
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

}