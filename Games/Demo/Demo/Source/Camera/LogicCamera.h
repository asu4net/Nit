#pragma once
#include <ShardGraphics.h>
#include <vector>
#include "CameraController.h"

class LogicCamera : public Shard::Graphics::Camera
{
public:
    LogicCamera(const std::shared_ptr<Shard::Graphics::Window>& window,
        const Projection startProjection = Projection::Perspective);
    
    ~LogicCamera() override;

    void Update(const float deltaTime = 0.02f);

    Camera GetRenderData();
    
    template<typename T>
    void AddController()
    {
        T* controller = new T(this);
        m_Controllers.push_back(controller);
        controller->SetEnabled(true);
    }

    template<typename T>
    void SetActiveController()
    {
        T* controller = GetController<T>();
        if (!controller) return;
        auto it = std::find(m_Controllers.begin(), m_Controllers.end(), controller);
        m_Controllers.erase(it);
        m_Controllers.push_back(controller);
    }

    template<typename T>
    void RemoveController()
    {
        T* controller = GetController<T>();
        if (!controller) return;
        auto it = std::find(m_Controllers.begin(), m_Controllers.end(), controller);
        m_Controllers.erase(it);
        delete controller;
    }
    
    template<typename T>
    T* GetController()
    {
        for (CameraController* controller : m_Controllers)
        {
            if (T* castedCtrl = dynamic_cast<T*>(controller))
            {
                return castedCtrl;
            }
        }
        return nullptr;
    }
    
    CameraController* GetActiveController() const
    {
        if (m_Controllers.empty())
            return nullptr;
        return m_Controllers[0];
    }
    
private:
    std::vector<CameraController*> m_Controllers;
};