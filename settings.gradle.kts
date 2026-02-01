pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
    }
}

rootProject.name = "MinniFramework"

include(":minni-core")
project(":minni-core").projectDir = file("src/android")

include(":android-demo")
project(":android-demo").projectDir = file("examples/android_demo")
