import net.rantingmong.snowowl.configureCppProject

plugins {
  `cpp-application`
}

application {
  configureCppProject(project)

  dependencies {
    implementation(project(":swl-cx"))
    implementation(project(":swl-gx"))
  }
}

unitTest {
  configureCppProject(project)
}
