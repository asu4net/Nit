#pragma once
#include <ShardGraphics.h>

class CameraController
{
public:
    CameraController(Shard::Graphics::Camera* owner);
    virtual ~CameraController() = default;
    
    bool IsEnabled() const { return m_bEnabled; }
    void SetEnabled(const bool bEnabled) { m_bEnabled = bEnabled; }
    Shard::Graphics::Camera* GetOwner() const { return m_Owner; }
    
    void Update(const float deltaTime);

protected:
    virtual void OnUpdate(const float deltaTime);
    
private:
    Shard::Graphics::Camera* m_Owner;
    bool m_bEnabled;
};
