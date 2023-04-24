#pragma once
#include <cassert>

namespace Nit
{
    template<typename T>
    class Singleton
    {
    public:
        static bool HasInstance() { return m_Instance; }
        
        static T& GetInstance()
        {
            assert(HasInstance() && "There is no instance to retrieve!");
            return *m_Instance;
        }
        
        static T& CreateSingleton()
        {
            assert(!m_Instance && "Instance already exists!");
            m_Instance = new T();
            return GetInstance();
        }

        static void DestroySingleton()
        {
            assert(m_Instance && "There is no instance to destroy!");
            delete m_Instance;
        }
        
    private:
        inline static T* m_Instance = nullptr;
    };
}