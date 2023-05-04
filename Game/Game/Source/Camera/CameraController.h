#pragma once
#include <Nit.h>

class CameraController
{
public:
    CameraController(Nit::Camera* owner);
    virtual ~CameraController() = default;
    
    bool IsEnabled() const { return m_bEnabled; }
    void SetEnabled(const bool bEnabled) { m_bEnabled = bEnabled; }
    Nit::Camera* GetOwner() const { return m_Owner; }
    
    void Update(const float deltaTime);

protected:
    virtual void OnUpdate(const float deltaTime);
    
private:
    Nit::Camera* m_Owner;
    bool m_bEnabled;
};
