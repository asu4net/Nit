#pragma once

namespace Nit::InputSystem
{

    void Register();

    InputAction* CreateInputAction(const String& keyCode);
    bool DestroyInputAction(InputAction* inputAction);

    void OnControllerAnalog(const String& _keyname, int _controllerId, float _analogValue);
    void OnControllerVector2(const String& _keyname, int _controllerId, const Vector2& _vector2Value);
    void OnControllerVector3(const String& _keyname, int _controllerId, const Vector3& _vector3Value);
    void OnControllerVector4(const String& _keyname, int _controllerId, const Vector4& _vector4Value);
    void OnControllerButtonPressed(const String& _keyname, int _controllerId, bool _bIsRepeat);
    void OnControllerButtonReleased(const String& _keyname, int _controllerId, bool _bIsRepeat);

    void SendForceFeedback(int _controllerId, float _leftMotor, float _rightMotor);
}   