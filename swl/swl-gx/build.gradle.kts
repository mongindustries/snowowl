import net.rantingmong.snowowl.configureCppProject

plugins {
  `cpp-library`
}

group = "engine"

library {
  configureCppProject(project)

  linkage.set(listOf(Linkage.STATIC))

  dependencies {
    implementation(project(":swl-cx"))
  }
}

unitTest {
  configureCppProject(project)

  dependencies {
    implementation(project(":deps:googletest"))
  }
}
