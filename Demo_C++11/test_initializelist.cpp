#include <vector>
#include <map>
#include <string>
#include <initializer_list>

#include "gtest/gtest.h"

//
class Initializer_Test : public testing::Test{
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(Initializer_Test, Sample1)
{
    std::vector<int> _vec = {1, 2, 3, 4, 5};
    EXPECT_EQ(_vec.size(), 5);
    EXPECT_EQ(_vec[0], 1);
    EXPECT_EQ(_vec[4], 5);


    std::map<int, std::string> _map = {{1, "1"}, {2, "2"}, {3, "3"}};
    EXPECT_EQ(_map.size(), 3);
    EXPECT_EQ(_map[1], "1");
    EXPECT_EQ(_map[3], "3");
}

TEST_F(Initializer_Test, Sample2)
{
    struct Demo
    {
        Demo(std::initializer_list<int> value)
        {
            for(auto &i : value){
                _value.push_back(i);
            }
        }
        std::vector<int> _value;
    };

    Demo demo = {0, 1, 2};
    EXPECT_EQ(demo._value.size(), 3);
    EXPECT_EQ(demo._value[0], 0);
    EXPECT_EQ(demo._value[2], 2);
}

TEST_F(Initializer_Test, Sample3){
    struct IdString{
        std::string name;
        int identifier;
    };

    IdString var1{"SomeName", 4};
    EXPECT_EQ(var1.name, "SomeName");
    EXPECT_EQ(var1.identifier, 4);

    IdString var2 = []() -> IdString { return {"Func", 5}; }();
    EXPECT_EQ(var2.name, "Func");
    EXPECT_EQ(var2.identifier, 5);
}

TEST_F(Initializer_Test, Sample4){
    struct MyVector{
        int operator[](int i) const{
            return data[i];
        }

        MyVector& operator[](std::initializer_list<int> l) {
            for(auto i : l){
                index.push_back(i);
            }
            return *this;
        }

        MyVector& operator=(int v) {
            if (index.empty())
                return *this;

            for(auto i : index){
                if (i >= data.size())
                    data.resize(i + 1);
                data[i] = v;
            }
            index.clear();
            return *this;
        }

        std::vector<int> data;
        std::vector<int> index;
    };

    MyVector my;
    my[{1, 3, 5}] = 5;
    my[{0, 2, 4}] = 10;
    EXPECT_EQ(my[0], 10);
    EXPECT_EQ(my[5], 5);
}
