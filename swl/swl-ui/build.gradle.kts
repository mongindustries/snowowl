import net.rantingmong.snowowl.configureCppProject

plugins {
  `cpp-library`
}

group = "engine"

library {
  configureCppProject(project)
}
