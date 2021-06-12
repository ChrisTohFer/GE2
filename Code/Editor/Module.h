#pragma once

#include <string>

namespace ge2::ed
{
    class Module
    {
    public:
        virtual ~Module() = default;

        void Draw();
        virtual void DrawContent() = 0;

        bool minimised;
        std::string name;
    };
}