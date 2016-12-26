#include <chrono>
#include <thread>
#include <ratio>

#include <sys/time.h>

#include "gtest/gtest.h"

class Time_Test : public testing::Test{
    void SetUp() override{
    }

    void TearDown() override{
    }
};

TEST_F(Time_Test, Duration){
    std::chrono::milliseconds ms(1000);
    ms *= 60;

    EXPECT_EQ(60 * 1000, ms.count());

    EXPECT_EQ(1, std::chrono::milliseconds::period::num);
    EXPECT_EQ(1000, std::chrono::milliseconds::period::den);

    std::chrono::seconds ss = std::chrono::duration_cast<std::chrono::seconds>(ms);
    EXPECT_EQ(60, ss.count());
}

TEST_F(Time_Test, SystemTime){
    using namespace std::chrono;

    //调用chrono，获取系统时间
    system_clock::time_point now = system_clock::now();

    //调用系统提供的api，获取时间
    timeval tv;
    gettimeofday(&tv, NULL);
    auto system_us = tv.tv_sec * 1000000ULL + tv.tv_usec;

    system_clock::duration dtn = now.time_since_epoch();
    auto chrono_us = duration_cast<microseconds>(dtn).count();
    std::cout << "current microseconds:" << chrono_us << std::endl;
    std::cout << "current seconds:" << time_point_cast<seconds>(now).time_since_epoch().count()  << std::endl;

    //判断调用两种方式，获取时间是否相同
    EXPECT_EQ(system_us, chrono_us);
}

TEST_F(Time_Test, SteadyTime){
    auto ms = 123;
    using namespace std::chrono;

    //稳定的时间间隔，后一次调用得到的时间总是比前一次的值大。修改系统时间并不影响它的值
    steady_clock::time_point t1 = steady_clock::now();
    std::this_thread::sleep_for(milliseconds(ms));
    steady_clock::time_point t2 = steady_clock::now();

    std::cout << "current microseconds:" << time_point_cast<microseconds>(t1).time_since_epoch().count() << std::endl;
    std::cout << "elapse milliseconds:" << duration_cast<milliseconds>(t2 - t1).count() << std::endl;
    std::cout << "elapse nanseconds:" << duration_cast<nanoseconds>(t2 - t1).count() << std::endl;

    EXPECT_EQ(duration_cast<milliseconds>(t2 - t1).count(), ms);
}

TEST_F(Time_Test, GetDays)
{
    using namespace std::chrono;

    typedef duration<int, std::ratio<3600 * 24>> days_type;
    auto today = system_clock::now();
    std::chrono::time_point<system_clock, days_type> days = time_point_cast<days_type>(today);
    std::cout << days.time_since_epoch().count();     //打印自1970年到today的天数
}

TEST_F(Time_Test, UserDefinedTime)
{

}
