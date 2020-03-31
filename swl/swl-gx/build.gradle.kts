import net.rantingmong.snowowl.configureCppProject

plugins {
  `cpp-library`
}

group = "engine"

val prop = java.util.Properties()

val localPropFile = project.rootProject.file("local.properties")
if (localPropFile.exists()) {
  prop.load(localPropFile.inputStream())
}

val vulkanSDK = prop.getProperty("vulkan.sdk")

library {

  if (vulkanSDK != null) {
    privateHeaders {
      from.add(java.nio.file.Paths.get(vulkanSDK, "include"))
    }
  }

  configureCppProject(project)

  dependencies {
    implementation(project(":swl-cx"))
    implementation(project(":swl-ui"))
  }
}

unitTest {

  if (vulkanSDK != null) {
    privateHeaders {
      from.add(java.nio.file.Paths.get(vulkanSDK, "include"))
    }
  }

  configureCppProject(project)

  dependencies {
    implementation(project(":deps:googletest"))
  }
}
