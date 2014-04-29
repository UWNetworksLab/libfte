#include "gtest/gtest.h"
#include "fte.h"

TEST(FteNormalUsage, Test1) {
    fte fteObj( "", "" );
    std::string input_plaintext = "Hello, World";
    std::string ciphertext = fteObj.encrypt( input_plaintext );
    std::string output_plaintext = fteObj.decrypt( ciphertext );
    EXPECT_EQ( input_plaintext, output_plaintext );
}