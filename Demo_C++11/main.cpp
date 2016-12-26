#include "gtest/gtest.h"

#include <cstring>
#include <string>
#include <stdio.h>

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    auto ret = RUN_ALL_TESTS();
    system("pause");
    return ret;
}
