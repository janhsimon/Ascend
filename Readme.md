# Ascend

## Building Ascend

*Ascend* uses *Premake* to generate project files for your favorite IDE on the OS of your choice. Don't worry if you have never heard of or worked with *Premake*, it's extremely easy. Here's a step-by-step guide:

1. Download the newest *Premake* binary for your OS from https://github.com/premake/premake-core/releases and place it in the root folder of this repository.
2. Open *Generate.lua* with your preferred text editor. You will find a number of paths at the beginning of the file. These are external dependencies, they are not part of the repository and have to be downloaded and installed on your machine locally. Edit these paths so that they point to the correct install locations on your system. Here's a handy list of these external dependencies and where to get them:
   1. *Qt*: Download *Qt* from https://www.qt.io/download.
   2. *SDL*: Download the *SDL* development libraries from https://www.libsdl.org/download-2.0.php.
   3. *Vulkan*: Download the *Vulkan* SDK from https://vulkan.lunarg.com/sdk/home.
3. Run *Premake* with the identifier for your desired IDE (*vs2017* for *Visual Studio 2017* for example) and point it to *Generate.lua* with *--file=Generate.lua*. You can find a list of all IDE identifiers here: https://github.com/premake/premake-core/wiki/Using-Premake. This is what your command would look like to generate a *Visual Studio 2017* solution:

   ```premake5 vs2017 --file=Generate.lua```

4. That's it! You have successfully generated preconfigured project files that should compile and link out of the box.
