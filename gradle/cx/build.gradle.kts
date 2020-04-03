import net.rantingmong.snowowl.configureCppProject

plugins {
  `cpp-library`
}

group = "engine"

library {

  targetMachines.set(listOf(machines.macOS.x86_64, machines.windows.x86_64))

  configureCppProject(project)
}

unitTest {
  configureCppProject(project)

  dependencies {
    implementation(project(":deps:googletest"))
  }
}
