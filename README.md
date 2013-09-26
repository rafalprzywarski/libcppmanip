# libcppmanip
**libcppmanip** is a [clang](http://clang.llvm.org)-based library that provides a reliable way to perform C++ refactoring that can be used in other tools (e.g. IDE plugins).
If you need a standalone tool, check out Maciej's [kdevcpprefactor](https://github.com/mcencora/kdevcpprefactor).

## Partially working features
 + extract function

## Planned features
 + extract method
 + inline method
 + change method signature
 + introduce local variable
 + introduce method parameter

## Installation

Currently, the only way to go is building from source.

## Building

You need the following dependencies/tools:
 *   libclang 3.2
 *   Boost
 *   cmake
 *   rake
 *   cucumber
 *   rspec

Using a newer version of libclang requires minor code changes. Support for newer versions of libclang will be provided soon.

To build the library simply run:

    rake

It will build the library and run all acceptance and unit test.
The binary can be found in library/build.
In order to install it, do the following:

    cd library/build
    sudo make install
