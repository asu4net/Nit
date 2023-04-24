#include "GraphicApplication.h"

using namespace Nit;
using namespace Nit;

class DemoGame : public Game
{
public:
    std::shared_ptr<Texture> GridTexture;
    std::shared_ptr<Texture> CatTexture;
    std::shared_ptr<Texture> CppTexture;
    glm::vec3 BallPosition = {.5f, .3f, 0.f};
    Quad Grid;

protected:
    void OnCreate() override
    {
        GridTexture = Texture2D::Create("Content/Textures/Checkerboard.png", {MagFilter::Nearest});
        CatTexture = Texture2D::Create("Content/Textures/bola.jpg");
        CppTexture = Texture2D::Create("Content/Textures/cpp.png");
        
        // GetRootWidget()->PushWidget<Vector3Widget>(BallPosition, "Cat Pos");
        
        Grid.Texture = GridTexture;
        Grid.Size *= 30;
        Grid.UVScale *= 30;
        Grid.Color = Global::DarkGreyColor;
    }

    void OnUpdate(float deltaTime) override
    {
        Renderer2D.SetRenderData({GetCamera()->GetRenderData()});

        Renderer2D.SetBlendingMode(BlendingMode::Alpha);
        
        Renderer2D.Begin();
        Renderer2D.SubmitQuad(Grid);
        Renderer2D.SubmitQuad({glm::translate(Global::IdentityMatrix, {1, 0, 0}), Global::LightRedColor});
        Renderer2D.SubmitQuad({glm::translate(Global::IdentityMatrix, {0, 1, 0}), Global::YellowColor});
        Renderer2D.SubmitQuad({Global::IdentityMatrix, Global::LightBlueColor});
        Renderer2D.SubmitQuad({glm::translate(Global::IdentityMatrix, {0, -1, 0}), Global::WhiteColor,  CppTexture});
        Renderer2D.End();

        Renderer2D.SetBlendingMode(BlendingMode::Add);
        
        Renderer2D.Begin();
        Renderer2D.SubmitQuad({glm::translate(Global::IdentityMatrix, BallPosition), Global::WhiteColor,  CatTexture});
        Renderer2D.End();
    }
};

int main()
{
    DemoGame demoGame;
    demoGame.Create();
}