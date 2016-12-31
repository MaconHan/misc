#include <functional>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <tuple>
#include <typeinfo>

#include "gtest/gtest.h"

class ExtensionCore_Test : public testing::Test{
    void SetUp() override{
    }

    void TearDown() override{
    }
};

constexpr int get_five() {
    return 5;
}
//泛化的常数表示式, 函数或是对象建构式是编译期常数
TEST_F(ExtensionCore_Test, ConstExpr1){
    int some_value[get_five()];

    EXPECT_EQ(sizeof(some_value), sizeof(int) * get_five());
}

TEST_F(ExtensionCore_Test, ConstExpr2)
{
    constexpr int SIZE = 5;//与const的效果相同
    int some_value[SIZE];

    EXPECT_EQ(sizeof(some_value), sizeof(int) * SIZE);
}


TEST_F(ExtensionCore_Test, TypeInference1){
    auto i = 1;
    EXPECT_EQ(typeid(i).name(), std::string("i"));

    auto f1 = [&i]() -> int& {return i;};
    auto f2 = [&i]() -> const int& {return i;};
    auto i1 = f1();
    auto i2 = f2();
    auto &i3= f1();

    ASSERT_TRUE((std::is_same<int, decltype(i1)>::value));
    ASSERT_TRUE((std::is_same<int, decltype(i2)>::value));
    ASSERT_TRUE((std::is_same<int&, decltype(i3)>::value));

    auto mul = [](int a, int b) -> int {return a * b;};
    EXPECT_EQ(mul(2, 5), 10);

    auto sub = [](int a, int b) ->int {return a - b;};
    auto sub1 = std::bind(sub, std::placeholders::_2, std::placeholders::_1);
    EXPECT_EQ(sub1(90, 100), 10);

    auto sub2 = std::bind(sub, 100, std::placeholders::_1);
    EXPECT_EQ(sub2(90), 10);
}

//decltype 能够被用来在编译期决定一个表示式的类型
//类似与gcc的typeof操作符
TEST_F(ExtensionCore_Test, TypeInference2){
    int i = 1;
    decltype(i) j;
    ASSERT_TRUE(typeid(i) == typeid(j));

    decltype(1 + double(10.0)) d;
    ASSERT_TRUE(typeid(d) == typeid(double));

    auto f = []() -> std::string{ return std::string(); };
    decltype(f()) s;
    ASSERT_TRUE(typeid(s) == typeid(std::string));

    std::vector<std::string> v;
    ASSERT_TRUE(typeid(decltype(v)::iterator) == typeid(v.begin()));

    ASSERT_TRUE(!std::is_lvalue_reference<decltype(i)>::value);
    ASSERT_TRUE(std::is_lvalue_reference<decltype((i))>::value);
    ASSERT_TRUE(std::is_rvalue_reference<decltype(std::move(i))>::value);
    ASSERT_TRUE(!std::is_rvalue_reference<decltype(0)>::value);
    ASSERT_TRUE(!std::is_lvalue_reference<decltype((f()))>::value);
    ASSERT_TRUE(!std::is_lvalue_reference<decltype(f())>::value);
}

TEST_F(ExtensionCore_Test, For1){
    int my_array1[5] = {1, 2, 3, 4, 5};
    for (int &x : my_array1){
        x *= 2;
    }
    EXPECT_EQ(my_array1[0], 2);
    EXPECT_EQ(my_array1[4], 10);

    std::vector<int> my_array2 = {1, 2, 3, 4, 5};
    for (auto &x : my_array2){
        x *= 2;
    }
    EXPECT_EQ(my_array2[0], 2);
    EXPECT_EQ(my_array2[4], 10);

    {
        std::map<std::string, std::string> str_map;
        str_map["abc"] = "123";
        str_map["efg"] = "456";
        for(std::map<std::string, std::string>::const_iterator it = str_map.begin(); it != str_map.end(); ++it){
            std::cout << "key=" << it->first << "; value=" << it->second << std::endl;
        }
    }

    std::map<std::string, std::string> str_map = {{"abc", "123"}, {"efg", "456"}};
    for(auto it : str_map){
        std::cout << "key=" << it.first << "; value=" << it.second << std::endl;
    }
}

TEST_F(ExtensionCore_Test, For2){

}

template<typename T1, typename T2>
auto Add(const T1 &a, const T2 &b) -> decltype(a + b){
    return a + b;
}
TEST_F(ExtensionCore_Test, AlternativeFunction){
    EXPECT_EQ(Add(10, 20), 30);
    EXPECT_EQ(Add(std::string("10"), "20"), "1020");
}

TEST_F(ExtensionCore_Test, NullPtr){
    struct Func{
        int operator()(char*){ return 1; };
        int operator()(int){ return 2; };
    };

    //nullptr 的类型为nullptr_t，能隐式转换为任何指针或是成员指针的类型，也能和它们进行相等或不等的比较。
    //而nullptr不能隐式转换为整数，也不能和整数做比较。
    EXPECT_EQ(Func()(nullptr), 1);
    EXPECT_EQ(Func()(0), 2);

    char *p = nullptr;
    EXPECT_EQ(*(decltype(sizeof(0))*)&p, 0);

    decltype(nullptr) n = NULL;
    EXPECT_EQ(n, nullptr);

    //编译失败
    //int i = nullptr;
}

TEST_F(ExtensionCore_Test, StronglyEnumerations){
    enum E1 {I = 0, J};

    E1 e1 = I;
    EXPECT_EQ(e1, 0);

    enum class E2 {I = 0, J};
    E2 e2 = E2::I;
    //e2 = I;   //编译错误
    //e2 == I;  //编译错误
    EXPECT_EQ(e2, E2::I);

    //允许为传统的枚举指定使用类型
    enum class E3 : unsigned long {I = 0, J};
}

TEST_F(ExtensionCore_Test, RightAngleBracket){
    std::vector<std::vector<std::string>> vec = {{"a", "b", "c"}, {"1", "2", "3", "4"}};

    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0].size(), 3);
    EXPECT_EQ(vec[1].size(), 4);
}

TEST_F(ExtensionCore_Test, ExplicitConversionOperators){
    struct Implicit{
        operator std::string () const{
            return "implicit";
        }
    };

    struct Explicit{
        explicit operator std::string () const{
            return "explicit";
        }
    };

    std::string r1 = Implicit();
    std::string r2 = static_cast<std::string>(Explicit());

    EXPECT_EQ(r1, "implicit");
    EXPECT_EQ(r2, "explicit");
}

template<typename F, typename S, typename T>
struct TriAdd
{
    auto operator()(const F &a, const S &b, const T &c) -> decltype(a + b + c){
        return a + b + c;
    }
};
template<typename T>
using Str_TriAdd = TriAdd<T, T, const char*>;

TEST_F(ExtensionCore_Test, Using){
    ASSERT_TRUE((TriAdd<int, int, int>()(1, 2, 3)) == 6);
    ASSERT_TRUE((Str_TriAdd<std::string>()("a", "b", "123")) == "ab123");

    typedef void (*F1)(char);
    using F2 = void (*)(char);

    F1 f1 = [](char){};
    F2 f2 = f1;
    ASSERT_TRUE(f1 == f2);
}

TEST_F(ExtensionCore_Test, Union){
    struct point{
      point(int x, int y): x_(x), y_(y) {}
      int x_, y_;
    };
    union FIT
    {
        FIT(){};

        int z;
        double w;
        point p;  // 合法的 C++11
    };

    ASSERT_TRUE(!std::is_pod<point>::value);
    ASSERT_TRUE(!std::is_trivial<point>::value);

    FIT u;
    u.p = point(1, 2);
    ASSERT_TRUE(u.z == 1);
}

template<typename ...Args>
struct Variadic{
    enum{
        size    = sizeof...(Args),
    };
};

TEST_F(ExtensionCore_Test,  VariadicTemplate1){
    ASSERT_TRUE(Variadic<>::size == 0);
    ASSERT_TRUE((Variadic<char, int>::size == 2));
}

template<typename ...C>
struct CombineType
{
    using T = std::tuple<C...>;
};

template<typename ...C>
struct ExpandFrontType{
};
template<typename B, typename ...C>
struct ExpandFrontType<B, C...>
{
    using T = B;
    using N = ExpandFrontType<C...>;
};
template<typename C>
struct ExpandFrontType<C>
{
    using T = C;
    using N = decltype(nullptr);
};

template<std::size_t S, typename ...C>
struct ExpandBackTypeWrapper
{
    using T = typename std::tuple_element<S, std::tuple<C...>>::type;
    using N = ExpandBackTypeWrapper<S - 1, C...>;

    T i;
    N j;

    void hello()
    {
        j.hello();
        std::cout << typeid(i).name() << std::endl;
        std::cout << typeid(j).name() << std::endl;
    }
};
template<typename ...C>
struct ExpandBackTypeWrapper<0, C...>
{
    using T = typename std::tuple_element<0, std::tuple<C...>>::type;
    using N = decltype(nullptr);

    T i;
    N j;

    void hello()
    {
        std::cout << typeid(T).name() << std::endl;
        std::cout << typeid(N).name() << std::endl;
    }
};

template<typename ...C>
struct ExpandBackType
{
    enum {S = sizeof ...(C), I = S - 1};
    using T = typename std::tuple_element<I, std::tuple<C...>>::type;
    using N = ExpandBackTypeWrapper<I - 1, C...>;

    T i;
    N j;

    void hello()
    {
        j.hello();
        std::cout << typeid(i).name() << std::endl;
        std::cout << typeid(j).name() << std::endl;
    }
};

template<typename T>
std::string SplitType(T&)
{
    return typeid(T).name();
}
template<typename ...T>
std::vector<std::string> SplitTypeWrapper(T... a)
{
    return {SplitType(a)...};
};


template<typename ...C>
struct MutiInheritance: public C...
{
    MutiInheritance(C&&... base) : C(base)... {
    }
};

TEST_F(ExtensionCore_Test,  VariadicTemplate2){
    using T1 = CombineType<int, float, std::string>;
    ASSERT_TRUE((std::is_same<T1::T, std::tuple<int, float, std::string>>::value));

    using T2 = ExpandFrontType<int, float, std::string>;
    ASSERT_TRUE((std::is_same<T2::T, int>::value));
    ASSERT_TRUE((std::is_same<T2::N::T, float>::value));
    ASSERT_TRUE((std::is_same<T2::N::N::T, std::string>::value));
    ASSERT_TRUE((std::is_same<T2::N::N::N, decltype(nullptr)>::value));

    using T3 = ExpandBackType<int, float, std::string>;
    ASSERT_TRUE((std::is_same<T3::T, std::string>::value));
    ASSERT_TRUE((std::is_same<T3::N::T, float>::value));
    ASSERT_TRUE((std::is_same<T3::N::N::T, int>::value));
    ASSERT_TRUE((std::is_same<T3::N::N::N, decltype(nullptr)>::value));

    std::vector<std::string> types = {typeid(int).name(), typeid(float).name(), typeid(std::string).name()};
    ASSERT_TRUE(types == SplitTypeWrapper(int(1), float(1.2), std::string("abc")));

    MutiInheritance<std::string, std::pair<int, int>> c(std::string("abc"), {1, 2});
    ASSERT_TRUE(c.c_str() == std::string("abc"));
    ASSERT_TRUE(c.first == 1);
    ASSERT_TRUE(c.second == 2);
}

template<typename T, std::size_t N>
struct TypeGetter{
    using type = typename TypeGetter<typename T::N, N - 1>::type;
};
template<typename T>
struct TypeGetter<T, 0>{
    using type = typename T::T;
};

template<std::size_t N>
struct Getter
{
    template<typename T>
    inline static typename TypeGetter<T, N>::type& get(T& val)
    {
        return Getter<N-1>::template get<typename T::N>(val.n);
    }
};
template<>
struct Getter<0>
{
    template<typename T>
    inline static typename TypeGetter<T, 0>::type& get(T& val)
    {
        return val.c;
    }
};

template<typename ...Args>
struct MyTuple{
};

template<>
struct MyTuple<>{
};

template<typename C, typename ...Args>
struct MyTuple<C, Args...>{
    using M = MyTuple<C, Args...>;
    using T = C;
    using N = MyTuple<Args...>;
    T c;
    N n;

    template<std::size_t N>
    inline typename TypeGetter<M, N>::type& get()
    {
        return Getter<N>::template get(*this);
    }

    template<std::size_t N>
    inline typename TypeGetter<M, N>::type& set(const typename TypeGetter<M, N>::type &val)
    {
        Getter<N>::template get(*this) = val;
    }
};
TEST_F(ExtensionCore_Test,  VariadicTemplate3){
    using Tuple = MyTuple<int, long, std::string>;

    EXPECT_EQ(typeid(TypeGetter<Tuple, 0>::type), typeid(int));
    EXPECT_EQ(typeid(TypeGetter<Tuple, 1>::type), typeid(long));
    EXPECT_EQ(typeid(TypeGetter<Tuple, 2>::type), typeid(std::string));

    Tuple t;
    t.set<2>("abc");
    ASSERT_TRUE(t.get<2>() == std::string("abc"));
}
