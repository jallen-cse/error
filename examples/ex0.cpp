
#include "jack/error.hpp"

enum MyErrCode
{
    ParseError,
    RPCError,
};


// class a
// {
//     constexpr a(const int code, const char* reason) :
//             code(code), reason(reason) 
//     {
//         static_assert(code != 0, "what");
//     }

//     int code;
//     const char* reason; 
// };



// jack::error returns_something_but_could_fail()
// {
//     // return jack::error::some(10, "what the");
//     return jack::error::none();
// }

// jack::reason returns_nothing_but_may_fail()
// {
//     // return jack::reason::none();
//     return jack::reason::some("abcd");
// }

#include <iostream>
// #include "expected/include/tl/expected.hpp"

// tl::expected<int, jack::error> foo()
// {
//     // return jack::error(10, "what_the_hell");
//     return tl::make_unexpected(jack::error(101, "what the hell"));
// }

// #include "fake_optional.hpp"

// optional<jack::reason> might_produce_reason()
// {
//     return jack::reason("what in the ", 101, "world");
// }

// optional<jack::error> might_produce_error()
// {
//     return jack::error(101, "ajsjjas", 1001);
// }

int main()
{

    // auto err = foo();
    // if (err) {
    //     printf("no err\n");
    // } else {
    //     auto e = std::move(err.error());
    //     e.wrap("there was a problem");
    //     auto re = std::move(e.reason());
    //     printf("%s\n", re.c_str());
    //     printf("%u\n", e.reason().size());
    //     printf("%s\n", err.error().reason().c_str());
    // }

    // jack::error er(MyErrCode::RPCError, "failed to get number ", 11, " from board");


    // printf("%s\n", er.reason().c_str());


    // auto err = might_produce_error();
    // if (err)
    // {
    //     (*err).extend("what", "the hell", 100)
    // }


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

    std::cout << error0.debug_string() << '\n'
              << error1.debug_string() << '\n';


    // std::cout << reason
    //     .wrap(jack::reason("failed to do something"))
    //     .extend("number is bad") << '\n';

    // std::cout << jack::error(101, "a fail reason").debug_string() << '\n';

    







    // std::cout << sizeof(jack::reason) << '\n';
    // // std::cout << sizeof(jack::error) << '\n';
    // // std::cout << (void*)"" << '\n';
    // // std::cout << sizeof(short) << '\n';
    
    // // a ana(10, "hello world");

    // auto x = returns_nothing_but_may_fail()
    //     .if_some([] (jack::reason* s) {

    //     })
    //     .if_none([] {

    //     });


    // std::cout << std::string("abcd").empty() << '\n';
    // std::cout << std::string("").empty() << '\n';
    

    // int x = Code0;
}