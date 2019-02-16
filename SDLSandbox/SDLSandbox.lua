project "SDLSandbox"
  kind "WindowedApp"
  language "C++"
  files { "SDLSandbox.lua", "Source/**.*", }
  objdir "Temp"
  targetdir "Bin"
  debugdir "Bin"

  includedirs
  {
    "%{wks.location}/Engine/Include",
    "%{wks.location}/External",
    (sdl .. "/include"),
    (vulkan .. "/Include")
  }

  libdirs { (sdl .. "/lib/x64"), (vulkan .. "/Lib") }
  links { "Engine", "SDL2", "SDL2main", "vulkan-1" }

  postbuildcommands
  {
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