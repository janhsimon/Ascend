# Ascend

## Building Ascend

Ascend uses *Premake* to generate project files for your favorite IDE on the OS of your choice. Don't worry if you have never heard of or worked with *Premake*, it's extremely easy. Here's a step-by-step guide:

1. Download Premake binaries for your OS from https://github.com/premake/premake-core/releases and put them into the root folder of the repository, right next to *Generate.lua*.
1. Open *Generate.lua* with your preferred text editor and set the paths to the external dependencies that are not part of the repository (currently that is Qt, SDL and Vulkan). Install these frameworks if you don't have them yet.
1. Run the premake binary and pass it the identifier for your desired IDE (*vs2017* for Visual Studio 2017 for example) and the following switch *--file=Generate.lua*. On Windows, your command could look like this: *premake5.exe vs2017 --file=Generate.lua*
1. You will now find a preconfigured solution file in the root folder, it should compile all projects (the engine and the sandbox projects) out of the box. Please let me know if you encounter any issues.
