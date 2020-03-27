import net.rantingmong.snowowl.Utilities.*

plugins {
  `cpp-application`
}

application {
  Utilities.asCppProject(project, this)

  dependencies {
    implementation(project(":swl-cx"))
  }
}
