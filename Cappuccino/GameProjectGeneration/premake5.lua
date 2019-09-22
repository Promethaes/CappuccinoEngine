os.execute("ECHO Enter Project Directory")
local solutionDir = io.read()
workspace "Cappuccino Game"
   defines{"_CRT_SECURE_NO_WARNINGS"}
   configurations { "Debug", "Release" }
   platforms {"x64"}
   location (solutionDir)
   language "C++"
   cppdialect "C++17"
   includedirs{os.getenv("CappuccinoPath").."include/"}
   os.execute("mkdir " ..solutionDir.."\\build")
   os.execute("mkdir " ..solutionDir.."\\build\\x64")
   os.execute("mkdir " ..solutionDir.."\\build\\x64\\Debug")
   os.execute("mkdir " ..solutionDir.."\\build\\x64\\Release")

local arch = " "
   filter { "platforms:x64" }
      system "Windows"
      architecture "x64"
      defines {"Win64"}
      arch = "x64"

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      targetdir (solutionDir.."\\build\\" .. arch .. "\\Debug")
      libdirs{os.getenv("CappuccinoPath").."libs/" .. arch .. "/Debug",os.getenv("CappuccinoPath").."libs/" .. arch .. "/Debug".."/Fmod"}

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      targetdir (solutionDir.."\\build\\" .. arch .. "\\Release")
      libdirs{os.getenv("CappuccinoPath").."libs/" .. arch .. "/Release",os.getenv("CappuccinoPath").."libs/" .. arch .. "/Release".."/Fmod"}


   filter {}
      os.execute("ECHO Enter Project Name")
      local s = io.read();
      os.execute("ECHO Creating Project...")
      os.execute("mkdir " ..solutionDir.."\\src")
      os.execute("XCOPY %CappuccinoPath%\\src\\Cappuccino\\main.cpp " .. solutionDir .. "\\src")
      os.execute("XCOPY %CappuccinoPath%\\libs\\x64\\Release\\Fmod\\fmod.dll " .. solutionDir .. "\\build\\x64\\Debug")
      os.execute("XCOPY %CappuccinoPath%\\libs\\x64\\Release\\Fmod\\fmod.dll " .. solutionDir .. "\\build\\x64\\Release")
      files{solutionDir.."\\src\\**.cpp"}
      
project (s)
    kind "ConsoleApp"
    configuration "windows"
        targetname (s)
        links {"capp.lib"}
    os.execute("ECHO Created Successfully");
    