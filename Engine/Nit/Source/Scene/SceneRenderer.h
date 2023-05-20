#pragma once
#include "Rendering/Camera.h"
#include "Rendering/Renderer2D.h"

namespace Nit
{
    class SceneRenderer
    {
    public:
        void PushCamera(const Camera& camera);
        void PushQuad(const Quad& quad);
        void PushTextQuad(const TextQuad& textQuad);
        
        void DrawScene();

    private:
        std::vector<Camera> m_Cameras;
        std::vector<Quad> m_Quads;
        std::vector<TextQuad> m_TextQuads;
    };
}