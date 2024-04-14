#pragma once

namespace Nit
{
    struct CCirclePrimitive;

    struct CircleComponent
    {
        bool             IsVisible    = true;
        float            Radius       = .5f;
        CColor            TintColor    = CColor::White;
        float            Thickness    = .05f;
        float            Fade         = .01f;
        int              SortingLayer = 0;
        CCirclePrimitive* Primitive    = nullptr;

        CircleComponent() = default;

        CircleComponent(float radius, const CColor& tintColor = CColor::White, float thickness = .05f, float fade = .01f, int sortingLayer = 0)
            : Radius(radius)
            , TintColor(tintColor)
            , Thickness(thickness)
            , Fade(fade)
        {
        }

        RTTR_ENABLE_NO_VIRTUAL
    };

    NIT_FORCE_LINK(CircleComponent)
}