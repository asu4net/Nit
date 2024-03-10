#pragma once

namespace Nit::InputSystem
{
    void Register();

    InputAction* CreateInputAction(KeyCode keyCode);    
    bool DestroyInputAction(InputAction* inputAction);
}