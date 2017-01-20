#include "gtest/gtest.h"
#include <tuple>
#include <type_traits>
#include <string>

class Tuple_Test:public testing::Test{
    void SetUp() override{
    }

    void TearDown() override{
    }
};

TEST_F(Tuple_Test, make)
{
    auto i = 100U;
    auto j = "123";
    auto t = std::make_tuple(i, j);

    ASSERT_TRUE((std::tuple_size<decltype(t)>::value == 2U));
    ASSERT_TRUE((std::is_same<unsigned,      std::tuple_element<0, decltype(t)>::type>::value));
    ASSERT_TRUE((std::is_same<const char*,   std::tuple_element<1, decltype(t)>::type>::value));

    ASSERT_TRUE(std::get<0>(t) == 100U);
    ASSERT_TRUE(std::get<1>(t) == std::string(j));
}

TEST_F(Tuple_Test, tie)
{
    auto i = 100U;
    auto j = "123";
    auto t = std::make_tuple(i, j);

    decltype(i) i1;
    decltype(j) j1;
    std::tie(i1, std::ignore) = t;
    std::tie(std::ignore, j1) = t;
    ASSERT_TRUE(i1 == 100U);
    ASSERT_TRUE(j1 == std::string(j));
}

TEST_F(Tuple_Test, cat)
{
    auto i = 100U;
    auto j = "123";
    auto t1 = std::make_tuple(i, j);
    auto t2 = std::make_tuple(i, j);

    auto t = std::tuple_cat(t1, t2);

    ASSERT_TRUE((std::tuple_size<decltype(t)>::value == 4U));
    ASSERT_TRUE((std::is_same<unsigned,      std::tuple_element<0, decltype(t)>::type>::value));
    ASSERT_TRUE((std::is_same<const char*,   std::tuple_element<1, decltype(t)>::type>::value));
    ASSERT_TRUE((std::is_same<unsigned,      std::tuple_element<2, decltype(t)>::type>::value));
    ASSERT_TRUE((std::is_same<const char*,   std::tuple_element<3, decltype(t)>::type>::value));

    ASSERT_TRUE(std::get<0>(t) == 100U);
    ASSERT_TRUE(std::get<1>(t) == std::string(j));
    ASSERT_TRUE(std::get<2>(t) == 100U);
    ASSERT_TRUE(std::get<3>(t) == std::string(j));
}



