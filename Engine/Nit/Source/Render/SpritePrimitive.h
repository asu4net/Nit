//#pragma once
//
//namespace Nit
//{
//    enum class FragmentShape : uint32_t
//    {
//        Defaut   = 0,
//        Circle = 1,
//        Rect   = 2
//    };
//    
//    class Sprite;
//
//    enum class SpriteShape
//    {
//        Defaut,
//        Circle,
//        Rect,
//        Line
//    };
//    
//    class SpritePrimitive
//    {
//    public:
//        SpritePrimitive();
//        ~SpritePrimitive();
//
//        FragmentShape            GetFragmentShape()        const { return m_FragmentShape; }
//        const Array<Vector3, 4>& GetLocalVertexPositions() const { return m_LocalVertexPositions; }
//        const Array<Vector3, 4>& GetVertexPositions()      const { return m_VertexPositions; }
//        const Array<Vector2, 4>& GetVertexUV()             const { return m_VertexUVs; }
//        const Array<Color,   4>& GetVertexColors()         const { return m_VertexColors; }
//        
//        void GenerateVertexData(SpriteShape shape = SpriteShape::Defaut);
//
//        // *** Used to generate vertex data ***
//        
//        // General
//        Matrix4           TransformMatrix;
//        Vector2           Size = Vector2::One;
//        Color             TintColor = Color::White;
//        Flip              FlipMode = Flip::None;
//        Vector2           UVScale = Vector2::One;
//
//        // Circle
//        float             Radius = .5f;
//
//        // Line
//        Vector2           StartPosition = Vector2::Zero;
//        Vector2           EndPosition = Vector2::Right;
//        Vector2           NormalVector = Vector2::Up;
//
//        // ************************************
//
//        // Vertex data
//        int               EntityID = -1;
//        SharedPtr<Sprite> SpriteRef = nullptr;
//        SpriteSource      Source = SpriteSource::Default;
//        String            SubSpriteName = "";
//        float             Thickness = .05f;
//        float             Fade = .01f;
//        Vector2           Bounds = Vector2::Zero;
//        Color             RectColor = Color::Orange;
//
//    private:
//        void GenerateSpriteVertexData();
//        void GenerateCircleVertexData();
//        void GenerateLineVertexData();
//        void GenerateRectVertexData();
//
//        void UpdateLocalVertexPositions();
//        void UpdateVertexPositions();
//        void UpdateVertexUVs();
//        void UpdateVertexColors();
//
//        // Vertex data
//        FragmentShape     m_FragmentShape = FragmentShape::Defaut;
//        Array<Vector3, 4> m_LocalVertexPositions;
//        Array<Vector3, 4> m_VertexPositions;
//        Array<Vector2, 4> m_VertexUVs;
//        Array<Color,   4> m_VertexColors;
//    };
//}