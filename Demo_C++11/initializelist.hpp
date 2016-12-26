#ifndef INITIALIZELIST_HPP_INCLUDED
#define INITIALIZELIST_HPP_INCLUDED

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <initializer_list>
#include <functional>
#include <algorithm>

using namespace std;

class InitializerDemo
{
public:
    InitializerDemo(std::initializer_list<int> value)
    {
        for(auto &i : value){
            _value.push_back(i);
        }
    }

private:
    std::vector<int> _value;
};

void initializer()
{
    std::vector<std::string> _vec = {"1", "2", "3"};
    std::for_each(_vec.begin(), _vec.end(), [](std::string &x){std::cout << x << std::endl;});

    std::map<int, int> _map = {{1, 2}, {20, 3}, {3, 4}, {0, 4}};
    for(auto &x : _map){
        std::cout << x.first << " -> " << x.second << std::endl;
    }

    auto total = 0UL;
    auto closure = [&total](std::map<int, int>::value_type &x){total += x.second;};
    const std::type_info &info = typeid(closure);
    std::cout << info.name() << std::endl;
    std::for_each(_map.begin(), _map.end(), closure);
    std::cout << "the total value of this map:" << total << std::endl;

    auto _list = {1, 2, 3, 4, 5, 6, 7, 8};
    InitializerDemo demo = _list;
}

#endif // INITIALIZELIST_HPP_INCLUDED
