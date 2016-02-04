#include "System.h"

// Rory Clark
// https://rorywebdev.com
// 2016

int main(int argc, char *argv[])
{

	System system;

	// Run throught each start function
	// Splitting them down allows for more information to be displayed to the user in case of error
	try
	{
		// We start SDL first so that we can use the message boxes that SDL provides
		system.StartSDL();
	}
	catch (char* error)
	{
		printf(error);
		return -1;
	}

	try
	{
		// Load the settings file next to allow for custom resolutions etc.
		system.LoadSettings();
	}
	catch (char* error)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Settings Error", error, NULL);
		return -1;
	}
	catch (std::string error)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Settings Error", error.c_str() , NULL);
		return -1;
	}

	// Start OpenGL, this actually just provides OpenGL parameters for SDL and doesn't create the context
	system.StartOpenGL();
	try
	{
		// Create the SDL windows and bind the OpenGL contexts
		system.StartWindow();
	}
	catch (char* error)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Window Error", error, NULL);
		return -1;
	}

	// Start the renders for SDL, both for the main OpenGL window and the SDL settings window
	try
	{
		system.StartRenderer();
	}
	catch (char* error)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Renderer Error", error, NULL);
		return -1;
	}

	// Start SDL_IMG plugin
	try
	{
		system.StartIMG();
	}
	catch (char* error)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Image Error", error, NULL);
		return -1;
	}

	// Start SDL_TTF plugin
	try
	{
		system.StartTTF();
	}
	catch (char* error)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL TTF Error", error, NULL);
		return -1;
	}

	// Start GLEW which also enables the depth tests and culling
	try
	{
		system.StartGLEW();
	}
	catch (char* error)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "GLEW Error", error, NULL);
		return -1;
	}

	// Load the OBJ file
	// This creates the scene and also loads the shaders and textures
	try
	{
		system.LoadObj();
	}
	catch (char* error)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Object Error", error, NULL);
		return -1;
	}
	catch (std::string error)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Object Error", error.c_str(), NULL);
		return -1;
	}

	// Finally load the fonts for the config window
	try
	{
		system.LoadFonts();
	}
	catch (char* error)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Font Error", error, NULL);
		return -1;
	}

	// Start and reset our delta time
	system.StartTime();
	while (system.GetRunning())
	{
		// Update and draw the system, which in turn draws everything else
		system.Update();
		system.Draw();
	}

	return 0;
}
