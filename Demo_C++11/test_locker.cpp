#include <atomic>
#include <thread>
#include <mutex>
#include <functional>
#include <algorithm>

#include "gtest/gtest.h"

class Locker_Test : public testing::Test{
    void SetUp() override{
    }

    void TearDown() override{
    }
};

TEST_F(Locker_Test, Atomic){
    auto f1 = [](int &i, int v){
        i = v;
    };
    auto f2 = [](int &i){
        while (i)
            break;
    };

    int i = 0;
    std::thread thr2(f2, std::ref(i));
    std::thread thr1(f1, std::ref(i), 10);

    thr2.join();
    thr1.join();
}

TEST_F(Locker_Test, Mutex){
    std::mutex mtx;
    std::vector<int> int_vec;

    auto f1 = [&](int n, int value){
        mtx.lock();
        for(auto i = 0; i != n; ++i){
            int_vec.push_back(value);
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        mtx.unlock();
    };

    std::thread thr1(f1, 10, 'a');
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    std::thread thr2(f1, 10, 'b');
    thr1.join();
    thr2.join();

    std::string str1, str2;
    EXPECT_EQ(int_vec.size(), 20);
    std::for_each(int_vec.begin(), int_vec.begin() + 10, [&str1](char v){str1 += v;});
    std::for_each(int_vec.begin() + 10, int_vec.end(), [&str2](char v){str2 += v;});

    EXPECT_EQ(str1, std::string(10, 'a'));
    EXPECT_EQ(str2, std::string(10, 'b'));
}

TEST_F(Locker_Test, Once){
    std::once_flag one_flag;
    int times = 0;
    auto run_onetime = [&times](){
        ++times;
    };
    auto f = [&one_flag, &run_onetime](){
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        std::call_once(one_flag, run_onetime);
    };

    std::thread thr_vec[10];
    for(auto i = 0; i != 10; ++i)
        thr_vec[i] = std::thread(f);

    for(auto &thr : thr_vec)
        thr.join();

    EXPECT_EQ(times, 1);
}

