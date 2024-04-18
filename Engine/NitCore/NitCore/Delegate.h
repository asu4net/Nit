#pragma once
#include <functional>

namespace Nit
{
    template<typename T>
    class Delegate;
    
    template<typename TReturn, typename ...TArgs>
    class Delegate<TReturn(TArgs...)>
    {
    public:
        Delegate() = default;
        
        Delegate(const std::function<TReturn(TArgs...)>& func)
            : m_Function(func)
        {}
        
        template<typename TSubscriber>
        void Bind(TSubscriber* obj, TReturn(TSubscriber::*funcPtr)(TArgs...))
        {
            m_Function = std::bind(funcPtr, obj, std::placeholders::_1);
        }
        
        template<typename ...Args>
        TReturn operator()(Args&&... args)
        {
            if (!m_Function)
                return TReturn();
            return m_Function(std::forward<Args>(args)...);
        }
        
        bool operator==(const Delegate& other) const
        {
            return m_Function == other.m_Function;
        }
        
        void Clear() { m_Function = nullptr; }
    
    private:
        std::function<TReturn(TArgs...)> m_Function;
    };
}