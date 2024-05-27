#include "InputModifiers.h"
#include "Core\Engine.h"

namespace Nit
{

    InputModifierDeadZone::InputModifierDeadZone(float _LowerThreshold /*= 0.2*/, float _UpperThreshold /*= 1.f*/, EDeadZoneType _Type /*= EDeadZoneType::Radial*/)
        : m_LowerThreshold(_LowerThreshold)
        , m_UpperThreshold(_UpperThreshold)
        , m_Type(_Type)
    {}

    void InputModifierDeadZone::Modify(Vector4& inputValue, EInputType inputType) const
    {
        if(inputType == EInputType::Digital) return;

        auto DeadZoneLambda = [this](const float _axisVal)
        {
            return Math::Min(1.f, (Math::Max(0.f, Math::Abs(_axisVal) - m_LowerThreshold) / (m_UpperThreshold - m_LowerThreshold))) * Math::Sign(_axisVal);
        };

        switch(m_Type)
        {
            case EDeadZoneType::Axial:
                inputValue.x = DeadZoneLambda(inputValue.x);
                inputValue.y = DeadZoneLambda(inputValue.y);
                inputValue.z = DeadZoneLambda(inputValue.z);
                inputValue.w = DeadZoneLambda(inputValue.w);
                break;
            case EDeadZoneType::Radial:
                if (inputType == EInputType::Axis2D)
                {
                    Vector2 newValue = Vector2(inputValue.x, inputValue.y);
                    newValue.Normalize();
                    newValue = newValue * DeadZoneLambda(newValue.Magnitude());
                    inputValue.x = newValue.x;
                    inputValue.y = newValue.y;
                }
                else if (inputType == EInputType::Axis3D)
                {
                    Vector3 newValue = Vector3(inputValue.x, inputValue.y, inputValue.z);
                    newValue.Normalize();
                    newValue *= DeadZoneLambda(newValue.Magnitude());
                    inputValue.x = newValue.x;
                    inputValue.y = newValue.y;
                    inputValue.z = newValue.z;
                }
                else if (inputType == EInputType::Axis4D)
                {
                    inputValue = inputValue.Normalized() * DeadZoneLambda(inputValue.Magnitude());
                }
                else
                {
                    inputValue.x = DeadZoneLambda(inputValue.x);
                }
                break;
        }
    }

    InputModifierScalar::InputModifierScalar(float _ScalarX /*= 1.f*/, float _ScalarY /*= 1.f*/, float _ScalarZ /*= 1.f*/, float _ScalarW /*= 1.f*/)
        : m_Scalar(Vector4(_ScalarX, _ScalarY, _ScalarZ, _ScalarW))
    {}

    InputModifierScalar::InputModifierScalar(const Vector4& _Scalar /*= Vector4::One*/)
        : m_Scalar(_Scalar)
    {}

    void InputModifierScalar::Modify(Vector4& inputValue, EInputType inputType) const
    {
        if(inputType == EInputType::Digital) return;

        inputValue = inputValue * m_Scalar;
    }

    InputModifierNegate::InputModifierNegate(bool _NegateX /*= true*/, bool _NegateY /*= true*/, bool _NegateZ /*= true*/, bool _NegateW /*= true*/)
        : m_NegateX(_NegateX)
        , m_NegateY(_NegateY)
        , m_NegateZ(_NegateZ)
        , m_NegateW(_NegateW)
    {}

    void InputModifierNegate::Modify(Vector4& inputValue, EInputType inputType) const
    {
        if (inputType == EInputType::Digital) return;

        inputValue = inputValue * Vector4(m_NegateX ? -1.f : 1.f, m_NegateY ? -1.f : 1.f, m_NegateZ ? -1.f : 1.f, m_NegateW ? -1.f : 1.f );
    }

    InputModifierSwizzleAxis::InputModifierSwizzleAxis(ESwizzleAxisType _Order)
        : m_Order(_Order)
    {}

    void InputModifierSwizzleAxis::Modify(Vector4& inputValue, EInputType inputType) const
    {
        if (inputType == EInputType::Digital) return;

        Vector4 auxValue = inputValue;
        switch (m_Order)
        {
            case ESwizzleAxisType::XZY:
                inputValue.y = auxValue.z;
                inputValue.z = auxValue.y;

                break;
            case ESwizzleAxisType::YXZ:
                inputValue.x = auxValue.y;
                inputValue.y = auxValue.x;

                break;
            case ESwizzleAxisType::YZX:
                inputValue.x = auxValue.y;
                inputValue.y = auxValue.z;
                inputValue.z = auxValue.x;

                break;
            case ESwizzleAxisType::ZXY:
                inputValue.x = auxValue.z;
                inputValue.y = auxValue.x;
                inputValue.z = auxValue.y;
                
                break;
            case ESwizzleAxisType::ZYX:
                inputValue.x = auxValue.z;
                inputValue.z = auxValue.x;
                
                break;
        }
    }

}