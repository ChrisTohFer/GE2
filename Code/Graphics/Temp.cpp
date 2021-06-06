#include "Temp.h"

#include "Loaders.h"
#include "Renderer.h"
#include "ShaderProgram.h"
#include "Shapes.h"
#include "Texture.h"
#include "VertexArray.h"

#include "AssetManager/Assets.h"

#include "glad/glad.h"
#include "SFML/Window.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace ge2::gfx
{
    using namespace plat;

    namespace {
        ShaderProgram* shaderProgram = nullptr;
        VertexArray* vertexArray = nullptr;
        glm::mat4      cameraTransform;
        int            screenWidth, screenHeight;
        Shapes shapesSingleton;
        JPGLoader jpgLoader;
        PNGLoader pngLoader;
    }

    void Init()
    {
        gladLoadGLLoader((GLADloadproc)sf::Context::getFunction);
        glEnable(GL_DEPTH_TEST);
        shapesSingleton.Initialise();
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
        screenWidth = sizeX;
        screenHeight = sizeY;
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
            "uniform mat4 model;"
            "uniform mat4 camera;"
            "void main()"
            "{"
            "TexCoord = aTexCoord;"
            "gl_Position = camera * model * vec4(aPos, 1.0);"
            "}\0";
        const char* fragmentSource = "#version 460 core\n"
            "out vec4 FragColor;"
            "in  vec2 TexCoord;"
            "uniform sampler2D tex;"
            "void main()"
            "{"
            "FragColor = texture(tex, TexCoord);"
            "}\0";
        shaderProgram = new ShaderProgram(vertexSource, fragmentSource);
        shaderProgram->SetUniform("tex", 0);
    }

    void DrawTriangle(Vector3f position, Vector3f rotation)
    {
        shaderProgram->MakeActive();

        glm::mat4 transform(1.0f);
        transform = glm::translate(transform, glm::vec3(position.x, position.y, position.z));
        transform = glm::rotate(transform, rotation.z, glm::vec3(0, 0, 1));
        transform = glm::rotate(transform, rotation.y, glm::vec3(0, 1, 0));
        transform = glm::rotate(transform, rotation.x, glm::vec3(1, 0, 0));
        unsigned int transformLoc = glGetUniformLocation(shaderProgram->Id(), "model");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        transformLoc = glGetUniformLocation(shaderProgram->Id(), "camera");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(cameraTransform));

        jpgLoader.textures[0].MakeActive(0);
        pngLoader.textures[0].MakeActive(1);
        shapesSingleton.Cube().Draw();
    }


    static glm::mat4 Convert(Matrix4x4f m)
    {
        return {
            {m.values[0][0], m.values[1][0], m.values[2][0], m.values[3][0]},
            {m.values[0][1], m.values[1][1], m.values[2][1], m.values[3][1]},
            {m.values[0][2], m.values[1][2], m.values[2][2], m.values[3][2]},
            {m.values[0][3], m.values[1][3], m.values[2][3], m.values[3][3]}
        };
    }


    void DrawTriangle(Transform const& t)
    {
        shaderProgram->MakeActive();

        glm::mat4 transform = Convert(t.Matrix());
        unsigned int transformLoc = glGetUniformLocation(shaderProgram->Id(), "model");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        transformLoc = glGetUniformLocation(shaderProgram->Id(), "camera");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(cameraTransform));

        jpgLoader.textures[0].MakeActive(0);
        pngLoader.textures[0].MakeActive(1);
        shapesSingleton.Cube().Draw();
    }

    void UpdateCamera(Camera const& camera)
    {
        cameraTransform = Convert(camera.Matrix());
        cameraTransform = glm::perspective(glm::radians(45.f), float(screenWidth) / float(screenHeight), 0.1f, 100.f) * cameraTransform;
    }

}