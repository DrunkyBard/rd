import com.jetbrains.rd.gradle.dependencies.kotlinVersion
import com.jetbrains.rd.gradle.plugins.applyKotlinJVM
import com.jetbrains.rd.gradle.tasks.creatingCopySourcesTask
import org.gradle.jvm.tasks.Jar

applyKotlinJVM()

plugins {
    kotlin("jvm")
}

dependencies {
    implementation(project(":rd-core:"))
    implementation(gradleApi())
    testImplementation(project(":rd-framework"))
    testImplementation("org.jetbrains:annotations:20.1.0")
    implementation("org.jetbrains.kotlin:kotlin-compiler-embeddable:${kotlinVersion}")
    implementation("org.jetbrains.kotlin:kotlin-script-runtime:${kotlinVersion}")
    implementation("org.jetbrains.intellij.deps:trove4j:1.0.20181211")
    testImplementation("org.jetbrains.kotlin:kotlin-test-junit5:${com.jetbrains.rd.gradle.dependencies.kotlinVersion}")
}

apply(from = "models.gradle.kts")

lateinit var models: SourceSet

sourceSets {
    main {
        compileClasspath = compileClasspath.minus(files(gradle.gradleHomeDir?.resolve("lib")?.listFiles()?.filter { it.name.contains("kotlin-stdlib") || it.name.contains("kotlin-reflect") } ?: listOf<File>()))
    }

    test {
        compileClasspath = compileClasspath.minus(files(gradle.gradleHomeDir?.resolve("lib")?.listFiles()?.filter { it.name.contains("kotlin-stdlib") || it.name.contains("kotlin-reflect") } ?: listOf<File>()))
        runtimeClasspath = runtimeClasspath.minus(files(gradle.gradleHomeDir?.resolve("lib")?.listFiles()?.filter { it.name.contains("kotlin-stdlib") || it.name.contains("kotlin-reflect") } ?: listOf<File>()))
    }

    models = create("models") {
        kotlin {
            compileClasspath += main.get().output
            compileClasspath = compileClasspath.minus(files(gradle.gradleHomeDir?.resolve("lib")?.listFiles()?.filter { it.name.contains("kotlin-stdlib") || it.name.contains("kotlin-reflect") } ?: listOf<File>()))

            listOf("interning", "demo", "sync", "openEntity").map {
                rootProject.buildDir.resolve("models").resolve(it)
            }.forEach {
                output.dir(it)
            }

            compiledBy("generateEverything")
        }
    }

    create("gradlePlugin") {
        compileClasspath += sourceSets["main"].compileClasspath
    }
}

val testCopySources by creatingCopySourcesTask(kotlin.sourceSets.test, models)

tasks.named("compileTestKotlin") {
    dependsOn(testCopySources)
}

tasks {
    jar {
        from(sourceSets["gradlePlugin"].output)
    }
}

val modelsImplementation: Configuration by configurations.getting {
    extendsFrom(configurations.implementation.get())
}

publishing.publications.named<MavenPublication>("pluginMaven") {
    pom {
        name.set("rd-gen")
        description.set("Code generator for the RD protocol.")
    }
}
