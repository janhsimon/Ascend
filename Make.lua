-- list of dependencies
qt = "C:/Qt/5.11.1/msvc2017_64"
sdl = "C:/SDL2-2.0.9"
vulkan = os.getenv("VULKAN_SDK")

require "Lib/premake-qt/qt"

workspace "Ascend"
	architecture "x64"
	configurations { "Debug", "Release" }
	startproject "QtSandbox"

include "Engine/Make.lua"
include "QtSandbox/Make.lua"
include "SDLSandbox/Make.lua"