#include "RenderUtils.h"

namespace Nit::Render
{
    const Array<Vector3, 4>& GetQuadVertexPositions()
    {
        static const Array<Vector3, 4>& defaultQuadVertexPositions =
        {
            Vector3(-.5f, -.5f, 0),
            Vector3( .5f, -.5f, 0),
            Vector3( .5f,  .5f, 0),
            Vector3(-.5f,  .5f, 0)
        };

        return defaultQuadVertexPositions;
    }

    const Array<Vector2, 4>& GetQuadVertexUVs()
    {
        static const Array<Vector2, 4> defaultQuadVertexUVs =
        {
            Vector2(0, 0), // bottom-left
            Vector2(1, 0), // bottom-right
            Vector2(1, 1), // top-right
            Vector2(0, 1)  // top-left
        };

        return defaultQuadVertexUVs;
    }

    void FillQuadVertexPositions(const Vector2& textureSize, Array<Vector3, 4>& vertexPositions)
    {
        static Vector2 s_VertexMag = Vector2::One / 2;
        Vector2 pos = s_VertexMag;

        if (std::abs(textureSize.x - textureSize.y) > 0.0001f)
            pos = textureSize.Normalized() / 2;

        vertexPositions[0] = { -pos.x, -pos.y, 0 };
        vertexPositions[1] = {  pos.x, -pos.y, 0 };
        vertexPositions[2] = {  pos.x,  pos.y, 0 };
        vertexPositions[3] = { -pos.x,  pos.y, 0 };
    }

    void FillQuadVertexUVs(const Vector2& uvMin, const Vector2& uvMax, Array<Vector2, 4>& vertexUVs)
    {
        vertexUVs[0] = { uvMin.x, uvMin.y }; // bottom-left
        vertexUVs[1] = { uvMax.x, uvMin.y }; // bottom-right
        vertexUVs[2] = { uvMax.x, uvMax.y }; // top-right
        vertexUVs[3] = { uvMin.x, uvMax.y }; // top-left
    }

    void FlipQuadVertexUVs(bool bFlipX, bool bFlipY, Array<Vector2, 4>& vertexUVs)
    {
        if (!bFlipX && !bFlipY)
            return;

        Array<Vector2, 4> uv = vertexUVs;

        if (bFlipX && bFlipY)
        {
            vertexUVs[0] = uv[3];
            vertexUVs[1] = uv[2];
            vertexUVs[2] = uv[1];
            vertexUVs[3] = uv[0];
            return;
        }

        if (bFlipX)
        {
            vertexUVs[0] = uv[1];
            vertexUVs[1] = uv[0];
            vertexUVs[2] = uv[3];
            vertexUVs[3] = uv[2];
            return;
        }

        if (bFlipY)
        {
            vertexUVs[0] = uv[2];
            vertexUVs[1] = uv[3];
            vertexUVs[2] = uv[0];
            vertexUVs[3] = uv[1];
            return;
        }
    }
}