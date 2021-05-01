#include "Temp.h"

#include "ShaderProgram.h"
#include "Texture.h"
#include "VertexArray.h"

#include "glad/glad.h"
#include "SFML/Window.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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
        ShaderProgram* shaderProgram = nullptr;
        Texture2D*     texture = nullptr;
        Texture2D*     texture2 = nullptr;
        VertexArray* vertexArray = nullptr;
    }
    void LoadTriangle()
    {
        std::vector<float> vertices = {
            // positions          // texture coords
             0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
             0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
        };
        std::vector<unsigned int> indices = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };

        std::vector<AttribPointer> attribPointers =
        {
            AttribPointer(0, 3),
            AttribPointer(1, 2)
        };
        vertexArray = new VertexArray(vertices, attribPointers, indices);
    }

    void LoadShaderProgram()
    {
        const char* vertexSource = "#version 460 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (location = 1) in vec2 aTexCoord;"
            "out vec2 TexCoord;"
            "uniform mat4 transform;"
            "void main()"
            "{"
            "TexCoord = aTexCoord;"
            "gl_Position = transform * vec4(aPos, 1.0);"
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

    void DrawTriangle(Vector3f position, Vector3f rotation)
    {
        shaderProgram->MakeActive();

        glm::mat4 transform(1.0f);
        transform = glm::translate(transform, glm::vec3(position.x, position.y, position.z));
        transform = glm::rotate(transform, rotation.z, glm::vec3(0, 0, 1));
        transform = glm::rotate(transform, rotation.y, glm::vec3(0, 1, 0));
        transform = glm::rotate(transform, rotation.x, glm::vec3(1, 0, 0));

        unsigned int transformLoc = glGetUniformLocation(shaderProgram->Id(), "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        texture->MakeActive(0);
        texture2->MakeActive(1);
        vertexArray->Draw();
    }

}