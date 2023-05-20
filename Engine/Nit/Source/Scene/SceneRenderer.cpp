#include "SceneRenderer.h"

namespace Nit
{
    void SceneRenderer::PushCamera(const Camera& camera)
    {
        m_Cameras.push_back(camera);
    }
    
    void SceneRenderer::PushQuad(const Quad& quad)
    {
        m_Quads.push_back(quad);
    }
    
    void SceneRenderer::PushTextQuad(const TextQuad& textQuad)
    {
        m_TextQuads.push_back(textQuad);
    }
    
    void SceneRenderer::DrawScene()
    {
        Renderer2D& renderer2D = Renderer2D::GetInstance();
        
        renderer2D.ClearScreen();
        
        //TODO: Add multiple camera support
        //TODO: Add magenta error draw call if missing cameras
        
        renderer2D.SetRenderData({
            m_Cameras[m_Cameras.size() - 1].GetProjectionViewMat4()
        });
        
        renderer2D.Begin();
        {
            for (const Quad& quad : m_Quads)
                renderer2D.SubmitQuad(quad);
            for (const TextQuad& textQuad : m_TextQuads)
                renderer2D.SubmitTextQuad(textQuad);
        }
        renderer2D.End();

        m_Cameras.clear();
        m_Quads.clear();
        m_TextQuads.clear();
    }
}