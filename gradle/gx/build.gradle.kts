import net.rantingmong.snowowl.configureCppProject
import java.nio.file.Paths

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

inline fun <reified Library : CppComponent>targetVulkan(component: Library, sdkPath: String) {

  val includePath = Paths.get(sdkPath, "include")
  val libPath = Paths.get(sdkPath, "lib")

  component.privateHeaders {
    logger.lifecycle("Including $includePath to include paths...")
    from.add(includePath)
  }

  component.binaries.configureEach(CppTestExecutable::class.java) {
    linkTask.get().linkerArgs.add("-L$libPath")
  }
}

library {

  targetMachines.set(listOf(machines.macOS.x86_64, machines.windows.x86_64))

  if (vulkanSDK != null) {
    logger.lifecycle("Vulkan SDK found: $vulkanSDK")
    targetVulkan(this, vulkanSDK)
  }

  configureCppProject(project)

  dependencies {
    implementation(project(":swl-ui"))
    implementation(project(":swl-cx"))
  }
}

unitTest {

  if (vulkanSDK != null) {
    logger.lifecycle("Vulkan SDK found: $vulkanSDK")
    targetVulkan(this, vulkanSDK)
  }

  configureCppProject(project)

  dependencies {
    implementation(project(":swl-cx"))
    implementation(project(":swl-ui"))

    implementation(project(":deps:googletest"))
  }
}
