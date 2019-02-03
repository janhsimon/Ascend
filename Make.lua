pathToQt = "C:/Qt/5.11.1/msvc2017_64"
pathToSDL = "C:/SDL2-2.0.9"
pathToVulkan = os.getenv("VULKAN_SDK")


require "Lib/premake-qt/qt"

workspace "Ascend"
	architecture "x64"
	configurations { "Debug", "Release" }
	startproject "QtSandbox"

project "Engine"
	location "Engine"
	kind "SharedLib"
	language "C++"
	files { "%{prj.name}/Source/**.cpp", "%{prj.name}/Source/**.hpp" }
	objdir "%{prj.name}/Temp"
	targetdir "%{prj.name}/Bin"
	targetname "Ascend"

	includedirs { "Engine/Source", (pathToVulkan .. "/Include") } -- TODO: Engine/Source does not work

	libdirs { (pathToVulkan .. "/Lib") }
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

project "QtSandbox"
	location "QtSandbox"
	kind "ConsoleApp" --"WindowedApp"
	language "C++"
	files { "%{prj.name}/Source/**.cpp", "%{prj.name}/Source/**.hpp" }
	objdir "%{prj.name}/Temp"
	targetdir "%{prj.name}/Bin"
	debugdir "QtSandbox/Bin"

	includedirs { "Engine/Include", (pathToVulkan .. "/Include") }

	libdirs { (pathToVulkan .. "/Lib") }
	links { "Engine", "vulkan-1" } -- qtmaind.lib should allow us to make a WindowedApp but causes linker errors

	postbuildcommands
	{
		"{COPY} ../Engine/Bin/Ascend.dll Bin/",
		("{COPY} " .. pathToQt .. "/bin/Qt5Core.dll Bin/"),
		("{COPY} " .. pathToQt .. "/bin/Qt5Gui.dll Bin/"),
		("{COPY} " .. pathToQt .. "/bin/Qt5Widgets.dll Bin/")
	}

  premake.extensions.qt.enable()
  qtpath(pathToQt)
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

project "SDLSandbox"
	location "SDLSandbox"
	kind "WindowedApp"
	language "C++"
	files { "%{prj.name}/Source/**.cpp", "%{prj.name}/Source/**.hpp" }
	objdir "%{prj.name}/Temp"
	targetdir "%{prj.name}/Bin"
	debugdir "SDLSandbox/Bin" -- %{prj.name} does not work correctly with debugdir, results in "SDLSandbox/SDLSandbox/Bin"

	includedirs { "Engine/Include", (pathToSDL .. "/include"), (pathToVulkan .. "/Include") }

	libdirs { (pathToSDL .. "/lib/x64"), (pathToVulkan .. "/Lib") }
	links { "Engine", "SDL2", "SDL2main", "vulkan-1" }

	postbuildcommands
	{
		"{COPY} ../Engine/Bin/Ascend.dll Bin/",
		("{COPY} " .. pathToSDL .. "/lib/x64/SDL2.dll Bin/")
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