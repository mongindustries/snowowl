# `swl` SnowOwl: engine code

This directory contains the code for the core engine.

## Contents

Each module is defined as a Gradle Native component.

- `swl-cx` Core components
- `swl-gx` Graphics components
- `swl-io` Input/Output components (from keyboard/gamepad to file streaming)
- `swl-sx` Sound components
- `swl-ui` User Interface components

In each module the project directory configuration is laid out like this:

- `root`
    - `build`: Build artifacts
    - `src`: Source files
        - `main`: Common files
            - `cpp`: C++ source files
            - `headers`: Private header files
            - `public`: Public header files
        - `platform`: Platfrom-specific files
            - `darwin`: macOS/iOS
            - `windows`: Windows
            - `linux`: Linux/Android
        - `test`: Unit test
            - `cpp`: Unit test cases
    - `build.gradle.kts`: Build configuration file