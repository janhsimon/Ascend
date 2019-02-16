-- list of dependencies
qt = "C:/Qt/5.11.1/msvc2017_64"
sdl = "C:/SDL2-2.0.9"
vulkan = "C:/VulkanSDK/1.1.97.0"

require "External/premake-qt/qt"

workspace "Ascend"
  architecture "x64"
  configurations { "Debug", "Release" }
  startproject "QtSandbox"

include "Engine/Engine.lua"
include "QtSandbox/QtSandbox.lua"
include "SDLSandbox/SDLSandbox.lua"