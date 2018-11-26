@echo off

del "*.sln" /s /f /q

rd "Engine\Temp" /s /q
del "Engine\Ascend.*" /s /f /q
del "Engine\Engine.*" /s /f /q

rd "Sandbox\Temp" /s /q
del "Sandbox\*.dll /s /f /q
del "Sandbox\Sandbox.* /s /f /q

Premake\premake5.exe vs2017 --file=Ascend.lua

pause