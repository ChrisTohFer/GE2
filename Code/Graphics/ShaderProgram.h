#pragma once

#include "Platform/Guid.h"

#include <string_view>

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

        bool CompiledWithoutError() const;

        unsigned int Id() const;
        void MakeActive() const;

        void SetUniform(const char* name, float value) const;
        void SetUniform(const char* name, bool value) const;
        void SetUniform(const char* name, int value) const;
        void SetUniform(const char* name, unsigned int value) const;
        void SetUniform(const char* name, double value) const;

    private:
        unsigned int m_id;
        int          m_success;
    };

    ShaderProgram const* ShaderProgramFromGuid(GUID vertGuid, GUID fragGuid);
    ShaderProgram const* ShaderProgramFromFilename(std::wstring_view const& vertexShader, std::wstring_view const& fragShader);

}