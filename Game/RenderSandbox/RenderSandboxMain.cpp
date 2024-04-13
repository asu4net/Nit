#include "NitRender/Render.h"

using namespace Nit;
using namespace Nit::Render;

int main()
{   
    WindowPtr window = Window::Create();
    Renderer2D::Init(GraphicsAPI::OpenGL);
    Renderer2D::GetRenderSystemMap().CreateRenderSystem<SpriteRenderSystem>();
    CommandQueue::Submit<SetClearColorCommand>(Renderer2D::GetRenderAPI(), Color::DarkGrey);
    
    while (window->IsOpened())
    {
        CommandQueue::Submit<ClearCommand>(Renderer2D::GetRenderAPI());
        Renderer2D::StartBatch();
        SpritePrimitive primitive;
        Renderer2D::GetRenderSystemMap().GetRenderSystem<SpriteRenderSystem>("SpriteRenderSystem").SubmitPrimitive(primitive);
        Renderer2D::Invalidate();
        window->Update();
    }
}