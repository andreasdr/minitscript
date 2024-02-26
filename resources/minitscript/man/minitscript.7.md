This is the home of MinitScript - the mini (C++) transpileable scripting language.

This document and repository is WIP. Expect a BETA release around 2024-03-01.\
See section 1.3. for 1.0 BETA release TODO list.

# 1. Introduction

MinitScript was developed as part of our [TDME2 3D Engine](https://github.com/Mindty-Kollektiv/tdme2) to match
the requirements for high performance script execution, as well as straight forward game logic scripting.

MinitScript might borrow some ideas from JavaScript, Kotlin, PHP and even C# and C++, which you might like and find familier.
Also note the focus on easy integration into other products and easy customizability and extendability.

## 1.1. Open source

- MinitScript is open source
  - please check the [license](https://github.com/Mindty-Kollektiv/minitscript/blob/main/LICENSE)
  - you find the source code at [https://github.com/Mindty-Kollektiv/minitscript](https://github.com/Mindty-Kollektiv/minitscript) 
  - here you find developer [documentation including collaboration graphs and API](https://www.mindty.com/products/minitscript/documentation/)

## 1.2. Language features

- Introduction to MinitScript language features:
  - very small implementation of a scripting language
  - runs on every CPU, OS, ... due to its simplicity, so its highly portable
  - can be easily extended by writing state machine machine states and script methods in C++ as well as custom data types
  - built-in data types: null, boolean, integer, float, string, byte array, array, map and set, ...
  - when calling script C++ methods or script functions with arguments it does optionally use references or value by copy
  - supports functions/stacklets and recursion
  - supports lamda functions and inline stacklets
  - supports operators by operator to method mapping by a preprocessor run
  - supports loops, conditions and switch/case/default blocks
  - supports programming with classes style programming
    - for built-in datatypes: string, byte array, array, map and set, ...
    - for script classes/objects
    - for custom data types
  - supports event like programming
  - unicode support via UTF8
  - can be transpiled to C++

## 1.3. 1.0 BETA release TODO list

- Whats left to do for 1.0 BETA release
  - some adjustments
    - private members: transpilated/native: check output, it differs from interpreter mode
    - multi line statement error messages are ugly as fuck???
    - make event system optional???
    - preprocessor definition for data files
    - native scripts should also be executable without original scripts(optional) and with changed scripts(interpreted mode)
  - tests
    - native tests for MSC/MINGW
    - bubblesort benchmark vs java script or other script engines, just to get the idea of current state of performance
  - documentation
    - detailed docu for methods and section
    - ...
  - installer for Windows
  - installer for MacOSX
