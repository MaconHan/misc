#ifndef RIGHTVALUE_HPP_INCLUDED
#define RIGHTVALUE_HPP_INCLUDED


#include <iostream>
#include <string>
#include <tuple>
#include <typeinfo>
#include <stdlib.h>

using namespace std;

struct X {};

void inner(const X&) {
    cout << "inner(const X&)" << endl;
}

void inner(X&&) {
    cout << "inner(X&&)" << endl;
}

template<typename T>
void outer(T&& t) {
    inner(forward<T>(t));
}

struct __struct__{
    __struct__(std::string i = "") : _i(i)
    {

    }

    __struct__(__struct__ &&s)
    {

    }

    __struct__& operator=(const std::string &i)
    {
        this->_i = i;
        return *this;
    }

    __struct__& operator=(__struct__ &&i)
    {
        std::swap(this->_i, i._i);
        return *this;
    }

    std::string _i = "123";
};

bool is_r_value(__struct__ &&i) {
    i = "is right value";
    return true;
}

bool is_r_value(__struct__ &i) {
    i = "is not right value";
    return false;
}

bool is_r_value(int &&i) {
    i = 100;
    return true;
}

bool is_r_value(const int &i) {
    return false;
}


template<typename T>
struct remove_ref
{
    enum {__deep = 0};

    typedef T __type;
};

template<typename T>
struct remove_ref<T&>
{
    typedef typename remove_ref<T>::__type __type;

    enum {__deep = remove_ref<T>::__deep + 1};
};

template<typename T>
struct remove_ref<T&&>
{
    typedef typename remove_ref<T>::__type __type;

    enum {__deep = remove_ref<T>::__deep + 1};
};

void rightvalue()
{
    __struct__ s1, s2("main");
    s1 = std::move(s2);
    is_r_value(s1);
    is_r_value(std::move(s1));

    is_r_value(1);
    int argc;
    is_r_value(argc);
    is_r_value(move<int&>(argc));
    is_r_value(static_cast<int&>(argc));
    is_r_value(static_cast<int&&>(argc));

    typedef int     INT;
    typedef int&    INT_R;
    typedef int&&   INT_R_VALUE;
    is_r_value(static_cast<INT>(argc));
    is_r_value(static_cast<INT&&>(argc));
    is_r_value(static_cast<INT_R&>(argc));
    is_r_value(static_cast<INT_R&&>(argc));
    is_r_value(static_cast<INT_R_VALUE&>(argc));
    is_r_value(static_cast<INT_R_VALUE&&>(argc));

    remove_ref<INT_R_VALUE&&>::__type naked = 12313;
    remove_ref<INT_R_VALUE&&> ref;
    int naked_deep = remove_ref<INT_R_VALUE&&>::__deep;

    X a;
    outer(a);
    outer(X());
}


#endif
