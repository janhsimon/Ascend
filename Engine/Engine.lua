project "Engine"
	kind "StaticLib"
	language "C++"
	files { "Engine.lua", "Include/**.*", "Source/**.*" }
	objdir "Temp"
	targetdir "Bin"
	targetname "Ascend"

	includedirs { "%{wks.location}/External", (vulkan .. "/Include") }

	libdirs { (vulkan .. "/Lib") }
	links { "vulkan-1" }

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