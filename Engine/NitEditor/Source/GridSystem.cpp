#include "GridSystem.h"
#include "Core/Engine.h"
#include "Render/Renderer.h"

namespace Nit::GridSystem
{
    // Registration stuff
    void OnCreate();
    void OnStart();
    void OnFinish();
    void OnDestroy();

    void Register()
    {
        Engine::CreateSystem("GridSystem", EditorExecutionOrder, ExecutionContext::Editor);
        Engine::SetSystemCallback(SystemStage::Create, &OnCreate);
        Engine::SetSystemCallback(SystemStage::Start, &OnStart);
        Engine::SetSystemCallback(SystemStage::Finish, &OnFinish);
        Engine::SetSystemCallback(SystemStage::Destroy, &OnDestroy);
    }

    // Actual code
    const Color GridColor = { Color::Grey.r, Color::Grey.g, Color::Grey.b, 0.5f };
    const int   GridLines = 30;
    const float LineLenght = 3.f;
    const float LineThickness = 0.005f;
    const float LinePadding = 0.1f;
    DynamicArray<LinePrimitive*> Lines;
    
    void SubmitGridLine(const Vector2& start, const Vector2& end)
    {
        LinePrimitive* line = Renderer::CreatePrimitive<LinePrimitive>();

        line->bIsVisible = true;
        line->Start      = start;
        line->End        = end;
        line->TintColor  = GridColor;
        line->Thickness  = LineThickness;
        line->SortingLayer = -Math::FloatMax();

        Lines.push_back(line);
    }

    void OnCreate()
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
            SubmitGridLine({ i * LinePadding, 0 }, { i * LinePadding, LineLenght });
        }

        for (int i = 0; i < GridLines; i++)
        {
            SubmitGridLine({ i * LinePadding, 0 }, { i * LinePadding,  -LineLenght });
        }

        for (int i = 0; i < GridLines; i++)
        {
            SubmitGridLine({ i * -LinePadding, 0 }, { i * -LinePadding, LineLenght });
        }

        for (int i = 0; i < GridLines; i++)
        {
            SubmitGridLine({ i * -LinePadding, 0 }, { i * -LinePadding,  -LineLenght });
        }
    }
    
    void OnDestroy()
    {
        for (LinePrimitive* line : Lines)
        {
            Renderer::DestroyPrimitive(line);
        }
        Lines.clear();
    }

    void OnStart()
    {
        for (LinePrimitive* line : Lines)
        {
            line->bIsVisible = false;
        }
    }

    void OnFinish()
    {
        for (LinePrimitive* line : Lines)
        {
            line->bIsVisible = true;
        }
    }
}