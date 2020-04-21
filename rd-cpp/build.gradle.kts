@file:Suppress("UNUSED_VARIABLE", "LocalVariableName")

import com.jetbrains.rd.gradle.tasks.CppBuildTask
import com.jetbrains.rd.gradle.tasks.CrossTestTaskCpp

tasks {
    val build by creating(CppBuildTask::class) {
        execPath("build.cmd")
    }

    val buildTests by creating(CppBuildTask::class) {
        dependsOn(":rd-gen:generateEverything")
        execPath("buildtest.cmd")
    }

    fun creatingCrossTestCppTask() = creating(CrossTestTaskCpp::class) {
        dependsOn(buildTests)
    }

    val CrossTest_AllEntities_CppClient by creatingCrossTestCppTask()

    val CrossTest_AllEntities_CppServer by creatingCrossTestCppTask()

    val CrossTest_BigBuffer_CppClient by creatingCrossTestCppTask()

    val CrossTest_RdCall_CppClient by creatingCrossTestCppTask()
}
