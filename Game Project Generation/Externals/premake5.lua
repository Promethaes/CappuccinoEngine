os.execute("echo Building external dependencies...")

project "Externals"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	
	targetdir "Build/bin/%{cfg.architecture}/%{cfg.buildcfg}"
	objdir "Build/obj/%{cfg.architecture}/%{cfg.buildcfg}"

    staticruntime "on"
	
	files {
		"glad/include/glad/glad.h",
        "glad/include/KHR/khrplatform.h",
        "glad/src/glad.c",
		
		"imgui/include/imgui/imconfig.h",
        "imgui/include/imgui/imgui.h",
        "imgui/include/imgui/imgui_impl_glfw.h",
        "imgui/include/imgui/imgui_impl_opengl3.h",
        "imgui/include/imgui/imgui_internal.h",
        "imgui/include/imgui/imstb_rectpack.h",
        "imgui/include/imgui/imstb_textedit.h",
        "imgui/include/imgui/imstb_truetype.h",
        "imgui/src/imgui.cpp",
        "imgui/src/imgui_demo.cpp",
        "imgui/src/imgui_draw.cpp",
        "imgui/src/imgui_impl_glfw.cpp",
        "imgui/src/imgui_impl_opengl3.cpp",
        "imgui/src/imgui_widgets.cpp",
		
		"stb/include/stb/stb_image.h",
        "stb/include/stb/stb_image_write.h",
        "stb/include/stb/stb_include.h",
        "stb/include/stb/stb_perlin.h",
        "stb/include/stb/stb_rect_pack.h",
        "stb/include/stb/stb_truetype.h",
        "stb/src/stb_impl.cpp",

        "glfw3/include/GLFW/glfw3.h",
        "glfw3/include/GLFW/glfw3native.h",
        "glfw3/src/glfw_config.h",
        "glfw3/src/context.c",
        "glfw3/src/init.c",
        "glfw3/src/input.c",
        "glfw3/src/monitor.c",
        "glfw3/src/vulkan.c",
        "glfw3/src/window.c",

        "json/include/json/json.hpp",

        "tiny/include/tiny_gltf.h",
        "tiny/src/tiny_impl.cpp"

	}
	
	includedirs {
		"glad/include",
		"glfw3/include",
		"imgui/include/imgui",
        "stb/include/stb",
        "tiny/include/",
        "json/include/"
	}
	
	links {
		"opengl32"
	}
	
	filter "system:windows"		
        systemversion "latest"
        
        files {
            "glfw3/src/win32_init.c",
            "glfw3/src/win32_joystick.c",
            "glfw3/src/win32_monitor.c",
            "glfw3/src/win32_time.c",
            "glfw3/src/win32_thread.c",
            "glfw3/src/win32_window.c",
            "glfw3/src/wgl_context.c",
            "glfw3/src/egl_context.c",
            "glfw3/src/osmesa_context.c"
        }

		defines { 
            "_GLFW_WIN32"
		}
		
	filter "configurations:Debug"
		defines "DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
		defines "NDEBUG"
        runtime "Release"
        optimize "on"
		
		