#include "Scene.h"

void ge2::Scene::Update()
{
}

void ge2::Scene::Draw(Camera const& camera, int screenX, int screenY)
{
    graphics.Draw(camera, screenX, screenY);
}
