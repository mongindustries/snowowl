allprojects {

    apply {
        plugin("org.gradle.base")
        plugin("org.gradle.cpp-unit-test")
    }

    repositories {
        jcenter()
    }

    version = "1.0.0"
}
