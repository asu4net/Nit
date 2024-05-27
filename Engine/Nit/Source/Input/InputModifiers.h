#pragma once

namespace Nit
{
    class InputModifier
    {
    public:
        virtual void Modify(Vector4& inputValue, EInputType inputType) const = 0;
    };

    enum class EDeadZoneType
    {
        Axial,
        Radial,
    };

    class InputModifierDeadZone : public InputModifier
    {
    public:
        float m_LowerThreshold = 0.2f;
        float m_UpperThreshold = 1.f;
        EDeadZoneType m_Type = EDeadZoneType::Radial;

        InputModifierDeadZone(float _LowerThreshold = 0.2, float _UpperThreshold = 1.f, EDeadZoneType _Type = EDeadZoneType::Radial);
        virtual void Modify(Vector4& inputValue, EInputType inputType) const override;
    };

    class InputModifierScalar : public InputModifier
    {
    public:
        Vector4 m_Scalar = Vector4::One;

        InputModifierScalar(float _ScalarX = 1.f, float _ScalarY = 1.f, float _ScalarZ = 1.f, float _ScalarW = 1.f);
        InputModifierScalar(const Vector4& _Scalar = Vector4::One);
        virtual void Modify(Vector4& inputValue, EInputType inputType) const override;
    };

    class InputModifierNegate : public InputModifier
    {
    public:
        bool m_NegateX = true;
        bool m_NegateY = true;
        bool m_NegateZ = true;
        bool m_NegateW = true;

        InputModifierNegate(bool _NegateX = true, bool _NegateY = true, bool _NegateZ = true, bool _NegateW = true);
        virtual void Modify(Vector4& inputValue, EInputType inputType) const override;
    };

    enum class ESwizzleAxisType
    {
        YXZ,
        ZYX,
        XZY,
        YZX,
        ZXY,
        // not doing it for w also thats 23 combinations lmfao
    };
    
    class InputModifierSwizzleAxis: public InputModifier
    {
    public:
        ESwizzleAxisType m_Order = ESwizzleAxisType::YXZ;

        InputModifierSwizzleAxis(ESwizzleAxisType _Order);
        virtual void Modify(Vector4& inputValue, EInputType inputType) const override;
    };
}