# Error 

Basic types for representing erroneous circumstances in modern C++.

[![Version](https://img.shields.io/github/v/release/jallen-cse/error?sort=semver)](https://github.com/jallen-cse/error/releases/latest)
[![License: MIT](https://img.shields.io/github/license/jallen-cse/error?color=blue&style=shield)](./LICENSE)
[![CircleCI](https://circleci.com/gh/jallen-cse/error.svg?style=shield)](https://circleci.com/gh/jallen-cse/error)

This repository was born of annoyances when percolating errors up a call stack. Error is specifically useful when there is some 
human on the calling end that needs a readable message about what went awry.

The classes are very simple. `jack::reason` is just a `std::string` with a narrowed & extended public interface; `jack::error` is a paired `jack::reason` and `int` error code with a proxy interface to the reason member. The main utility is centered around two member functions: 
- `wrap` to **prepend** additional context to the failure description
- `extend` to **append** additional information to the failure description

I always use Error with wrappers that express uncertainty like `std::optional` (C++17) & `std::expected` (C++23) or non-standard implementations like 
those of [Sy Brand](https://github.com/TartanLlama) or within the [Boost](https://www.boost.org/) libraries. Having something that always 
produces an error would be a strange pattern!

```cpp
std::optional<jack::error> foo_may_fail() {
    const auto val = std::rand();
    return val % 2 ? 
        std::make_optional<jack::error>(1001, "rand was odd w/ val ", val) :
        std::nullopt;
}

std::optional<jack::error> bar_may_fail() {
    auto maybe_err = foo_may_fail();
    if (maybe_err) {
        maybe_err->wrap("foo failed"); // now "foo failed: rand was odd w/ val ..." 
        return maybe_err;
    } else {
        return std::nullopt;
    }
}
```

## Usage
### Copy / Paste
Error is a header-only library, so adding it to your project is very easy. Simply place the include files in your source tree and get back to other work.

### CMake
Error can also be used with CMake. Adding Error as a subdirectory will produce an interface library called `error` that other targets can link against.

## Test
Unit tests use [Catch2](https://github.com/catchorg/Catch2) and are built with CMake. To do so, use:

```shell
mkdir build && cd build
cmake .. -DJACK_ERROR_BUILD_TESTS=on
make -j$(nproc)
```
Test executables will be prefixed with `jack_test_` and will be found in the `/build/test` directory.  

## Examples
To build examples, use:

```shell
mkdir build && cd build
cmake .. -DJACK_ERROR_BUILD_EXAMPLES=on 
make -j$(nproc)
```
Example executables will be prefixed with `jack_example_` and will be found in the `/build/examples` directory.

## Docs
Documentation is contained inline in the source file but is also available through Doxygen. Open `/doc/html/annotated.html` in a browser to view the generated docs.

## Todo
I consider this project to be unfinished. Testing, features, and efficiencies can be improved. A rough list of future work is as follows:
- better tests / full code coverage
- ci other OS / arch / compilers
- format string overloads of reason::reason, ::wrap, and ::extend
- non-header-only build
- defaultable error (to avoid some wrapper overhead like w/ optional) 
