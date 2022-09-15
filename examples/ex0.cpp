
#include <iostream>

#include "jack/error.hpp"

int main()
{
    jack::reason reason0("a fail reason");
    jack::reason reason1("a fail reason");

    reason0.wrap("const char* ctx");
    reason0.wrap(std::string("string ctx"));
    reason0.wrap(jack::reason("reason ctx"));
    reason0.wrap("ap0, ", std::string("ap1, "), jack::reason("ap2, "), 100);

    reason1.extend("const char* ctx");
    reason1.extend(std::string("string ctx"));
    reason1.extend(jack::reason("reason ctx"));
    reason1.extend("ap0, ", std::string("ap1, "), jack::reason("ap2, "), 100);

    std::cout << reason0 << '\n'
              << reason1 << '\n';

    jack::error error0(100, "a fail reason");
    jack::error error1(100, "a fail reason");

    error0.wrap("const char* ctx");
    error0.wrap(std::string("string ctx"));
    error0.wrap(jack::reason("reason ctx"));
    error0.wrap("ap0, ", std::string("ap1, "), jack::reason("ap2, "), 100);

    error1.extend("const char* ctx");
    error1.extend(std::string("string ctx"));
    error1.extend(jack::reason("reason ctx"));
    error1.extend("ap0, ", std::string("ap1, "), jack::reason("ap2, "), 100);

    std::cout << jack::debug::str(error0) << '\n'
              << jack::debug::str(error1) << '\n';
}