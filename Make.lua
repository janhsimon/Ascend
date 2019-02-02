path_to_qt = "C:/Qt/5.11.1/msvc2017_64"
path_to_sdl = "C:/SDL2-2.0.9"
path_to_vulkan = os.getenv("VULKAN_SDK")


require "Lib/premake-qt/qt"

workspace "Ascend"
	architecture "x64"
	configurations { "Debug", "Release" }
	startproject "SDL Sandbox"

project "Engine"
	location "Engine"
	kind "SharedLib"
	language "C++"
	files { "%{prj.name}/Source/**.cpp", "%{prj.name}/Source/**.hpp" }
	objdir "%{prj.name}/Temp"
	targetdir "%{prj.name}/Bin"
	targetname "Ascend"

	includedirs { "Engine/Source", (path_to_vulkan .. "/Include") } -- TODO: Engine/Source does not work

	libdirs { (path_to_vulkan .. "/Lib") }
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

project "SDL Sandbox"
	location "SDL Sandbox"
	kind "WindowedApp"
	language "C++"
	files { "%{prj.name}/Source/**.cpp", "%{prj.name}/Source/**.hpp" }
	objdir "%{prj.name}/Temp"
	targetdir "%{prj.name}/Bin"
	debugdir "SDL Sandbox/Bin" -- %{prj.name} does not work correctly with debugdir, results in "SDL Sandbox/SDL Sandbox/Bin"

	includedirs { "Engine/Include", (path_to_sdl .. "/include"), (path_to_vulkan .. "/Include") }

	libdirs { (path_to_sdl .. "/lib/x64"), (path_to_vulkan .. "/Lib") }
	links { "Engine", "SDL2", "SDL2main", "vulkan-1" }

	postbuildcommands
	{
		"{COPY} ../Engine/Bin/Ascend.dll Bin/",
		("{COPY} " .. path_to_sdl .. "/lib/x64/SDL2.dll Bin/")
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
		
project "Qt Sandbox"
	location "Qt Sandbox"
	kind "ConsoleApp" --"WindowedApp"
	language "C++"
	files { "%{prj.name}/Source/**.cpp", "%{prj.name}/Source/**.h", "%{prj.name}/Source/**.qrc" }
	--objdir "%{prj.name}/Temp"
	targetdir "%{prj.name}/Bin"
	debugdir "Qt Sandbox/Bin"

	includedirs { "Engine/Include", (path_to_vulkan .. "/Include") }

	libdirs { (path_to_vulkan .. "/Lib") }
	links { "Engine", "vulkan-1" } -- qtmaind.lib should allow us to make a WindowedApp but causes linker errors

	postbuildcommands
	{
		"{COPY} ../Engine/Bin/Ascend.dll Bin/",
		("{COPY} " .. path_to_qt .. "/bin/Qt5Core.dll Bin/"),
		("{COPY} " .. path_to_qt .. "/bin/Qt5Gui.dll Bin/"),
		("{COPY} " .. path_to_qt .. "/bin/Qt5Widgets.dll Bin/")
	}

  premake.extensions.qt.enable()
  qtpath(path_to_qt)
  qtmodules { "core", "gui", "widgets" }
  qtprefix "Qt5"
  
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