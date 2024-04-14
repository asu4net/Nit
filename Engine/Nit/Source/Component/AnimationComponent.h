#pragma once
#include "Asset\AssetRef.h"

namespace Nit
{
    struct AnimationComponent
    {
        AssetRef CurrentAnimation;
        bool PlayOnStart = true;
        uint32_t CurrentIndex = 0;
        float CurrentTime = 0.f;
        bool LoopEnabled = true;
        bool IsPlaying = false;

        AnimationComponent() = default;

        AnimationComponent(const AssetRef& animationRef)
            : CurrentAnimation(animationRef)
        {}

        AnimationComponent(const TString& animationAssetName);

        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(AnimationComponent)
}