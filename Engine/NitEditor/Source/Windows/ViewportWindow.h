#pragma once
#include "Entity\Entity.h"

namespace Nit
{
    class CFramebuffer;
}

namespace Nit::ViewportWindow
{
    CVector2 GetMousePosition();
    void Show(bool* bShow, CFramebuffer* TargetFramebuffer);
}