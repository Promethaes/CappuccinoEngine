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
os.execute("mkdir " .. solutionDir .. "\\Engine\\Cappuccino")
os.execute("mkdir " .. solutionDir .. "\\Engine\\Externals")
os.execute("mkdir " .. solutionDir .. "\\src")
os.execute("mkdir " .. solutionDir .. "\\libs\\Debug")
os.execute("mkdir " .. solutionDir .. "\\libs\\Release")

print("Copying necessary files...")
os.execute("xcopy %CappuccinoPath%\\Externals\\dll\\fmod.dll " .. solutionDir .. "\\Build\\bin\\x64\\Debug")
os.execute("xcopy %CappuccinoPath%\\Externals\\dll\\fmod.dll " .. solutionDir .. "\\Build\\bin\\x64\\Release")
os.execute("xcopy %CappuccinoPath%\\Externals\\dll\\fmodstudio.dll " .. solutionDir .. "\\Build\\bin\\x64\\Debug")
os.execute("xcopy %CappuccinoPath%\\Externals\\dll\\fmodstudio.dll " .. solutionDir .. "\\Build\\bin\\x64\\Release")
os.execute("xcopy %CappuccinoPath%\\Externals\\dll\\freetype.dll " .. solutionDir .. "\\Build\\bin\\x64\\Debug")
os.execute("xcopy %CappuccinoPath%\\Externals\\dll\\freetype.dll " .. solutionDir .. "\\Build\\bin\\x64\\Release")
os.execute("xcopy %CappuccinoPath%\\Build\\bin\\x86_64\\Debug\\capp.lib " .. solutionDir .. "\\libs\\Debug")
os.execute("xcopy %CappuccinoPath%\\Build\\bin\\x86_64\\Release\\capp.lib " .. solutionDir .. "\\libs\\Release")
os.execute("xcopy %CappuccinoPath%\\Externals\\Build\\bin\\x86_64\\Debug\\Externals.lib " .. solutionDir .. "\\libs\\Debug")
os.execute("xcopy %CappuccinoPath%\\Externals\\Build\\bin\\x86_64\\Release\\Externals.lib " .. solutionDir .. "\\libs\\Release")


os.execute("xcopy /S %CappuccinoPath%\\Cappuccino\\include\\Cappuccino " .. solutionDir .. 			  "\\Engine\\Cappuccino")
os.execute("xcopy /S %CappuccinoPath%\\\"Game Project Generation\"\\Externals " .. solutionDir ..     "\\Engine\\Externals")
os.execute("xcopy /S %CappuccinoPath%\\Cappuccino\\src\\Cappuccino\\main.cpp " .. solutionDir .. "\\src")

-- This is to make sure the filters are created in Visual Studio even if there are no files in the folders
os.execute("xcopy reeee.txt " .. solutionDir .. "\\include\\")

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
	objdir (solutionDir .. "/Build/obj/%{cfg.platform}/%{cfg.buildcfg}")
	

	includedirs {
		solutionDir .. "/include",
	
		solutionDir.."/Engine",
		solutionDir.."/Engine/Externals/fmod/include",
		solutionDir.."/Engine/Externals/fmod/studio/include"
		solutionDir.."/Engine/Externals/freetype/include",
		solutionDir.."/Engine/Externals/glad/include",
		solutionDir.."/Engine/Externals/glfw3/include",
		solutionDir.."/Engine/Externals/glm/include",
		solutionDir.."/Engine/Externals/imgui/include",
		solutionDir.."/Engine/Externals/stb/include"
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
		libdirs {
			solutionDir.."\\libs\\Debug"
		}

	filter "configurations:Release"
		defines "NDEBUG"
		runtime "Release"
		optimize "on"
		libdirs {
			solutionDir.."\\libs\\Release"
		}

print(string.format("Building project %s...", projName)) 
project (projName)
	kind "ConsoleApp"
	configuration "windows"
	targetname (projName)
	
	links {
		"capp",
		"Externals"
	}
	
	files {
		solutionDir .. "/include/reeee.txt",
		solutionDir .. "/include/**.h",
		solutionDir .. "/src/**.cpp",
	}
	
os.execute("del /s /q %{solutionDir}\\reeee.txt")