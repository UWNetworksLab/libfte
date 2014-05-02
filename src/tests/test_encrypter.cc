#include <regex>

#include "gtest/gtest.h"

#include "dfas.h"
#include "fte/encrypter.h"

TEST(SanityCheck, Test1) {
    std::string str = "aaaaaaaaaaaaaaaa";
    std::regex rx("^(a|b){16}$");
    bool match = regex_match(str.begin(), str.end(), rx);
    EXPECT_EQ( true, match );
}

TEST(SanityCheck, Test2) {
    std::string str = "bbbbbbbbbbbbbbbb";
    std::regex rx("^(a|b){16}$");
    bool match = regex_match(str.begin(), str.end(), rx);
    EXPECT_EQ( true, match );
}

/*TEST(FteNormalUsage, Test1) {
    fte::encrypter fteObj( VALID_DFA_1, 16,
                           VALID_DFA_1, 16,
                           "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF" );

    std::string input_plaintext = "aaaabbbb";
    std::string ciphertext = fteObj.encrypt( input_plaintext );
    std::string output_plaintext = fteObj.decrypt( ciphertext );

    EXPECT_EQ( input_plaintext, output_plaintext );

    std::regex rx("^(a|b){1,16}$");
    bool match = regex_match(ciphertext.begin(), ciphertext.end(), rx);

    EXPECT_EQ( true, match );
}*/
