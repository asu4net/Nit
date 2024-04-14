#include "NitRender/Render.h"

using namespace Nit;

int main()
{
    const TWindowPtr window = CWindow::Create();
    Render2D::Init(EGraphicsAPI::OpenGL);
    CRenderCommandQueue::Submit<SetClearColorCommand>(Render2D::GetRenderAPI(), CColor::DarkGrey);
    CSpritePrimitive primitive;
    
    while (window->IsOpened())
    {
        CRenderCommandQueue::Submit<ClearCommand>(Render2D::GetRenderAPI());
        Render2D::BeginBatch();
        Render2D::SubmitPrimitive<CSpritePrimitive>(primitive);
        Render2D::EndBatch();
        window->Update();
    }
}