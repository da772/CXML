workspace "CXML"
    architecture "x86_64"

    
	configurations
	{
		"Debug",
		"Release",
	}

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    project "CXML_Tests"
        kind "ConsoleApp"
        language "C++"
            cppdialect "C++17"
        staticruntime "on"

        targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
        objdir ("%{prj.location}/obj/" .. outputdir .. "/%{prj.name}")

        defines 
        {
            "GE_PROJECT_WizardJump"
        }
        
        files 
        {
            "tests/**.cpp",
            "tests/**.hpp",
            "src/**.cpp"
        }

        includedirs 
        {
            "include"
        }

        filter "configurations:Debug"
				runtime "Debug"
				symbols "On"
			filter "configurations:Release"
				runtime "Release"
				optimize "On"