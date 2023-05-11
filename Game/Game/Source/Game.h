#pragma once
#include <Nit.h>

class ViewportCameraController;
class LogicCamera;

class Game : public Nit::EngineLayer
{
public:
    Nit::Shared<LogicCamera> Camera;
    
    Nit::AssetLink<Nit::Texture2D> GridTexture;
    Nit::AssetLink<Nit::Texture2D> CatTexture;
    Nit::AssetLink<Nit::Texture2D> CppTexture;
    Nit::AssetLink<Nit::AudioBuffer> LaserAudio;
    Nit::AssetLink<Nit::Font> TheFont;
    Nit::AudioSource LaserAudioSource;
    
    Nit::vec3 BallPosition = {.5f, .3f, 0.f};
    Nit::Quad Grid;
    Nit::TextQuad HelloWorldMessage;
    
    void OnStart() override;
    void OnUpdate(const Nit::TimeStep& timeStep) override;
    void OnFinish() override;
};
