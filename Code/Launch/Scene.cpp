#include "Scene.h"
#include "Graphics/Core.h"

void ge2::Scene::Update()
{
}

void ge2::Scene::Draw(Camera const& camera, int screenX, int screenY)
{
    for (auto const& entity : entities)
    {
        ge2::gfx::AddRenderer(entity.transform, entity.renderer);
    }
    ge2::gfx::Draw(camera, screenX, screenY);
}
