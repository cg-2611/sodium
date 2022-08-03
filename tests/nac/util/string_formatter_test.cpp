#include "sodium/nac/util/string_formatter.h"

#include <string>

#include <gtest/gtest.h>

TEST(StringFormatterTest, StringFormatterFormatsCorrectly) {
    std::string format("%d, %c, %s, %s, %.7s %.*s");
    std::string testString("test std::string");
    std::string formattedString(nac::StringFormatter::formatString(format, 12, 'b', "test string", testString.c_str(),
                                                                   "another test string", 4, testString.c_str()));

    EXPECT_EQ("12, b, test string, test std::string, another test", formattedString);
}

TEST(StringFormatterTest, StringFormatterHandlesEmptyFormatString) {
    std::string emptyFormat("");
    std::string testString("test string");
    std::string formattedString(nac::StringFormatter::formatString(emptyFormat, testString.c_str()));

    EXPECT_EQ("", formattedString);
}

TEST(StringFormatterTest, StringFormatterHandlesEmptyArgumentString) {
    std::string format("%s");
    std::string emptyString("");
    std::string formattedString(nac::StringFormatter::formatString(format, emptyString.c_str()));

    EXPECT_EQ("", formattedString);
}
