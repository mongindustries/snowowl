/*
 * This C++ source file was generated by the Gradle 'init' task.
 */

#include "app.h"
#include <cassert>

#include <core.hpp>

int main() {
    snow_owl::Greeter greeter;
    assert(greeter.greeting() == "Hello, World " + swl::cx::Core::version);
    return 0;
}
