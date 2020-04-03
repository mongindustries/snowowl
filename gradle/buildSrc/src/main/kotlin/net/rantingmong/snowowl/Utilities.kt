package net.rantingmong.snowowl

import org.gradle.api.Project
import org.gradle.language.cpp.CppComponent
import org.gradle.language.cpp.CppLibrary
import org.gradle.language.cpp.CppSharedLibrary
import org.gradle.language.cpp.CppStaticLibrary
import org.gradle.nativeplatform.Linkage
import org.gradle.nativeplatform.OperatingSystemFamily
import org.gradle.nativeplatform.toolchain.Clang
import org.gradle.nativeplatform.toolchain.VisualCpp
import kotlin.collections.set

fun CppComponent.configureCppProject(master: Project) = run {

  val logger = master.logger

  logger.lifecycle("[mong|$name] Begin configuration:")

  if (this is CppLibrary) {
    linkage.set(listOf(Linkage.SHARED, Linkage.STATIC))
  }

  val componentName = name
  binaries.whenElementFinalized {
    logger.lifecycle("$componentName|$name:")

    val compileArgs = mutableListOf<String>()
    val targetMachine = targetMachine.operatingSystemFamily.name

    logger.lifecycle("  Applying per-platform specific source path (${targetMachine}).")

    var platform = ""

    when (targetMachine) {
      OperatingSystemFamily.MACOS -> {
        platform = "darwin"
      }
      OperatingSystemFamily.LINUX -> {
        platform = "linux"
      }
      OperatingSystemFamily.WINDOWS -> {
        platform = "windows"
      }
    }

    if (this is CppStaticLibrary || this is CppSharedLibrary) {

      privateHeaders {
        from.add("src/main/platform/$platform")
        from.add("src/main/headers")
      }

      source {

        from.add("src/main/cpp")
        from.add("src/main/platform/$platform")
      }
    }

    when (toolChain) {
      is Clang -> {
        logger.lifecycle("  Applying c++17 language standard.")
        compileArgs.add("-std=c++17")
      }

      is VisualCpp -> {
        logger.lifecycle("  Applying c++17 language standard.")
        compileArgs.add("/std:c++17")
        compileArgs.add("/EHsc")
      }
    }

    compileTask.get().macros.run {

      when (targetMachine) {
        OperatingSystemFamily.WINDOWS ->{
          this["SWL_WIN32"] = "1"
          this["UNICODE"] = "1"
        }
        OperatingSystemFamily.LINUX   -> this["SWL_LINUX"] = "1"
        OperatingSystemFamily.MACOS   -> this["SWL_DARWIN"] = "1"
      }

      if (name.contains("debug", true)) {
        this["SWL_DEBUG"] = "1"
      }
    }

    compileTask.get().compilerArgs.addAll(compileArgs)
  }
}
