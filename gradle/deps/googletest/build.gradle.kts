plugins {
  `cpp-library`
}

library {

  val rootDir = "lib/googletest"

  source {
    from.add("$rootDir/src/gtest-all.cc")
  }

  publicHeaders {
    from.add("$rootDir/include")
  }

  privateHeaders {
    from.add(rootDir)
  }

  linkage.set(listOf(Linkage.STATIC))

  binaries.whenElementFinalized {
    compileTask.get().compilerArgs.add("-std=c++17")
  }
}
