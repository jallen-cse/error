
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
#include "expected/include/tl/expected.hpp"

tl::expected<int, jack::error> foo()
{
    // return jack::error(10, "what_the_hell");
    return tl::make_unexpected(jack::error(101, "what the hell"));
}

int main()
{

    auto err = foo();
    if (err) {
        printf("no err\n");
    } else {
        auto e = std::move(err.error());
        e.wrap("there was a problem");
        auto re = std::move(e.reason());
        printf("%s\n", re.c_str());
        printf("%u\n", e.reason().size());
        printf("%s\n", err.error().reason().c_str());
    }


    jack::error er(MyErrCode::RPCError, "failed to get number ", 11, " from board");

    printf("%s\n", er.reason().c_str());













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