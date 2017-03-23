#pragma once
#include <mutex>

/*
* 继承此模板类
* 并在子类中添加此类的友元声明
* friend class SingleTon<T>;
*/

template<class T>
class SingleTon
{
protected:
    SingleTon() {}
    virtual ~SingleTon() {}
    SingleTon(SingleTon &other) = delete;
    SingleTon& operator= (SingleTon &other) = delete;

public:
    template<class ... Types>
    static T *GetInstance(Types ... args)
    {
        if (nullptr == s_Self)
        {
            std::lock_guard<std::mutex> lock(s_Mutex);
            if (nullptr == s_Self)
            {
                s_Self = new T(args...);
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
