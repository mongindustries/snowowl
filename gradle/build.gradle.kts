allprojects {

    apply {
        plugin("org.gradle.base")
        plugin("org.gradle.cpp-unit-test")

        plugin("org.gradle.visual-studio")
    }

    repositories {
        jcenter()
    }

    version = "1.0.0"
}
