JSON for Modern C++ Version 2.0.2
@nlohmann nlohmann released this 18 days ago · 41 commits to develop since this release

Release date: 2016-07-31
MD5: ee7821cfae39a98e81f4afaff8de3712
Summary

This release combines a lot of small fixes and improvements. The release is backwards compatible.

Changes

The parser has been overworked, and a lot of small issues have been fixed:
Improved parser performance by avoiding recursion and using move semantics for the return value.
Unescaped control charaters \x10-\x1f are not accepted any more.
Fixed a bug in the parser when reading from an input stream.
Improved test case coverage for UTF-8 parsing: now, all valid Unicode code points are tested both escaped and unescaped.
The precision of output streams is now preserved by the parser.
Started to check the code correctness by proving termination of important loops. Furthermore, individual assertions have been replaced by a more systematic function which checks the class invariants. Note that assertions should be switched off in production by defining the preprocessor macro NDEBUG, see the documentation of assert.
A lot of code cleanup: removed unused headers, fixed some compiler warnings, and fixed a build error for Windows-based Clang builds.
Added some compile-time checks:
Unsupported compilers are rejected during compilation with an #error command.
Static assertion prohibits code with incompatible pointer types used in get_ptr().
Improved the documentation, and adjusted the documentation script to choose the correct version of sed.
Replaced a lot of "raw loops" by STL functions like std::all_of, std::for_each, or std::accumulate. This facilitates reasoning about termination of loops and sometimes allowed to simplify functions to a single return statement.
Implemented a value() function for JSON pointers (similar to at function).
The Homebrew formula (see Integration) is now tested for all Xcode builds (6.1 - 8.x) with Travis.
Avoided output to std::cout in the test cases.