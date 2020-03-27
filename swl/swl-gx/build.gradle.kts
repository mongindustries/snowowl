import net.rantingmong.snowowl.Utilities.*

plugins {
  `cpp-library`
}

group = "engine"

library {
  Utilities.asCppProject(project, this)

  dependencies {
    implementation(project(":swl-cx"))
  }
}
