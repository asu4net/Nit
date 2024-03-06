#pragma once
#include "Entity\Entity.h"

namespace Nit
{
    class Framebuffer;
}

namespace Nit::ViewportWindow
{
    Vector2 GetMousePosition();
    void Show(bool* bShow, Framebuffer* TargetFramebuffer);
}