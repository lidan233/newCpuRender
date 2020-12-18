//
// Created by 李源 on 2019-11-22.
//

inline template<typename T> void swap(T& a,T& b)
{
    T temp = a ;
    a = b  ;
    b = temp ;
}