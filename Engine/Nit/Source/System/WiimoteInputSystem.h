#pragma once

namespace Nit::InputSystem::Wiimote
{

    void Register();
    void ScanWiimotes();
    bool AnyWiimoteConnected();
    
    void SendForceFeedback(int _controllerId, float _leftMotor, float _rightMotor);
}