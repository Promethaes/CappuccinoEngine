os.execute("echo Enter project directory:")
local solutionDir = io.read()
if(string.find(solutionDir, "\"", 1)) then
	solutionDir = solutionDir:sub(2)
end
if(string.find(solutionDir, "\"", -1)) then
	solutionDir = solutionDir:sub(1, -2)
end

os.execute("echo.")
os.execute("echo Enter project name:")
local projName = io.read()
os.execute("echo.")

print(string.format("Creating project \"%s\" in directory %s", projName, solutionDir))

print("Making directories...")
os.execute("mkdir " .. solutionDir .. "\\Build")
os.execute("mkdir " .. solutionDir .. "\\Build\\bin")
os.execute("mkdir " .. solutionDir .. "\\Build\\bin\\x64")
os.execute("mkdir " .. solutionDir .. "\\Build\\bin\\x64\\Debug")
os.execute("mkdir " .. solutionDir .. "\\Build\\bin\\x64\\Release")
os.execute("mkdir " .. solutionDir .. "\\Assets")
os.execute("mkdir " .. solutionDir .. "\\include")
os.execute("mkdir " .. solutionDir .. "\\src")

print("Copying necessary files...")
os.execute("xcopy %CappuccinoPath%\\Externals\\dll\\fmod.dll " .. solutionDir .. "\\Build\\bin\\x64\\Debug")
os.execute("xcopy %CappuccinoPath%\\Externals\\dll\\fmod.dll " .. solutionDir .. "\\Build\\bin\\x64\\Release")
os.execute("xcopy %CappuccinoPath%\\Externals\\dll\\freetype.dll " .. solutionDir .. "\\Build\\bin\\x64\\Debug")
os.execute("xcopy %CappuccinoPath%\\Externals\\dll\\freetype.dll " .. solutionDir .. "\\Build\\bin\\x64\\Release")

os.execute("xcopy %CappuccinoPath%\\Cappuccino\\src\\Cappuccino\\main.cpp " .. solutionDir .. "\\src")

os.execute("echo.")
print("Starting premake build...")
print(string.format("Building solution %s...", projName))

workspace (projName)
	location(solutionDir)
	
	defines "_CRT_SECURE_NO_WARNINGS"
	configurations { "Debug", "Release" }
		
	platforms "x64"
	
	filter "platforms:x64"
		system "Windows"
		architecture "x64"
		defines "Win64"

	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir (solutionDir .. "/Build/bin/%{cfg.platform}/%{cfg.buildcfg}")
	debugdir (solutionDir .. "/Build/bin/%{cfg.platform}/%{cfg.buildcfg}")
	objdir (solutionDir .. "/Build/obj/%{cfg.platform}/%{cfg.buildcfg}")
	

	includedirs {
		os.getenv("CappuccinoPath") .. "/Cappuccino/include",
		os.getenv("CappuccinoPath") .. "/Externals/fmod/include",
		os.getenv("CappuccinoPath") .. "/Externals/freetype/include",
		os.getenv("CappuccinoPath") .. "/Externals/glad/include",
		os.getenv("CappuccinoPath") .. "/Externals/glfw3/include",
		os.getenv("CappuccinoPath") .. "/Externals/glm/include",
		os.getenv("CappuccinoPath") .. "/Externals/imgui/include",
		os.getenv("CappuccinoPath") .. "/Externals/stb/include"
	}
	
	libdirs {
		os.getenv("CappuccinoPath") .. "/Build/bin/%{cfg.architecture}/%{cfg.buildcfg}",
		os.getenv("CappuccinoPath") .. "/Externals/Build/bin/%{cfg.architecture}/%{cfg.buildcfg}"
	}
	
	filter "platforms:x64"
		system "windows"
		architecture "x64"
		defines "Win64"
	
	filter "system:windows"
		systemversion "latest"
		
		defines {
			"GLFW_INCLUDE_NONE",
			"WINDOWS"
		}
		
	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "NDEBUG"
		runtime "Release"
		optimize "on"

print(string.format("Building project %s...", projName)) 
project (projName)
	kind "ConsoleApp"
	configuration "windows"
	targetname (projName)
	
	debugdir (solutionDir .. "/Build/bin/%{cfg.platform}/%{cfg.buildcfg}")
	
	links {
		"capp",
		"Externals"
	}
	
	files {
		solutionDir .. "/include/**.h",
		solutionDir .. "/include/**.hpp",
		solutionDir .. "/src/**.cpp",
		
		solutionDir .. "/Assets/**"
	}