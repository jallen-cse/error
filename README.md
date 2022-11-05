# Error
Basic types for representing erroneous circumstances in modern C++.

This repository was born of headaches when percolating errors up the call stack. This library is specifically useful when there is some 
human on the calling end that needs a readable message about what in the world happened.  

I always use Error with classes like `std::optional` (C++17) & `std::expected` (C++23) or non-standard implementations like 
those of [Sy Brand](https://github.com/TartanLlama) or within the [Boost](https://www.boost.org/) libraries. Having something that always 
produces an error is quite a strange pattern!

TODO
- test
- ci
- more readme
- cmake
- split header
- version / tag / release
