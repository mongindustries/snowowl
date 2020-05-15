# <img src="https://raw.githubusercontent.com/mongindustries/snowowl/master/logo.png" width=20/> SnowOwl:
![Build Status](https://github.com/mongindustries/snowowl/workflows/swl%20build/badge.svg?branch=develop)

A 2D/3D game toolkit

## Introduction
_Since I have too much free time because of working at home (let's not talk why im doing WFH...) why not revive a project that is in hiatus for 5 years!_

I'm building this toolkit as a way to explore modern graphics rendering through DX12, Vulkan, and Metal. To add, this will serve as the cross-platform foundation for building an actual game engine (see mongindustries/sushi for progress).

The codebase is written using C++20, mostly modules can be compiled in C++14/17 but the `rd` module can only be compiled using C++20 as it uses concepts.

## What's in and what's not
This project focuses on the low-level nitty gritty stuff. It has abstractions for:
- Window management
- Device input
- Graphics
- Sound
- Storage
- Network 

This will **not** be a project that can help you guild games; its a project that _will_ help you build the tech for your games.

Some of the goals for this project are:
- Be able to run code on various platforms
- Abstract API differences across various platform's API with an opinionated subset
- Simplify my creation for an actual game engine

## Documentation
Coming soon!

## Building

### Everything else
Its a cmake project! Recommended project structure for CMake is to create a `build` folder on the root:

```
mong@snowowl> mkdir cmbuild
```

`cd` to it and run CMake:

```
mong@snowowl> cd cmbuild
mong@cmbuild> cmake ../swl -GNinja
```

To disable unit tests add `-DPACKAGE_TESTS=OFF` to the command line arguments.

### Windows
Open Command Prompt (not powershell as this will not work!) and do the command:

```
C:\<path to snow owl>\build\win32> "<path to visual studio cmake>\cmake.exe" -G"Visual Studio 16 2019"
```

### Windows Runtime
Open Command Prompt (not powershell as this will not work!) and do the command:

```
C:\<path to snow owl>\build\vs_uwp> "<path to visual studio cmake>\cmake.exe" -G"Visual Studio 16 2019" -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0
```

### macOS and iOS
There is a xcodegen configuration file located in build/xcodegen. Install xcodegen via homebrew:

```
mong@mong> brew install xcodegen
```

Then `cd` to the `build/xcodegen` folder and run:

```
mong@xcodegen> xcodegen
```

## Author

Michael Ong (@rantingmong)
