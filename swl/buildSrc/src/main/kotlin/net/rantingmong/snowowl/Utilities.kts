package net.rantingmong.snowowl

import org.gradle.api.Project
import org.gradle.language.cpp.CppBinary
import org.gradle.language.cpp.CppSharedLibrary
import org.gradle.language.cpp.CppStaticLibrary
import org.gradle.language.cpp.ProductionCppComponent
import org.gradle.language.cpp.tasks.CppCompile
import org.gradle.nativeplatform.OperatingSystemFamily
import org.gradle.nativeplatform.toolchain.Clang

class Utilities {
  companion object {

    inline fun <reified Comp : ProductionCppComponent> asCppProject(master: Project, cppProject: Comp) = cppProject.apply {

      master.logger.lifecycle("mong: Begin configuration:")

      master.tasks.withType(CppCompile::class.java) {
        if (name.contains("debug", true)) {
          macros["SWL_DEBUG"] = "1"
        } else {

        }
      }

      binaries.whenElementFinalized(CppBinary::class.java) {

        val args          = mutableListOf<String>()
        val targetMachine = targetMachine.operatingSystemFamily.name

        if (this is CppSharedLibrary || this is CppStaticLibrary) {

          master.logger.lifecycle("  Applying per-platform specific source path (${targetMachine}).")

          source {

            from.add("src/main/cpp")

            when (targetMachine) {
              OperatingSystemFamily.MACOS   -> from.add("src/platform/darwin")
              OperatingSystemFamily.WINDOWS -> from.add("src/platform/windows")
            }
          }
        }

        when (toolChain) {
          is Clang -> {
            master.logger.lifecycle("  Applying c++17 language standard.")

            args.add("-std=c++17")

            this.compileTask.get().compilerArgs.set(args)
          }
        }
      }
    }
  }
}
