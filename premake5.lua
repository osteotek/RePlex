workspace "replex"
    configurations {"Debug", "Release"}
    -- Use C++ as the target language for all builds
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"

    flags {"C++14"}

    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"

    filter "configurations:Release"
        defines {"RELEASE"}
        optimize "On"

    project "GoogleTest"
        kind "StaticLib"
        files {"googletest/googletest/src/gtest-all.cc"}
        includedirs {"googletest/googletest/include", "googletest/googletest"}
        
    project "replexruntime"
        kind "ConsoleApp"
        files {"runtime/**.h", "runtime/**.cpp"}
        includedirs {"lib/pub", "test/pub", "googletest/googletest/include"}
        links {"GoogleTest"}
    
    project "replextest"
        kind "SharedLib"
        files {"test/**.h", "test/**.cpp", "test/pub/*.h"}
        includedirs {"lib/pub"}