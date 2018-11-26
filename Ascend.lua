vulkansdk = os.getenv("VULKAN_SDK")

workspace "Ascend"
	architecture "x64"
	configurations { "Debug", "Release" }
	startproject "Sandbox"

project "Engine"
	location "Engine"
	kind "SharedLib"
	language "C++"
	files { "%{prj.name}/Source/**.cpp", "%{prj.name}/Source/**.hpp" }
	objdir "%{prj.name}/Temp"
	targetdir "%{prj.name}"
	targetname "Ascend"

	includedirs { (vulkansdk .. "/Include") }

	libdirs { (vulkansdk .. "/Lib") }
	links { "vulkan-1" }

	filter "system:windows"
		cppdialect "c++17"
		staticruntime "On"
		systemversion "latest"
		defines { "ASC_BUILD_DLL" }

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"
 
	filter "configurations:Release"
		removedefines "DEBUG"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "WindowedApp"
	language "C++"
	files { "%{prj.name}/Source/**.cpp", "%{prj.name}/Source/**.hpp" }
	objdir "%{prj.name}/Temp"
	targetdir "%{prj.name}"

	includedirs { "Engine/Include", "%{prj.name}/lib/SDL2-2.0.9/include", (vulkansdk .. "/Include") }

	libdirs { "%{prj.name}/Lib/SDL2-2.0.9/lib/x64", (vulkansdk .. "/Lib") }
	links { "Engine", "SDL2", "SDL2main", "vulkan-1" }

	postbuildcommands { "{COPY} Lib/SDL2-2.0.9/lib/x64/SDL2.dll", "{COPY} ../Engine/Ascend.dll" }

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