#pragma once

namespace Nit::AnimationSystem
{
    void Register();
    void OnAnimationComponentAdded(Registry&, const RawEntity entity);
    void OnCreate();
    void OnStart();
    void OnUpdate();
}