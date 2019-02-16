project "QtSandbox"
  kind "ConsoleApp" --"WindowedApp"
  language "C++"
  files { "QtSandbox.lua", "Source/**.*" }
  objdir "Temp"
  targetdir "Bin"
  debugdir "Bin"

  includedirs
  {
    "%{wks.location}/Engine/Include",
    "%{wks.location}/External",
    (vulkan .. "/Include")
  }

  libdirs { (vulkan .. "/Lib") }
  links { "Engine", "vulkan-1" } -- qtmaind.lib should allow us to make a WindowedApp but causes linker errors

  postbuildcommands
  {
    ("{COPY} " .. qt .. "/bin/Qt5Core.dll Bin/"),
    ("{COPY} " .. qt .. "/bin/Qt5Gui.dll Bin/"),
    ("{COPY} " .. qt .. "/bin/Qt5Widgets.dll Bin/")
  }

  require "External/premake-qt/qt"

  premake.extensions.qt.enable()
  qtpath(qt)
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
    defines "DEBUG"
    optimize "On"

  filter { "files:Source/Shaders/*", "configurations:Debug" }
    buildcommands (vulkan .. "/bin/glslc.exe %{file.relpath} -o Bin/Shaders/%{file.name}.spv -O0")
    buildoutputs "Bin/Shaders/%{file.name}.spv"

  filter { "files:Source/Shaders/*", "configurations:Release" }
    buildcommands (vulkan .. "/bin/glslc.exe %{file.relpath} -o Bin/Shaders/%{file.name}.spv -Os")
    buildoutputs "Bin/Shaders/%{file.name}.spv"