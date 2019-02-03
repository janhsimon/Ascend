project "Engine"
	kind "SharedLib"
	language "C++"
	files { "Source/**.*" }
	objdir "Temp"
	targetdir "Bin"
	targetname "Ascend"

	includedirs { (vulkan .. "/Include") }

	libdirs { (vulkan .. "/Lib") }
	links { "vulkan-1" }

	defines { "ASC_BUILD_DLL" }

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