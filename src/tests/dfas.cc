#include <iostream>
#include <string.h>
#include "dfas.h"

// completely invalid format
std::string INVALID_DFA_1 = "";
std::string INVALID_DFA_2 = "0";
std::string INVALID_DFA_3 = "abcdefg";

// no final states
std::string INVALID_DFA_4 = "0	1	0	0";

// final state, but not path to it
std::string INVALID_DFA_5 = "0	1	0	0\n"
                            "X";

// no reachable final states
std::string INVALID_DFA_6 = "0	1	0	0\n"
                            "2";

// regex1 = ^(a|b)+$
std::string VALID_DFA_1 = "0	1	97	97\n"
                          "0	1	98	98\n"
                          "1	1	97	97\n"
                          "1	1	98	98\n"
                          "1";
