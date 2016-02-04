OBJ-Loader
======

An .OBJ loader written in C++ using OpenGL and SDL2 with specular lighting and texturing.

It uses a secondary window for a GUI that handles frame rate and information.

Information
======
The OBJ loader is designed to read vertex based .OBJ models with external texture files. It can be configured to run at any (windowed) resolution.

Models are stored in VBOs (vertex buffer objects).

Shader files are stored and read externally. There is both a vertex and fragment shader included with the project that allow for texturing and specular lighting calculations with user configured run time changes.

Input, general window management and secondary window usage use SDL2, the main window uses an OpenGL implementation.

Included File
======
The included Visual Studio project is set up so that all include files are automatically in the project.

This means no extra files should be required to run or open the project.

**A pre-built version of the loader is included in the Release folder.**

Requirements
======

- Visual Studio 2013 (minimum)

The project and code utilise features found within the 2013 version of Visual Studio, using older versions will cause errors due to the compiler.
