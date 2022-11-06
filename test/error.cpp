
#include <cstring>

// let catch define main
#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"
#include "jack/error.hpp"

namespace jack 
{
inline bool operator==(const jack::reason& lhs, const char* rhs)
{
    return !strcmp(lhs.c_str(), rhs);
}
}

// TODO given reason's constructor overloads, we could really just
// let most of the desc arguments get coerced into reasons by the compiler
TEST_CASE("error constructors", "[error.constructors]")
{   
    // c string constructor
    jack::error e0(101, "a fail reason");
    REQUIRE(e0.code == 101);
    REQUIRE(e0.desc == "a fail reason");

    jack::reason rs("some fail reason");
    
    // copy from reason
    jack::error e1(101, rs);
    REQUIRE(e1.code == 101);
    REQUIRE(e1.desc == "some fail reason");

    // move from reason
    jack::error e2(101, std::move(rs));
    REQUIRE(e2.code == 101);
    REQUIRE(e2.desc == "some fail reason");

    // copy constructor; other error
    jack::error e3(e2);
    REQUIRE(e3.code == 101);
    REQUIRE(e3.desc == "some fail reason");

    // move constructor; other error
    jack::error e4(e2);
    REQUIRE(e4.code == 101);
    REQUIRE(e4.desc == "some fail reason");

    std::string ss("another fail reason");

    // copy from string
    jack::error e5(202, ss);
    REQUIRE(e5.code == 202);
    REQUIRE(e5.desc == "another fail reason");

    // move from string
    jack::error e6(202, std::move(ss));
    REQUIRE(e6.code == 202);
    REQUIRE(e6.desc == "another fail reason");

    // variadic reason construction
    jack::error e7(101, "some fail reason w/ val ", 100, " plus more info");
    REQUIRE(e7.code == 101);
    REQUIRE(e7.desc == "some fail reason w/ val 100 plus more info");
}

TEST_CASE("error operator=", "[error.assignment]")
{
    // copy assignment
    jack::error es0(101, "a fail reason");
    jack::error ed0(0, "");
    ed0 = es0;
    REQUIRE(ed0.code == 101);
    REQUIRE(ed0.desc == "a fail reason");

    // move assignment
    jack::error es1(101, "a fail reason");
    jack::error ed1(0, "");
    ed1 = std::move(es1);
    REQUIRE(ed1.code == 101);
    REQUIRE(ed1.desc == "a fail reason");
}

TEST_CASE("error::wrap member function", "[error.wrap]")
{
    jack::error e0(10, "some fail reason");
    e0.wrap("more context");
    REQUIRE(e0.code == 10);
    REQUIRE(e0.desc == "more context: some fail reason");
}

TEST_CASE("error::extend member function", "[error.extend]")
{
    jack::error e0(10, "some fail reason");
    e0.extend("more info");
    REQUIRE(e0.code == 10);
    REQUIRE(e0.desc == "some fail reason: more info");
}