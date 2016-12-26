#include <type_traits>
#include <iostream>
#include <utility>
#include <thread>
#include <vector>
#include <functional>

#include "gtest/gtest.h"

struct TypeTraits_Test : testing::Test{
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(TypeTraits_Test, auto)
{
    int i;
    int *j;
    const int k = 1;
    const int &l = i;

    auto foo = []() -> int{return 1;};
    auto bar = [&i]() -> const int&{return i;};
    auto kite= [](std::string str) -> std::string {return str;};

    auto i1 = i;
    auto j1 = j;
    auto k1 = k;
    auto l1 = l;
    auto foo1 = foo();
    auto bar1 = bar();
    auto kite1= kite("kite");
    ASSERT_TRUE((std::is_same<int, decltype(i1)>::value));
    ASSERT_TRUE((std::is_same<int*, decltype(j1)>::value));
    ASSERT_TRUE((std::is_same<int, decltype(k1)>::value));
    ASSERT_TRUE((std::is_same<int, decltype(l1)>::value));
    ASSERT_TRUE((std::is_same<int, decltype(foo1)>::value));
    ASSERT_TRUE((std::is_same<int, decltype(bar1)>::value));
    ASSERT_TRUE((std::is_same<std::string, decltype(kite1)>::value));

    auto &i2 = i;
    auto &j2 = j;
    auto &k2 = k;
    auto &l2 = l;
    auto foo2 = foo();
    auto &bar2 = bar();
    //auto &kite2= kite("kite"); //编译失败,引用一个右值；
    ASSERT_TRUE((std::is_same<int&, decltype(i2)>::value));
    ASSERT_TRUE((std::is_same<int*&, decltype(j2)>::value));
    ASSERT_TRUE((std::is_same<const int&, decltype(k2)>::value));
    ASSERT_TRUE((std::is_same<const int&, decltype(l2)>::value));
    ASSERT_TRUE((std::is_same<int, decltype(foo2)>::value));
    ASSERT_TRUE((std::is_same<const int&, decltype(bar2)>::value));

    auto *i3 = &i;
    auto *j3 = j;
    const auto &l3 = l;
    const auto &kite3= kite("kite");
    ASSERT_TRUE((std::is_same<int*, decltype(i3)>::value));
    ASSERT_TRUE((std::is_same<int*, decltype(i3)>::value));
    ASSERT_TRUE((std::is_same<int*, decltype(j3)>::value));
    ASSERT_TRUE((std::is_same<const int&, decltype(l3)>::value));
    ASSERT_TRUE((std::is_same<const std::string&, decltype(kite3)>::value));
}


template<typename T1, typename T2>
auto Sum(T1 t1, T2 t2) -> decltype(t1 + t2)
{
    return t1 + t2;
}

TEST_F(TypeTraits_Test, decltype)
{
    /**
    decltype(e)规则说明：
    1、如果e是一个没有带括号的标记符表达式或者类成员访问表达式，那么decltype(e)就是e所命名的实体类型。
    此外，如果e是一个被重载的函数，则会导致编译时错误；
    2、否则，假设e的类型是T，如果e是一个亡值(xvalue)，那么decltype(e)为T&&；
    3、否则，假设e的类型是T，如果e是一个左值，则decltype(e)为T&；
    4、否则，假设e的类型是T，则decltype(e)为T；
    */
    int i;
    const int j = 1;
    const int& k = j;
    int arr[10];
    int *p;
    struct S{int a;} s1;
    const S s2 = {1};
    int f1(int);
    int&& f2(int);
    const int f3(int);

    ASSERT_TRUE((std::is_same<int&, decltype((i))>::value));
    ASSERT_TRUE((std::is_same<int, decltype(i)>::value));
    ASSERT_TRUE((std::is_same<const int, decltype(j)>::value));
    ASSERT_TRUE((std::is_same<const int&, decltype(k)>::value));
    ASSERT_TRUE((std::is_same<int, decltype(s1.a)>::value));
    ASSERT_TRUE((std::is_same<int, decltype(s2.a)>::value));
    ASSERT_TRUE((std::is_same<int[10], decltype(arr)>::value));
    ASSERT_TRUE((std::is_same<int*, decltype(p)>::value));
    ASSERT_TRUE((std::is_same<int (int), decltype(f1)>::value));
    ASSERT_TRUE((std::is_same<int&&, decltype(f2(1))>::value));
    ASSERT_TRUE((std::is_same<int, decltype(f3(1))>::value));

    ASSERT_TRUE((std::is_same<int&, decltype((i))>::value));
    ASSERT_TRUE((std::is_same<int&, decltype(++i)>::value));
    ASSERT_TRUE((std::is_same<int, decltype(i++)>::value));
    ASSERT_TRUE((std::is_same<int&, decltype(arr[0])>::value));
    ASSERT_TRUE((std::is_same<int&, decltype(*p)>::value));

    ASSERT_TRUE((std::is_same<int, decltype(Sum(i, j))>::value));
    ASSERT_TRUE((std::is_same<double, decltype(Sum(1.0, 1))>::value));

    std::vector<S> s_vec;
    ASSERT_TRUE((std::is_same<std::vector<S>::iterator, decltype(s_vec)::iterator>::value));
}

template<typename T1, typename T2>
decltype(std::declval<T1>() * std::declval<T2>()) Mul(T1 t1, T2 t2)
{
    return t1 * t2;
}

TEST_F(TypeTraits_Test, declval)
{
    struct A {
        int a;
        A() = delete;
        A(const A&) = delete;
        //A& operator= (const A&) = delete;

        const int value();
    };

    using T = decltype(std::declval<A>() = std::declval<A>());
    ASSERT_TRUE((std::is_same<A&, T>::value));

    ASSERT_TRUE((std::is_same<int, decltype(std::declval<A>().a)>::value));
    ASSERT_TRUE((std::is_same<int, decltype(std::declval<A>().value())>::value));

    ASSERT_TRUE((std::is_same<int, decltype(Mul(1, 1))>::value));
    ASSERT_TRUE((std::is_same<double, decltype(Mul(1.0, 2))>::value));
}

double fn(int, int){return 1.0;}
typedef int (*fn_ptr)(int);
typedef int (&fn_ref)(int);
struct fn_class{int operator()(int);};
TEST_F(TypeTraits_Test, result_of)
{
    std::function<decltype(fn)> f = fn;
    EXPECT_EQ(1.0, f(1, 1));

    decltype(fn) &f1 = fn;
    EXPECT_EQ(1.0, f1(1, 1));

    ASSERT_TRUE((std::is_same<double, std::result_of<decltype(fn)&(int, int)>::type>::value));
    ASSERT_TRUE((std::is_same<int, std::result_of<fn_ptr(int)>::type>::value));
    ASSERT_TRUE((std::is_same<int, std::result_of<fn_ref(int)>::type>::value));
    ASSERT_TRUE((std::is_same<int, std::result_of<fn_class(int)>::type>::value));
}
