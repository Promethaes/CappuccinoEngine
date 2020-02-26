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

print(string.format("Creating project \"%s\" in directory \"%s\"", projName, solutionDir))

-----------------------------------------------------------------------------------------------------------------------
print("Making directories...")
os.execute("mkdir \"" .. solutionDir .. "\\Assets\"")
os.execute("mkdir \"" .. solutionDir .. "\\include\"")
os.execute("mkdir \"" .. solutionDir .. "\\src\"")

print("Copying necessary files...")
-- Copy engine libraries into libs folder
-- os.execute("robocopy %CappuccinoPath%\\Externals\\Build\\bin "        .. "\"" .. solutionDir .. "\\libs\" *.lib /e /xo /xx /ns /np /ndl /njh /njs")
-- os.execute("robocopy %CappuccinoPath%\\Build\\bin "                   .. "\"" .. solutionDir .. "\\libs\" *.lib /e /xo /xx /ns /np /ndl /njh /njs")

-- Copy external header, inline, and dll files
os.execute("robocopy \"%CappuccinoPath%\\Externals\" "                       .. "\"" .. solutionDir .. "\\Engine\\Externals\" *.dll *.h *.hpp *.inl /s /xo /xx /ns /np /ndl /njh /njs")

-- Copy engine header files
os.execute("robocopy \"%CappuccinoPath%\\Cappuccino\\include\" "             .. "\"" .. solutionDir .. "\\Engine\\Cappuccino\\include\" *.h *.hpp /mir /xo /xx /ns /np /ndl /njh /njs")

-- Copy other files
os.execute("robocopy \"%CappuccinoPath%\\Game Project Generation\" "     .. "\"" .. solutionDir .. "\\include\" reeee.txt /xo /xx /ns /np /ndl /njh /njs")
os.execute("robocopy \"%CappuccinoPath%\\Cappuccino\\src\\Cappuccino\" " .. "\"" .. solutionDir .. "\\src\" main.cpp /xo /xx /ns /np /ndl /njh /njs")

-----------------------------------------------------------------------------------------------------------------------

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
		"\"" .. solutionDir .. "/include\"",
		"\"" .. solutionDir .. "/Engine/Cappuccino/include\"",
		
		"\"" .. solutionDir .. "/Engine/Externals/fmod/include\"",
		"\"" .. solutionDir .. "/Engine/Externals/fmod/studio/include\"",
		"\"" .. solutionDir .. "/Engine/Externals/freetype/include\"",
		"\"" .. solutionDir .. "/Engine/Externals/glad/include\"",
		"\"" .. solutionDir .. "/Engine/Externals/glfw3/include\"",
		"\"" .. solutionDir .. "/Engine/Externals/glm/include\"",
		"\"" .. solutionDir .. "/Engine/Externals/imgui/include\"",
		"\"" .. solutionDir .. "/Engine/Externals/stb/include\""
	}
	
	libdirs {
		-- Reeee environment variables
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
	
	links {
		"capp",
		"Externals"
	}
	
	files {
		solutionDir .. "\\include/reeee.txt",
		solutionDir .. "\\include/**.h",
		solutionDir .. "\\include/**.hpp",
		solutionDir .. "\\src/**.cpp",
	}
	
	prebuildcommands {
		-- Update header files from engine
		"robocopy \"" .. os.getenv("CappuccinoPath") .. "\\Cappuccino\\include\" \"%{prj.location}\\Engine\\Cappuccino\\include\" *.h *.hpp /mir /xo /xx /ns /np /ndl /njh /njs",
		
		-- Update lib files from engine
		-- "robocopy \"%CappuccinoPath%\\Externals\\Build\\bin\" \"%{prj.location}\\libs\" *.lib /e /xo /xx /ns /np /ndl /njh /njs",
		-- "robocopy \"%CappuccinoPath%\\Build\\bin\" \"%{prj.location}\\libs\" *.lib /e /xo /xx /ns /np /ndl /njh /njs",
		
		-- Update any default assets
		"robocopy \"" .. os.getenv("CappuccinoPath") .. "\\Cappuccino\\Assets\" \"%{prj.location}\\Assets\" /s /xo /xx /ns /np /ndl /njh /njs",
		
		"exit 0"
	}
	
	postbuildcommands {
		-- Copy all dll files next to executable
		"robocopy \"%{prj.location}\\Engine\\Externals\\dll\" \"%{cfg.targetdir}\" /e /xo /xx /ns /np /ndl /njh /njs",
		
		-- Copy assets directory next to executable
		"robocopy \"%{prj.location}\\Assets\" \"%{cfg.targetdir}\\Assets\" /mir /xo /xx /ns /np /ndl /njh /njs",
		
		"exit 0"
	}