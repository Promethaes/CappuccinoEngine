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
      targetdir ("build/build/" .. arch .. "/Debug")
      config = "/Debug"
      libdirs{os.getenv("CappuccinoPath").."libs/" .. arch .. "/Debug",os.getenv("CappuccinoPath").."libs/" .. arch .. "/Debug".."/Fmod"}

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      targetdir ("build/build/" .. arch .. "/Release")
      config = "/Release"
      libdirs{os.getenv("CappuccinoPath").."libs/" .. arch .. "/Release",os.getenv("CappuccinoPath").."libs/" .. arch .. "/Release".."/Fmod"}


   filter {}
      
      -- config = "\\Release\\"
      -- os.execute("mkdir " .. "build\\build\\"..arch..config)
      -- os.execute("XCOPY " .. "libs\\x64\\Debug\\Fmod\\fmod.dll " .."build\\build\\"..arch..config)
      -- config = "\\Debug\\"
      -- os.execute("mkdir " .. "build\\build\\"..arch..config)
      -- os.execute("XCOPY " .. "libs\\x64\\Debug\\Fmod\\fmod.dll " .."build\\build\\"..arch..config)
	  os.execute("ECHO.");      
	  os.execute("ECHO Enter Project Name")
      local s = io.read();
	  os.execute("ECHO.");
      os.execute("ECHO Creating Project...")
      os.execute("mkdir " ..solutionDir.."\\src")
      os.execute("XCOPY %CappuccinoPath%\\src\\Cappuccino\\main.cpp " .. solutionDir .. "\\src")
      files{solutionDir.."\\src\\**.cpp"}
      
project (s)
    kind "ConsoleApp"
    configuration "windows"
        targetname (s)
        links {"capp.lib"}
    os.execute("ECHO Created Successfully");
    os.execute("Pause");
