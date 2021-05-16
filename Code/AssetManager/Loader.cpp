#include "Loader.h"

ge2::ast::Loader::Loader(std::wstring const& extension)
    : m_extension(extension)
{
}

const std::wstring& ge2::ast::Loader::Extension() const
{
    return m_extension;
}
