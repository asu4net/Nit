#pragma once
#include "Entity\Entity.h"

namespace Nit
{
    class Framebuffer;
}

namespace Nit::ViewportWindow
{
    void Show(bool* bShow, Framebuffer* TargetFramebuffer);
}