# Ascend

## Building Ascend

Ascend uses *Premake* to generate project files for your favorite IDE on the OS of your choice. Don't worry if you have never heard of or worked with *Premake*, it's extremely easy. Here's a step-by-step guide:

1. Download Premake binaries for your OS from https://github.com/premake/premake-core/releases and put them into the root folder of the repository, right next to *Generate.lua*.
2. Open *Generate.lua* with your preferred text editor and set the paths of the external dependencies that are not part of the repository  to your local install locations - currently that is *Qt*, *SDL* and *Vulkan*. Install these frameworks if you don't have them yet.
3. Run the premake binary with the identifier for your desired IDE (*vs2017* for Visual Studio 2017 for example, see https://github.com/premake/premake-core/wiki/Using-Premake for more IDE identifiers) and the following switch *--file=Generate.lua*. Your command would look like this if you're on Windows with Visual Studio 2017:

```premake5.exe vs2017 --file=Generate.lua```

4. That's it! You will now find a preconfigured solution file in the root folder. Open that in your IDE and check that everything compiles. Please let me know if you encounter any issues.
