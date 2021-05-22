#pragma once

namespace ge2::gfx
{

    class ShaderProgram
    {
    public:
        ShaderProgram(const char* vertexSource, const char* fragmentSource);
        ShaderProgram(ShaderProgram const&) = delete;
        ShaderProgram(ShaderProgram&&) noexcept;
        ~ShaderProgram();
        ShaderProgram& operator=(ShaderProgram const&) = delete;
        ShaderProgram& operator=(ShaderProgram&&) noexcept;

        bool CompiledWithoutError();

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
    };

}