package net.rantingmong.snowowl

import org.gradle.api.Project
import org.gradle.language.cpp.CppComponent
import org.gradle.language.cpp.CppSharedLibrary
import org.gradle.language.cpp.CppStaticLibrary
import org.gradle.nativeplatform.OperatingSystemFamily
import org.gradle.nativeplatform.toolchain.Clang
import org.gradle.nativeplatform.toolchain.VisualCpp

fun CppComponent.configureCppProject(master: Project) = run {

  val logger = master.logger

  logger.lifecycle("[mong] Begin configuration:")

  binaries.whenElementFinalized {
    logger.lifecycle("$name:")

    val compileArgs = mutableListOf<String>()
    val targetMachine = targetMachine.operatingSystemFamily.name

    if (this is CppSharedLibrary || this is CppStaticLibrary) {

      logger.lifecycle("  Applying per-platform specific source path (${targetMachine}).")

      source {

        from.add("src/main/cpp")

        when (targetMachine) {
          OperatingSystemFamily.MACOS -> from.add("src/platform/darwin")
          OperatingSystemFamily.WINDOWS -> from.add("src/platform/windows")
        }
      }
    }

    when (toolChain) {
      is Clang -> {
        logger.lifecycle("  Applying c++17 language standard.")
        compileArgs.add("-std=c++17")
      }

      is VisualCpp -> {
        logger.lifecycle("  Applying c++17 language standard.")
        compileArgs.add("/std:c++latest")
        compileArgs.add("/EHsc")
      }
    }

    if (name.contains("debug", true)) {
      compileTask.get().macros["SWL_DEBUG"] = "1"
    }

    compileTask.get().compilerArgs.set(compileArgs)
  }
}
