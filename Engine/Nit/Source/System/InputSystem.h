#pragma once

namespace Nit::InputSystem
{
    void Register();

    InputAction* RegisterInputAction(KeyCode keyCode);    
    bool UnregisterInputAction(InputAction* inputAction);
}