#pragma once

namespace Nit::Math
{
    float ToRadians(float degrees);
    Vector3 ToRadians(const Vector3& degrees);
    float ToDegrees(float radians);
    Vector3 ToDegrees(const Vector3& radians);

    inline float FloatMax() { return std::numeric_limits<float>::max(); }
    Vector3 ScreenToWorldPoint(const Matrix4& ProjectionViewMatrix, const Vector2& screenPoint, const Vector2& windowSize);

    // Quad
    const Array<Vector2, 4>& GetDefaultQuadUVs();
    const Array<Vector3, 4>& GetDefaultQuadVertexPositions();
    void FillQuadVertexPositions(const Vector2& textureSize, Array<Vector3, 4>& vertexPositions);
    void FlipQuadVertexUVs(Flip flip, Array<Vector2, 4>& vertexUVs);
    void FillQuadVertexUVs(const Vector2& uvMin, const Vector2& uvMax, Array<Vector2, 4>& vertexUVs);
}