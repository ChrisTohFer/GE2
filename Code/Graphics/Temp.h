#pragma once

#include "Platform/Window.h"

namespace ge2::gfx
{
    using namespace plat;

    class ShaderProgram
    {
    public:
        ShaderProgram(const char* vertexSource, const char* fragmentSource);
        ~ShaderProgram();

        bool CompiledWithoutError();
        const char* ErrorLog();

        unsigned int Id() const;
        void MakeActive();

        void SetUniform(const char* name, float value);
        void SetUniform(const char* name, bool value);
        void SetUniform(const char* name, int value);
        void SetUniform(const char* name, unsigned int value);
        void SetUniform(const char* name, double value);

    private:
        unsigned int m_id;
        int          m_success;
        char*        m_log = nullptr;
    };

    class Texture2D
    {
    public:
        Texture2D(const char* filename, bool hasAlphaChannel);
        ~Texture2D();

        void MakeActive(int unit = 0);

    private:
        unsigned int m_id;
        int          m_width;
        int          m_height;
        int          m_nChannels;
        int          m_success;
    };

    void Init();

    void ClearColour();
    void Display(Window::WindowKey const&);
    void Update(int sizeX, int sizeY);

    void LoadTriangle();
    void LoadShaderProgram();
    void LoadTexture();
    void DrawTriangle();

}