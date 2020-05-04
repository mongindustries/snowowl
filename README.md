# SnowOwl:
A 2D vector graphics game toolkit

## Introduction
I'm building this toolkit as a way to explore modern graphics rendering through DX12, Vulkan, and Metal. In addition, since I have too much free time because of working at home (let's not talk why im doing WFH...) why not revive a project that is in hiatus for 5 years!

The codebase is written using C++20, mostly modules can be compiled in C++14/17 but the `rd` module can only be compiled using C++20 as it uses concepts.

## Roadmap
I have no clear roadmap for this project. As of now I am focused on finishing the `gx` module (1/3 complete!). Some parts in `cx` and `ui` are working and some unit tests in `cx` exists to verify stuff.

Some of my end goals are:
- Run exact code on Win32, UWP, macOS, iOS, and Android
- Create a basic entity system
- Create a simple game (like snake? or flappy bird?)
- Publish game to the Appstore/Google Play

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

### macOS and iOS
There is a xcodegen configuration file located in build/xcodegen (to be added soon). Install xcodegen via homebrew:

```
mong@mong> brew install xcodegen
```

Then `cd` to the `build/xcodegen` folder and run:

```
mong@xcodegen> xcodegen
```

## Author

Michael Ong (@rantingmong)
