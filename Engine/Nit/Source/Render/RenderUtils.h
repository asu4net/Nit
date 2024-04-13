#pragma once

namespace Nit::Render
{
    // Utility
    const Array<Vector3, 4>& GetQuadVertexPositions();
    const Array<Vector2, 4>& GetQuadVertexUVs();

    void FillQuadVertexPositions(const Vector2& textureSize, Array<Vector3, 4>& vertexPositions);
    void FillQuadVertexUVs(const Vector2& uvMin, const Vector2& uvMax, Array<Vector2, 4>& vertexUVs);
    void FlipQuadVertexUVs(bool bFlipX, bool bFlipY, Array<Vector2, 4>& vertexUVs);
}