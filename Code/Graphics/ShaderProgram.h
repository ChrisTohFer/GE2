#pragma once

#include "AssetManager/Loader.h"
#include "Platform/Guid.h"

#include <string_view>

namespace ge2::gfx
{

    class ShaderProgram
    {
    public:
        ShaderProgram(GUID vertGuid, GUID fragGuid, const char* vertexSource, const char* fragmentSource);
        ShaderProgram(ShaderProgram const&) = delete;
        ShaderProgram(ShaderProgram&&) noexcept;
        ~ShaderProgram();
        ShaderProgram& operator=(ShaderProgram const&) = delete;
        ShaderProgram& operator=(ShaderProgram&&) noexcept;

        bool CompiledWithoutError() const;

        unsigned int Id() const;
        GUID VertGuid() const;
        GUID FragGuid() const;
        void MakeActive() const;

        void SetUniform(const char* name, float value) const;
        void SetUniform(const char* name, bool value) const;
        void SetUniform(const char* name, int value) const;
        void SetUniform(const char* name, unsigned int value) const;
        void SetUniform(const char* name, double value) const;

    private:
        GUID         m_vertGuid;
        GUID         m_fragGuid;
        unsigned int m_id;
        int          m_success;
    };

    //Loader
    class ShaderLoader : public assets::Loader<ShaderProgram, 1>
    {
    public:
        ShaderLoader();

        static ShaderLoader const& Instance();
    private:
        ShaderProgram Load(std::wstring const& file) const override;

        static ShaderLoader g_loader;
    };
}