#pragma once
#include <mutex>

template<class T>
class SingleTon
{
public:
    SingleTon() {}
    virtual ~SingleTon() {}
    SingleTon(SingleTon &other) = delete;
    SingleTon& operator= SingleTon(SingleTon &other) = delete;

public:
    static T *GetInstance()
    {
        if (nullptr == s_Self)
        {
            std::lock_guard<std::mutex> lock(s_Mutex);
            if (nullptr == s_Self)
            {
                s_Self = new T();
            }
        }
        return s_Self;
    }

    static void ReleaseInstance()
    {
        if (nullptr != s_Self)
        {
            std::lock_guard<std::mutex> lock(s_Mutex);
            if (nullptr != s_Self)
            {
                delete s_Self;
                s_Self = nullptr;
            }
        }
    }

private:
    static T *s_Self;
    static std::mutex s_Mutex;
};

template<class T>
T *SingleTon<T>::s_Self = nullptr;

template<class T>
std::mutex SingleTon<T>::s_Mutex;