local solutionDir = "build/"
workspace "Cappuccino - Built"
   defines{"_CRT_SECURE_NO_WARNINGS"}
   configurations { "Debug", "Release" }
   platforms {"x64"}
   location (solutionDir)
   language "C++"
   cppdialect "C++17"
   files{
	  "src/**.h",
      "src/**.cpp",
      "src/**.c",
	  
      "include/Cappuccino/**.h",
      "include/Cappuccino/**.hpp",
      "include/Cappuccino/**.cs",
	  
	  "include/imgui/**.h",
     "include/imgui/**.cpp",
     
     --"include/ft2build.h"
    -- "include/freetype/**.h"
	  
      "Assets/Mesh/**.obj",
      "Assets/Textures/**.png",
      "Assets/Shaders/**.vert",
      "Assets/Shaders/**.frag"
   }
   includedirs{"include/","imgui/"}

local arch = " "
local config = " "
   filter { "platforms:x64" }
      system "Windows"
      architecture "x64"
      defines {"Win64"}
      arch = "x64"

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      targetdir ("libs/" .. arch .. "/Debug")
      config = "/Debug"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      targetdir ("libs/" .. arch .. "/Release")
      config = "/Release"

   filter {}
      libdirs{"libs/" .. arch .. config,"libs/" .. arch .. config.."/Fmod"}
      os.execute("ECHO Building Project...")
     
      os.execute("Pause");

project "Cappuccino Engine"
    kind "StaticLib"
    configuration "windows"
        targetextension ".lib"
        targetname "capp"
        links { "opengl32", "glfw3.lib","fmod_vc.lib","freetype.lib"}