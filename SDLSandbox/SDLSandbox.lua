project "SDLSandbox"
	kind "WindowedApp"
	language "C++"
	files { "Source/**.*", }
	objdir "Temp"
	targetdir "Bin"
	debugdir "Bin"

	includedirs { "../Engine/Include", (sdl .. "/include"), (vulkan .. "/Include") }

	libdirs { (sdl .. "/lib/x64"), (vulkan .. "/Lib") }
	links { "Engine", "SDL2", "SDL2main", "vulkan-1" }

	postbuildcommands
	{
		"{COPY} ../Engine/Bin/Ascend.dll Bin/",
		("{COPY} " .. sdl .. "/lib/x64/SDL2.dll Bin/")
	}

	filter "system:windows"
		cppdialect "c++17"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"

	filter "configurations:Release"
		removedefines "DEBUG"
		optimize "On"

	filter { "files:Source/Shaders/*", "configurations:Debug" }
		buildcommands (vulkan .. "/bin/glslc.exe %{file.relpath} -o Bin/Shaders/%{file.name}.spv -O0")
		buildoutputs "Bin/Shaders/%{file.name}.spv"

	filter { "files:Source/Shaders/*", "configurations:Release" }
		buildcommands (vulkan .. "/bin/glslc.exe %{file.relpath} -o Bin/Shaders/%{file.name}.spv -Os")
		buildoutputs "Bin/Shaders/%{file.name}.spv"