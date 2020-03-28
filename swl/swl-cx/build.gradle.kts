import net.rantingmong.snowowl.configureCppProject

plugins {
  `cpp-library`
}

group = "engine"

library {
  configureCppProject(project)

  linkage.set(listOf(Linkage.STATIC))
}

unitTest {
  configureCppProject(project)

  dependencies {
    implementation(project(":deps:googletest"))
  }
}
