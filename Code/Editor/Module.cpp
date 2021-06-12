#include "Module.h"
#include "imgui.h"

namespace ge2::ed
{
    void Module::Draw()
    {
        if (ImGui::CollapsingHeader(name.c_str()))
        {
            DrawContent();
        }
    }
}