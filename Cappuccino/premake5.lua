
local solutionDir = "build/"
workspace "Cappuccino - Built"
   defines{"_CRT_SECURE_NO_WARNINGS"}
   configurations { "Debug", "Release" }
   platforms {"x64"}
   location (solutionDir)
   language "C++"
   cppdialect "C++17"
   files{
      "src/**.cpp",
      "src/**.c",
      "include/Cappuccino/**.h",
      "include/Cappuccino/**.hpp",
      "include/Cappuccino/**.cs",
      "Assets/Mesh/**.obj",
      "Assets/Textures/**.png",
      "Assets/Shaders/**.vert",
      "Assets/Shaders/**.frag"
   }
   includedirs{"include/"}

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
      config = "\\Release\\"
      os.execute("ECHO Building Project...")
      os.execute("mkdir " .. "build\\build\\"..arch..config)
      os.execute("XCOPY " .. "libs\\x64\\Debug\\Fmod\\fmod.dll " .."build\\build\\"..arch..config)
      config = "\\Debug\\"
      os.execute("mkdir " .. "build\\build\\"..arch..config)
      os.execute("XCOPY " .. "libs\\x64\\Debug\\Fmod\\fmod.dll " .."build\\build\\"..arch..config)
      
      os.execute("Pause");

project "Cappuccino Engine"
    kind "StaticLib"
    targetname{"Capp"}
    targetextension {".lib"}
    configuration "windows"
        links { "opengl32", "glfw3.lib","fmod_vc.lib"}