#include <thread>
#include <future>
#include <chrono>

#include "gtest/gtest.h"

class Thread_Test : public testing::Test{
    void SetUp() override{
    }

    void TearDown() override{
    }
};

TEST_F(Thread_Test, Basic){
    int a = 0;
    auto f1 = [&a](){
        a = 100;
    };
    std::thread thr1(f1);
    thr1.join();
    ASSERT_TRUE(a == 100);

    auto f2 = [&a](int i){
        a = i;
    };
    std::thread thr2(f2, 50);
    thr2.join();
    ASSERT_TRUE(a == 50);
}

thread_local int G = 0;
TEST_F(Thread_Test, Local){
    auto f1 = [](){
        G = 100;
    };
    std::thread thr1(f1);
    thr1.join();
    ASSERT_TRUE(G == 0);
}

TEST_F(Thread_Test, Class){
    struct C{
        void operator()(int n)
        {
            i = n;
        }

        int i = 0;
    };

    C c;
    std::thread thr1(std::ref(c), 2);
    thr1.join();
    ASSERT_TRUE(c.i == 2);
}

TEST_F(Thread_Test, Future){
    auto mul = [](int a, int b) -> int{
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return a * b;
    };
    std::future<int> fut1 = std::async(mul, 10, 5);
    fut1.wait();
    ASSERT_TRUE(fut1.valid());
    ASSERT_TRUE(fut1.get() == 50);
    ASSERT_TRUE(!fut1.valid());

    std::future<int> fut2 = std::async(mul, 5, 5);
    std::shared_future<int> share = fut2.share();
    share.wait();
    ASSERT_TRUE(!fut2.valid());
    ASSERT_TRUE(share.valid());
    ASSERT_TRUE(share.get() == 25);
    ASSERT_TRUE(share.valid());
}

TEST_F(Thread_Test, Promise){
    auto mul2 = [](std::future<int> &data) -> int{
        return data.get() * 2;
    };

    std::promise<int> prom;
    std::future<int> carrier = prom.get_future();
    std::future<int> result = std::async(mul2, std::ref(carrier));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    prom.set_value(10);
    ASSERT_TRUE(result.get() == 20);
}

TEST_F(Thread_Test, Launch){
    auto setvalue = [](std::string &data, std::size_t size, char c) {
      data = std::string(size, c);
    };

    std::string data1, data2;
    std::future<void> foo = std::async(std::launch::async, setvalue, std::ref(data1), 5, '*');
    std::future<void> bar = std::async(std::launch::async, setvalue, std::ref(data2), 5, '@');
    std::this_thread::sleep_for (std::chrono::milliseconds(100));
    ASSERT_TRUE(data1 == "*****");
    ASSERT_TRUE(data2 == "@@@@@");
    foo.get();
    bar.get();

    data1.clear();
    data2.clear();
    foo = std::async (std::launch::deferred, setvalue, std::ref(data1), 5, '*');
    bar = std::async (std::launch::deferred, setvalue, std::ref(data2), 5, '@');
    std::this_thread::sleep_for (std::chrono::milliseconds(100));
    ASSERT_TRUE(data1 == "");
    ASSERT_TRUE(data2 == "");

    bar.get();
    ASSERT_TRUE(data1 == "");
    ASSERT_TRUE(data2 == "@@@@@");

    std::this_thread::sleep_for (std::chrono::milliseconds(100));
    foo.get();
    ASSERT_TRUE(data1 == "*****");
    ASSERT_TRUE(data2 == "@@@@@");

    std::cout << '\n';
}

TEST_F(Thread_Test, HardwareConcurrency){
    auto concurrency = std::thread::hardware_concurrency();
    std::cout << "HardwareConcurrency:" << concurrency << std::endl;
    ASSERT_TRUE(concurrency > 0);
}
