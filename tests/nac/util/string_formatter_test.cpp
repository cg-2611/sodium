#include "sodium/nac/util/string_formatter.h"

#include <string>

#include <gtest/gtest.h>

TEST(StringFormatterTest, StringFormatterFormatsCorrectly) {
    std::string format("%d, %c, %s, %s, %.7s %.*s");
    std::string test_string("test std::string");
    std::string formatted_string(sodium::StringFormatter::format_string(
        format, 2, 'b', "test string", test_string.c_str(), "another test string", 4, test_string.c_str()));

    EXPECT_EQ("2, b, test string, test std::string, another test", formatted_string);
}

TEST(StringFormatterTest, StringFormatterHandlesEmptyFormatString) {
    std::string empty_format;
    std::string test_string("test string");
    std::string formatted_string(sodium::StringFormatter::format_string(empty_format, test_string.c_str()));

    EXPECT_EQ("", formatted_string);
}

TEST(StringFormatterTest, StringFormatterHandlesEmptyArgumentString) {
    std::string format("%s");
    std::string empty_string;
    std::string formatted_string(sodium::StringFormatter::format_string(format, empty_string.c_str()));

    EXPECT_EQ("", formatted_string);
}
