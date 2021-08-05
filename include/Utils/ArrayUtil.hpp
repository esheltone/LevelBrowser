#pragma once

#include "beatsaber-hook/shared/utils/typedefs.h"
#include <functional>

namespace ArrayUtil
{
    template<typename T>
    inline T* Last(Array<T*>* arr)
    {
        if (!arr || !arr->Length()) return nullptr;
        return arr->values[arr->Length() - 1];
    }

    template<typename T, class Predicate>
    inline T* Last(Array<T*>* arr, Predicate condition)
    {
        if (!arr) return nullptr;
        int length = arr->Length();
        if (!length) return nullptr;
        for (int i = length - 1; i >= 0; i--) 
        {
            T* value = arr->values[i];
            if (condition(value)) return value;
        }
        return nullptr;
    }

    template<typename T>
    inline T* First(Array<T*>* arr)
    {
        if (!arr || !arr->Length()) return nullptr;
        return arr->values[0];
    }

    template<typename T, class Predicate>
    inline T* First(Array<T*>* arr, Predicate condition)
    {
        if (!arr) return nullptr;
        int length = arr->Length();
        if (!length) return nullptr;
        for (int i = 0; i < length; i++) 
        {
            T* value = arr->values[i];
            if (condition(value)) return value;
        }
        return nullptr;
    }
}