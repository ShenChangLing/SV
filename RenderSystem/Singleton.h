//
// Created by shenc on 2017/10/28.
//

#ifndef SV_SINGLETON_H
#define SV_SINGLETON_H

#include "Prerequisites.h"


template <typename T>
class Singleton
{
public:
    virtual ~Singleton()
    {
        assert(m_pSingleton);

        m_pSingleton = nullptr;
    }

    static T* getSingletonPrt()
    {
        assert(m_pSingleton);
        return m_pSingleton;
    }

    static T& getSingleton()
    {
        assert(m_pSingleton);
        return *m_pSingleton;
    }

protected:
    Singleton()
    {
        assert(!m_pSingleton);

        m_pSingleton = static_cast<T*>(this);
    }

    Singleton(const Singleton&) = delete;
    Singleton operator=(const Singleton&) = delete;

protected:
    static T* m_pSingleton;
};

#endif //SV_SINGLETON_H
