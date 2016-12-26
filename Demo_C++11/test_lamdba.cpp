#include <utility>
#include <functional>
#include <exception>
#include <stdexcept>
#include <vector>
#include <algorithm>

#include "gtest/gtest.h"

class Lambda_Test : public testing::Test {
    void SetUp() override{
    }

    void TearDown() override{
    }
};

TEST_F(Lambda_Test, Sample1){
    auto f = [](int a) -> int {
        return a;
    };

    std::cout << typeid(f).name() << std::endl;
    EXPECT_EQ(std::is_function<decltype(f)>::value, false);
    EXPECT_EQ(f(1), 1);
}

TEST_F(Lambda_Test, Sample2){
    int a = 1, b = 2, c = 3;
    auto f1 = [=](int v) -> int {
        return a + v;  //a只读
    };
    EXPECT_EQ(f1(100), 101);

    auto f2 = [&a](int v) {
        a = v;
    };
    f2(100);
    EXPECT_EQ(a, 100);

    auto f3 = [=, &a](int v) {
        a = v + b + c;  //a引用。引入的b和c是个常量，不会随外边的值变化而变化；
    };
    f3(100);
    EXPECT_EQ(a, 105);

    b = 100;
    f3(100);
    EXPECT_EQ(a, 105);
}

TEST_F(Lambda_Test, Sample3){
    struct S {
        void f(int i){
            [&, i](){};
            [&, &i](){};    //i定义的引用重复
            [=, this](){};  //在=号中，this是默认包含
            [i, i](){};     //i重复
        }
    };
}

TEST_F(Lambda_Test, Sample4){
    auto throw_except = []() noexcept(false) {
        throw std::logic_error("throw_except");
    };

    try{
        throw_except();
    }
    catch(std::exception &e){
        EXPECT_EQ(e.what(), std::string("throw_except"));
    }
}

TEST_F(Lambda_Test, Sample5){
    std::vector<int> vec = {5, 1, 0, 4, 2, 3};
    std::sort(vec.begin(), vec.end(), [](int &a, int &b) -> bool { return a < b; });
    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec[5], 5);

    auto total = 0;
    auto sum = [&total](int &x){total += x;};
    std::for_each(vec.begin(), vec.end(), sum);
    EXPECT_EQ(total, 15);
}
