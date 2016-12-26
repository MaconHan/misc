#include <regex>
#include <vector>
#include <iterator>

#include <gtest/gtest.h>

struct Regex_Test : public testing::Test{
    void SetUp() override{
    }

    void TearDown() override{
    }
};

TEST_F(Regex_Test, regex_search)
{
    std::smatch result;
    const std::regex pattern("[\\d]+");

    std::string value = "123 abc 456 abc 48e";
    std::vector<std::string> val_vec;
    while(std::regex_search(value, result, pattern)){
        val_vec.push_back(result[0].str());
        value = result.suffix().str();
    }

    EXPECT_EQ(val_vec, std::vector<std::string>({"123", "456", "48"}));
}

TEST_F(Regex_Test, regex_search_group)
{
    std::smatch result;
    std::regex pattern("([a-zA-Z]+)://([^/]+)/([^\\s]+)");

    std::string value = "123 http://www.sina.com/news/1.jpg";
    std::regex_search(value, result, pattern);
    EXPECT_EQ(4U, result.size());

    EXPECT_EQ("http", result[1].str());
    EXPECT_EQ("www.sina.com", result[2].str());
    EXPECT_EQ("news/1.jpg", result[3].str());
}

TEST_F(Regex_Test, regex_match)
{
    std::smatch result;
    std::regex pattern("([a-zA-Z]+)://([^/]+)/([^\\s]+)");

    //完整匹配整个字符串
    std::string value1 = "123 http://www.sina.com/news/1.jpg";
    ASSERT_FALSE(std::regex_match(value1, pattern));

    std::string value2 = "http://www.sina.com/news/1.jpg";
    ASSERT_TRUE(std::regex_match(value2, result, pattern));
    EXPECT_EQ(4U, result.size());

    EXPECT_EQ(value2, result[0].str());
    EXPECT_EQ("http", result[1].str());
    EXPECT_EQ("www.sina.com", result[2].str());
    EXPECT_EQ("news/1.jpg", result[3].str());
}

TEST_F(Regex_Test, regex_replace)
{
    std::string str("there is a subsequence in the string");
    std::regex pattern("\\b(sub)([^ ]*)");

    EXPECT_EQ("there is a sub-sequence in the string", std::regex_replace(str, pattern, "sub-$2"));
    EXPECT_EQ("sub-sequence", std::regex_replace(str, pattern, "sub-$2", std::regex_constants::format_no_copy));

    std::string result;
    std::regex_replace(std::back_inserter(result), str.begin(), str.end(), pattern, "$2");
    EXPECT_EQ("there is a sequence in the string", result);


    std::string week = "monday tuesday wednesday thursday friday";
    result = std::regex_replace(week, std::regex("day"), "days");
    EXPECT_EQ("mondays tuesdays wednesdays thursdays fridays", result);
}
