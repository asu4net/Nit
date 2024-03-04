#include "Math.h"

namespace Nit::Math
{
    inline static constexpr float DegreesToRadiansFactor = 0.0174533f;
    inline static constexpr float RadiansToDegreesFactor = 57.2958f;

    float ToRadians(float degrees)
    {
        return degrees * DegreesToRadiansFactor;
    }

    Vector3 ToRadians(const Vector3& degrees)
    {
        return degrees * DegreesToRadiansFactor;
    }

    float ToDegrees(float radians)
    {
        return radians * RadiansToDegreesFactor;
    }

    Vector3 ToDegrees(const Vector3& radians)
    {
        return radians * RadiansToDegreesFactor;
    }

    Vector3 ScreenToWorldPoint(const Matrix4& ProjectionViewMatrix, const Vector2& screenPoint, const Vector2& windowSize)
    {
        const float halfScreenWidth = windowSize.x / 2.f;
        const float halfScreenHeight = windowSize.y / 2.f;
        const Matrix4 inverseMv = ProjectionViewMatrix.GetInverse();
        const float nearPlaneX = (screenPoint.x - halfScreenWidth) / halfScreenWidth;
        const float nearPlaneY = -((screenPoint.y - halfScreenHeight) / halfScreenHeight);
        const Vector4 nearPlane = { nearPlaneX, nearPlaneY, -1, 1 };
        Vector4 nearResult = inverseMv * nearPlane;
        nearResult /= nearResult.w;
        return { nearResult.x, nearResult.y, 0 };
    }

    const Array<Vector2, 4>& Math::GetDefaultQuadUVs()
    {
        static Array<Vector2, 4> uv =
        {
            Vector2(0, 0), // bottom-left
            Vector2(1, 0), // bottom-right
            Vector2(1, 1), // top-right
            Vector2(0, 1)  // top-left
        };
        return uv;
    }

    const Array<Vector3, 4>& Math::GetDefaultQuadVertexPositions()
    {
        static Array<Vector3, 4> vertexPositions =
        {
            Vector3(-.5f, -.5f, 0),
            Vector3( .5f, -.5f, 0),
            Vector3( .5f,  .5f, 0),
            Vector3(-.5f,  .5f, 0)
        };
        return vertexPositions;
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

    void FlipQuadVertexUVs(Flip flip, Array<Vector2, 4>& vertexUVs)
    {
        if (flip == Flip::None)
            return;

        Array<Vector2, 4> uv = vertexUVs;

        switch (flip)
        {
        case Flip::X:
            vertexUVs[0] = uv[1];
            vertexUVs[1] = uv[0];
            vertexUVs[2] = uv[3];
            vertexUVs[3] = uv[2];
            return;
        case Flip::Y:
            vertexUVs[0] = uv[2];
            vertexUVs[1] = uv[3];
            vertexUVs[2] = uv[0];
            vertexUVs[3] = uv[1];
            return;
        case Flip::Both:
            vertexUVs[0] = uv[3];
            vertexUVs[1] = uv[2];
            vertexUVs[2] = uv[1];
            vertexUVs[3] = uv[0];
        }
    }

    void FillQuadVertexUVs(const Vector2& uvMin, const Vector2& uvMax, Array<Vector2, 4>& vertexUVs)
    {
        vertexUVs[0] = { uvMin.x, uvMin.y }; // bottom-left
        vertexUVs[1] = { uvMax.x, uvMin.y }; // bottom-right
        vertexUVs[2] = { uvMax.x, uvMax.y }; // top-right
        vertexUVs[3] = { uvMin.x, uvMax.y }; // top-left
    }
}