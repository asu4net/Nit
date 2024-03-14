#include "GridSystem.h"
#include "Core/Engine.h"
#include "Render/Renderer.h"

namespace Nit::GridSystem
{
    // Registration stuff
    void OnPreDrawPrimitives();
    
    void Register()
    {
        Engine::CreateSystem("GridSystem", 500, ExecutionContext::Editor);
        Engine::SetSystemCallback(SystemStage::PreDrawPrimitives, &OnPreDrawPrimitives);
    }

    // Actual code
    const Color GridColor = { Color::Grey.r, Color::Grey.g, Color::Grey.b, 0.5f };
    const int   GridLines = 30;
    const float LineLenght = 3.f;
    const float LineThickness = 0.005f;
    const float LinePadding = 0.1f;

    void SubmitGridLine(const Vector2& start, const Vector2& end, bool bIsVertical = false)
    {
        SpritePrimitive p;
        p.StartPosition = start;
        p.EndPosition = end;
        p.TintColor = GridColor;
        p.NormalVector = bIsVertical ? Vector2::Right : Vector2::Up;
        p.Thickness = LineThickness;
        p.GenerateVertexData(SpriteShape::Line);
        Renderer::SubmitSpritePrimitive(p);
    }
    
    void OnPreDrawPrimitives()
    {
        if (!Engine::IsPaused() || !Engine::IsStopped())
            return;

        for (int i = 0; i < GridLines; i++)
        {
            SubmitGridLine({ 0, i * LinePadding }, { LineLenght, i * LinePadding });
        }

        for (int i = 0; i < GridLines; i++)
        {
            SubmitGridLine({ 0, i * LinePadding }, {-LineLenght, i * LinePadding });
        }

        for (int i = 0; i < GridLines; i++)
        {
            SubmitGridLine({ 0, i * -LinePadding }, { LineLenght, i * -LinePadding });
        }

        for (int i = 0; i < GridLines; i++)
        {
            SubmitGridLine({ 0, i * -LinePadding }, { -LineLenght, i * -LinePadding });
        }

        for (int i = 0; i < GridLines; i++)
        {
            SubmitGridLine({ i * LinePadding, 0 }, { i * LinePadding, LineLenght }, true);
        }

        for (int i = 0; i < GridLines; i++)
        {
            SubmitGridLine({ i * LinePadding, 0 }, { i * LinePadding,  -LineLenght }, true);
        }

        for (int i = 0; i < GridLines; i++)
        {
            SubmitGridLine({ i * -LinePadding, 0 }, { i * -LinePadding, LineLenght }, true);
        }

        for (int i = 0; i < GridLines; i++)
        {
            SubmitGridLine({ i * -LinePadding, 0 }, { i * -LinePadding,  -LineLenght }, true);
        }
    }
}