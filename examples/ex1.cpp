
#include <iostream>

#if __cplusplus < 201703L
int main() { std::cout << "This example requires C++17\n"; }
#else

#include <ctime>
#include <optional>

#include "jack/error.hpp"

std::optional<jack::error> foo_may_fail() {
    const auto val = std::rand();
    return val % 2 ? 
        std::make_optional<jack::error>(1001, "rand was odd w/ val ", val) :
        std::nullopt;
}

std::optional<jack::error> bar_may_fail() {
    auto maybe_err = foo_may_fail();
    if (maybe_err) {
        maybe_err->wrap("foo failed"); // now == "foo failed: rand was odd w/ val ..." 
        return maybe_err;
    } else {
        return std::nullopt;
    }
}

int main()
{
    std::srand(std::time(nullptr));
    for (int i = 0; i < 10; ++i)
    {
        auto v = bar_may_fail();
        if (v) std::cout << jack::debug::str(*v) << '\n';
    }
}

#endif // __cplusplus < 201703L