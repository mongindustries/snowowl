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

  targetMachines.set(listOf(machines.macOS.x86_64, machines.windows.x86_64))

  if (vulkanSDK != null) {
    logger.lifecycle("Vulkan SDK found: $vulkanSDK")
    privateHeaders {
      val vulkanIncl = java.nio.file.Paths.get(vulkanSDK, "include")

      logger.lifecycle("Including $vulkanIncl to include paths...")

      from.add(vulkanIncl)
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
