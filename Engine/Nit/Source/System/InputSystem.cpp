#include "InputSystem.h"
#include "Core\Engine.h"
#include "Input\Input.h"
#include "Input\InputAction.h"

namespace Nit::InputSystem
{
    DynamicArray<InputAction*> m_InputActions;
    
    void OnUpdate();
    void OnDestroy();

    void Register()
    {
        Engine::CreateSystem("InputSystem", InputExecutionOrder, ExecutionContext::Always);
        Engine::SetSystemCallback(SystemStage::Update, &OnUpdate);
        Engine::SetSystemCallback(SystemStage::Destroy, &OnDestroy);
    }

    InputAction* RegisterInputAction(KeyCode keyCode)
    {
        InputAction* inputAction = new InputAction(keyCode);
        m_InputActions.push_back(inputAction);
        return inputAction;
    }

    bool UnregisterInputAction(InputAction* inputAction)
    {
        if (!inputAction)
        {
            return false;
        }

        auto it = std::find(m_InputActions.begin(), m_InputActions.end(), inputAction);

        if (!*it)
        {
            return false;
        }

        m_InputActions.erase(it);
        return true;
    }
    
    void OnUpdate()
    {
        if (!Input::IsEnabled()) return;

        for (InputAction* inputAction : m_InputActions)
        {
            inputAction->Update();
        }
    }

    void OnDestroy()
    {
        for (InputAction* inputAction : m_InputActions)
        {
            delete inputAction;
        }
        m_InputActions.clear();
    }
}