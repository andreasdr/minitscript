[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://github.com/andreasdr/minitscript/blob/master/LICENSE)
[![NIX Build Status](https://github.com/andreasdr/minitscript/actions/workflows/nix.yml/badge.svg)](https://github.com/andreasdr/minitscript/actions)
[![MacOSX Build Status](https://github.com/andreasdr/minitscript/actions/workflows/macosx.yml/badge.svg)](https://github.com/andreasdr/minitscript/actions)
[![Windows/MINGW Build Status](https://github.com/andreasdr/minitscript/actions/workflows/windows-mingw.yml/badge.svg)](https://github.com/andreasdr/minitscript/actions)
[![Windows/MSC Build Status](https://github.com/andreasdr/minitscript/actions/workflows/windows-msc.yml/badge.svg)](https://github.com/andreasdr/minitscript/actions)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/29926/badge.svg)](https://scan.coverity.com/projects/mindty-kollektiv-minitscript)
  
![LOGO](https://raw.githubusercontent.com/andreasdr/minitscript/master/resources/github/minitscript-logo.png)

This is the home of MinitScript - The mini C++ transpileable scripting language.

This document and repository is WIP. Expect a BETA release around 2025-03-01.\
See section 1.3. for 1.0 BETA release TODO list.

# 1. Introduction

MinitScript is developed as part of our [TDME2 3D Engine](https://github.com/andreasdr/tdme2) to match
the requirements for high performance script execution, as well as straight forward game logic scripting.

MinitScript might borrow some ideas from JavaScript, Kotlin, PHP and even Pascal, C# and C++, which you might like and find familier.
Also note the focus on easy integration into other products and easy customizability and extendability.

## 1.1. Open source

- MinitScript is open source
  - please check the [license](https://github.com/andreasdr/minitscript/blob/main/LICENSE)
  - you find the source code at [https://github.com/andreasdr/minitscript](https://github.com/andreasdr/minitscript) 
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
  - supports exceptions
  - supports modules
  - unicode support via UTF8
  - can be transpiled to C++

## 1.3. 1.0 BETA release TODO list

- Whats left to do for 1.0 BETA release
  - some adjustments
    - exceptions(WIP)
    - have a more sophisticated parser
    - have interfaces
    - optional: make event system optional??? what about error emits/errors in functions/stacklets??? 
    - build system: check executable output directories
  - parser
    - improve error messages even more
  - documentation
    - detailed documentation for specific sections and maybe methods
    - README.md: resolve TODOs
    - README-BuildingHowTo.md: resolve TODOs
    - README-Syntax.md: TODO
    - README-Extension.md: TODO
    - README-Integration.md: TODO
    - ...
  - installer for Windows
  - installer for MacOSX

# 2. Language documentation

- [Syntax](./documentation/README-Syntax.md)
- [Data types, variables and constants](./documentation/README-DataTypes.md)
- [Flow control - if, elseif and else](./documentation/README-FlowControl-Conditions.md)
- [Flow control - switch, case and default](./documentation/README-FlowControl-Conditions2.md)
- [Flow control - forTime, forCondition, for and forEach](./documentation/README-FlowControl-Loops.md)
- [Flow control - exceptions](./documentation/README-FlowControl-Exceptions.md)
- [Functions](./documentation/README-Functions.md)
- [Lambda functions](./documentation/README-Lambda-Functions.md)
- [Classes](./documentation/README-Classes.md)
- [Modules](./documentation/README-Modules.md)
- [Built-in functions](./documentation/README-BuiltIn-Functions.md)
- [Built-in classes](./documentation/README-BuiltIn-Classes.md)
- [Operators](./documentation/README-Operators.md)
- [Constants](./documentation/README-Constants.md)


## 3. Links

- MinitScript, see [README.md](./README.md)
- MinitScript - How to build, see [README-BuildingHowTo.md](./README-BuildingHowTo.md)
- MinitScript - How to use, see [README-Tools.md](./README-Tools.md)
- The Mindty Kollektiv [Discord Server](https://discord.gg/Na4ACaFD)

# 11. Donation 
- I have to pay my bills, beer and stuff, so if you have any use for this open source project, like educational, productive or fun, ..., consider a donation here [PayPal](https://www.paypal.me/andreasdrewke), here [GitHub Sponsors](https://github.com/sponsors/andreasdr) or via [Amazon Wishlist](https://www.amazon.de/hz/wishlist/ls/250IBUL4JSZVR?ref_=wl_share)

# 12. Bounties
- We will most likely accept MinitScript (related) tasks as kind of bounties for a customized MinitScript or default MinitScript
