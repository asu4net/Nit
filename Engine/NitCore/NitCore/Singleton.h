#pragma once

#define NIT_SINGLETON_FRIEND(_CLASS_NAME) \
    private: \
        _CLASS_NAME(_CLASS_NAME&&) = delete; \
        friend class Nit::Singleton<_CLASS_NAME>;

namespace Nit
{
    template<typename T>
    class Singleton
    {
    public:
        static bool HasInstance() { return m_Instance; }

        static T& GetInstance()
        {
            NIT_CHECK(HasInstance(), "There is no instance to retrieve!");
            return *m_Instance;
        }

        template<typename... Args>
        static T& CreateInstance(Args&&... args)
        {
            NIT_CHECK(!m_Instance, "Instance already exists!");
            m_Instance = new T(std::forward<Args>(args)...);
            return GetInstance();
        }

        static void DestroyInstance()
        {
            NIT_CHECK(m_Instance, "There is no instance to destroy!");
            delete m_Instance;
        }

    private:
        inline static T* m_Instance = nullptr;
    };
}
