
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

TEST_CASE("reason constructors", "[reason.constructors]")
{
    // variadic constructor
    jack::reason r0("a fail reason w/ value ", 100);
    REQUIRE(r0 == "a fail reason w/ value 100");
    
    // move constructor; other reason
    jack::reason r1(std::move(r0));
    REQUIRE(r1 == "a fail reason w/ value 100");

    // copy constructor; other reason
    jack::reason r2(r1);
    REQUIRE(r1.c_str() != r2.c_str());              // internal buffers should differ
    REQUIRE(r2 == "a fail reason w/ value 100");

    // c string constructor
    jack::reason r3("some other fail reason");
    REQUIRE(r3 == "some other fail reason");

    std::string src("another fail reason");

    // copy constructor; std::string
    jack::reason r4(src);
    REQUIRE(r4 == "another fail reason");

    // move constructor; std::string
    jack::reason r5(std::move(src));
    REQUIRE(r5 == "another fail reason");
}

TEST_CASE("reason operator=", "[reason.assignment]")
{
    // copy assignment
    jack::reason rs0("a reason");
    jack::reason rd0("");
    rd0 = rs0;
    REQUIRE(rs0.c_str() != rd0.c_str());    // internal buffers are different
    REQUIRE(rd0 == "a reason");             // value is correct

    // move assignment
    jack::reason rs1("a reason");
    jack::reason rd1("");
    rd1 = std::move(rs1);
    REQUIRE(rd1 == "a reason");             // value is correct
}

TEST_CASE("reason::wrap member function", "[reason.wrap]")
{
    jack::reason r0("a fail reason");
    r0.wrap("const char* ctx");
    REQUIRE(r0 == "const char* ctx: a fail reason");

    jack::reason r1("a fail reason");
    r1.wrap(std::string("string ctx"));
    REQUIRE(r1 == "string ctx: a fail reason");

    jack::reason r2("a fail reason");
    r2.wrap(jack::reason("reason ctx"));
    REQUIRE(r2 == "reason ctx: a fail reason");

    jack::reason r3("a fail reason");
    r3.wrap("ap0, ", std::string("ap1, "), jack::reason("ap2, "), 100);
    REQUIRE(r3 == "ap0, ap1, ap2, 100: a fail reason");
}

TEST_CASE("reason::extend member function", "[reason.extend]")
{
    jack::reason r0("a fail reason");
    r0.extend("const char* info");
    REQUIRE(r0 == "a fail reason: const char* info");

    jack::reason r1("a fail reason");
    r1.extend(std::string("string info"));
    REQUIRE(r1 == "a fail reason: string info");

    jack::reason r2("a fail reason");
    r2.extend(jack::reason("reason info"));
    REQUIRE(r2 == "a fail reason: reason info");

    jack::reason r3("a fail reason");
    r3.extend("ap0, ", std::string("ap1, "), jack::reason("ap2, "), 100);
    REQUIRE(r3 == "a fail reason: ap0, ap1, ap2, 100");
}