#pragma once

namespace Nit
{
    // Utility functions
    const TArray<CVector3, 4>& GetQuadVertexPositions();
    const TArray<CVector2, 4>& GetQuadVertexUVs();
    
    void FillQuadVertexPositions(const CVector2& textureSize, TArray<CVector3, 4>& vertexPositions);
    void FillQuadVertexUVs(const CVector2& uvMin, const CVector2& uvMax, TArray<CVector2, 4>& vertexUVs);
    void FlipQuadVertexUVs(bool bFlipX, bool bFlipY, TArray<CVector2, 4>& vertexUVs);
}