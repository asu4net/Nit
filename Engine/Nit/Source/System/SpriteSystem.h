#pragma once

namespace Nit::SpriteSystem
{
    void Register();
    void OnCreate();
    void OnSpriteComponentAdded(Registry&, RawEntity entity);
    void OnPreDrawPrimitives();
}