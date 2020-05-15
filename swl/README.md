# `swl` SnowOwl: low level engine code

This directory contains the code for the core engine. This does not expose high level stuff that is (ex: node graph, GUI)

## Contents

Each module is defined as a cmake component.

- `swl-cx` Core components
- `swl-gx` Graphics components
- `swl-rd` Rendercore components
- `swl-ui` Window/App system components

In each module the project directory configuration is laid out like this:

- `root`
    - `build`: Build artifacts
    - `src`: Source files
        - `main`: Common files
            - `cpp`: C++ source files
            - `headers`: Private header files
            - `public`: Public header files
          `CMakeLists.txt`: Build configuration file
        - `platform`: Platfrom-specific files
        - `test`: Unit test
            - `cpp`: Unit test cases
