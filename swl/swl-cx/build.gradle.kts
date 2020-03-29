import net.rantingmong.snowowl.configureCppProject

plugins {
  `cpp-library`
}

group = "engine"

library {
  configureCppProject(project)
}

unitTest {
  configureCppProject(project)

  dependencies {
    implementation(project(":deps:googletest"))
  }
}
