os.execute("echo Building solution...")
workspace "Cappuccino Engine"
	defines "_CRT_SECURE_NO_WARNINGS"
	configurations { "Debug", "Release" }
	
	platforms "x64"
	
	filter "platforms:x64"
		system "Windows"
		architecture "x64"
		defines "Win64"
		

include "Externals"

os.execute("echo Building Cappuccino Engine...")
project "CappuccinoEngine"
	location "Cappuccino/"
	
	kind "StaticLib"
	targetname "capp"
	targetextension ".lib"
	
	targetdir "Build/bin/%{cfg.architecture}/%{cfg.buildcfg}"
	objdir "Build/obj/%{cfg.architecture}/%{cfg.buildcfg}"
	
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	files {
		"Cappuccino/include/**.h",
		"Cappuccino/include/**.hpp",
		"Cappuccino/include/**.c",
		"Cappuccino/include/**.cpp",
		"Cappuccino/include/**.cs",
		
		"Cappuccino/src/**.h",
		"Cappuccino/src/**.hpp",
		"Cappuccino/src/**.cpp",
		
		"Cappuccino/Assets/Mesh/**.obj",
		
		"Cappuccino/Assets/Shaders/**.vert",
		"Cappuccino/Assets/Shaders/**.frag",
		"Cappuccino/Assets/Shaders/**.geom",
		
		"Cappuccino/Assets/Textures/**.png",
		"Cappuccino/Assets/Textures/**.jpg"
	}
	
	includedirs {
		"Cappuccino/include",
		
		"Externals/fmod/include",
		"Externals/freetype/include",
		"Externals/glad/include",
		"Externals/glfw3/include",
		"Externals/glm/include",
		"Externals/imgui/include",
		"Externals/stb/include",
		"Externals/json/include",
		"Externals/tiny/include"
	}
	
	libdirs {
		"Externals/fmod",
		"Externals/freetype"
	}
	
	links {
		"Externals",
		
		"imagehlp",
		"opengl32",
		"fmod_vc",
		"freetype"
	}
	
	filter "system:windows"
		systemversion "latest"
		
		defines {
			"GLFW_INCLUDE_NONE",
			"WINDOWS"
		}		
		
	filter "configurations:Debug"
		defines { "DEBUG" }
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines { "NDEBUG" }
		runtime "Release"
		optimize "on"