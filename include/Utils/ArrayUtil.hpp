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

    template<typename T, class Predicate>
    inline T* First(std::vector<T*> vec, Predicate condition)
    {
        for (auto v : vec) if (condition(v)) return v;
        return nullptr;
    }

    template<typename T, class Predicate>
    inline int FirstIndexOf(Array<T*>* arr, Predicate condition)
    {
        if (!arr) return -1;
        int length = arr->Length();
        if (!length) return -1;
        for (int i = 0; i < length; i++) 
        {
            if (condition(arr->values[i])) return i;
        }
        return -1;
    }

    template<typename T, class Predicate>
    inline int FirstIndexOf(List<T*>* list, Predicate condition)
    {
        if (!list) return -1;
        int length = list->get_Count();
        if (!length) return -1;
        for (int i = 0; i < length; i++) 
        {
            if (condition(list->items->values[i])) return i;
        }
        return -1;
    }

    template<typename T, class ToCall>
    inline void ForEach(Array<T*>* arr, ToCall lambda)
    {
        if (!arr) return;
        int length = arr->Length();
        if (!length) return;
        for (int i = 0; i < length; i++) lambda(arr->values[i]);
    }

    template<typename T, class ToCall>
    inline void ForEach(List<T*>* list, ToCall lambda)
    {
        if (!list) return;
        int length = list->get_Count();
        if (!length) return;
        for (int i = 0; i < length; i++) lambda(list->items->values[i]);
    }
}