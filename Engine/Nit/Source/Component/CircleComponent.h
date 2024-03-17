#pragma once

namespace Nit
{
    struct CirclePrimitive;

    struct CircleComponent
    {
        bool             IsVisible    = true;
        float            Radius       = .5f;
        Color            TintColor    = Color::White;
        float            Thickness    = .05f;
        float            Fade         = .01f;
        int              SortingLayer = 0;
        CirclePrimitive* Primitive    = nullptr;

        CircleComponent() = default;

        CircleComponent(float radius, const Color& tintColor = Color::White, float thickness = .05f, float fade = .01f, int sortingLayer = 0)
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