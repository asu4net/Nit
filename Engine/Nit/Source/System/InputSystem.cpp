#include "InputSystem.h"
#include "Core\Engine.h"
#include "Input\Input.h"
#include "Input\InputAction.h"
#include "KeyboardInputSystem.h"
#include "WiimoteInputSystem.h"


namespace Nit::InputSystem
{
    // TODO: take into account different local players
    Map<String, InputAction*> m_InputActions;
    
    void OnUpdate();
    void OnDestroy();

    void Register()
    {
        Engine::CreateSystem("InputSystem", InputExecutionOrder, ExecutionContext::Always);
        // Engine::SetSystemCallback(SystemStage::Update, &OnUpdate);
        Engine::SetSystemCallback(SystemStage::Destroy, &OnDestroy);

        Keyboard::Register();
        Wiimote::Register();
    }

    InputAction* CreateInputAction(const String& keyCode)
    {
        if (m_InputActions.contains(keyCode)) return m_InputActions[keyCode];

        InputAction* inputAction = new InputAction(keyCode);
        m_InputActions[keyCode] = inputAction;
        return inputAction;
    }

    bool DestroyInputAction(InputAction* inputAction)
    {
        if (!inputAction)
        {
            return false;
        }

        m_InputActions.erase(inputAction->GetKeyCode());
        delete inputAction;

        return true;
    }
    
    void OnUpdate()
    {
        if (!Input::IsEnabled()) return;

        /*
        for (std::pair<const String, InputAction*> inputActionPair : m_InputActions)
        {
            inputActionPair.second->Update();
        }
        */
    }

    void OnDestroy()
    {
        for (std::pair<const String, InputAction*> inputActionPair : m_InputActions)
        {
            delete inputActionPair.second;
        }
        m_InputActions.clear();
    }

    void OnControllerAnalog(const String& _keyname, int _controllerId, float _analogValue)
    {
        if(!m_InputActions.contains(_keyname)) return;

        m_InputActions[_keyname]->OnControllerAnalog(_controllerId, _analogValue);
    }

    void OnControllerVector2(const String& _keyname, int _controllerId, const Vector2& _vector2Value)
    {
        if (!m_InputActions.contains(_keyname)) return;

        m_InputActions[_keyname]->OnControllerVector2(_controllerId, _vector2Value);
    }

    void OnControllerVector3(const String& _keyname, int _controllerId, const Vector3& _vector3Value)
    {
        if (!m_InputActions.contains(_keyname)) return;

        m_InputActions[_keyname]->OnControllerVector3(_controllerId, _vector3Value);
    }

    void OnControllerVector4(const String& _keyname, int _controllerId, const Vector4& _vectorValue)
    {
        if (!m_InputActions.contains(_keyname)) return;

        m_InputActions[_keyname]->OnControllerVector4(_controllerId, _vectorValue);
    }

    void OnControllerButtonPressed(const String& _keyname, int _controllerId, bool _bIsRepeat)
    {
        if (!m_InputActions.contains(_keyname)) return;

        m_InputActions[_keyname]->OnControllerButtonPressed(_controllerId, _bIsRepeat);
    }

    void OnControllerButtonReleased(const String& _keyname, int _controllerId, bool _bIsRepeat)
    {
        if (!m_InputActions.contains(_keyname)) return;

        m_InputActions[_keyname]->OnControllerButtonReleased(_controllerId, _bIsRepeat);
    }

    void SendForceFeedback(int _controllerId, float _leftMotor, float _rightMotor)
    {
        if(Wiimote::AnyWiimoteConnected())
        {
            Wiimote::SendForceFeedback(_controllerId, _leftMotor, _rightMotor);
        }
    }

}