#include "System.h"

// Rory Clark
// https://rorywebdev.com
// 2016

System::System()
{
	// Set our default values
	m_settingsName = "settings.ini";
	m_running = false;
	m_cfgUpdate = 0.0f;
	m_reFocus = false;
	m_reFocusTimer = 0.0f;
	m_heldTexture = false;
	m_usingKeyboard = false;
}

System::~System()
{
	this->CloseProgram();
}

void System::CloseProgram()
{
	// Clear our memory
	delete m_scene;
	delete m_config;
	// Delete the contexts, renders and windows created by SDL
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyRenderer(m_render);
	SDL_DestroyRenderer(m_settingsRender);
	SDL_DestroyWindow(m_window);
	SDL_DestroyWindow(m_settingsWindow);
	// Quit our plugins and SDL itself
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void System::StartSDL()
{
	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw("ERROR: SDL could not be initialised.");
	}
}

void System::StartOpenGL()
{
	// Run the OGL attributes by using the options read from our settings file
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, m_glMajor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, m_glMinor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Update ASAP, no v-sync unless forced by system
	SDL_GL_SetSwapInterval(0);
}

void System::StartWindow()
{
	// Create our normal window
	m_window = SDL_CreateWindow("Object Loader", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_screenWidth, m_screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	int x, y;
	SDL_GetWindowPosition(m_window, &x, &y);
	// Create our config window below our main
	m_settingsWindow = SDL_CreateWindow("Object Loader Config", x , y+m_screenHeight, m_screenWidth, 130, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
	if (m_window == NULL || m_settingsWindow == NULL)
	{
		throw("Unable to create window.");
	}
}

void System::StartRenderer()
{
	// Start our two renderers for the main and config window
	m_render = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	m_settingsRender = SDL_CreateRenderer(m_settingsWindow, -1, SDL_RENDERER_ACCELERATED);
	if (m_render == NULL || m_settingsRender == NULL)
	{
		throw("Unable to create renderer.");
	}
	// Create the OpenGL context for the main window
	m_glContext = SDL_GL_CreateContext(m_window);
	if (m_glContext == NULL)
	{
		throw("Unable to create OpenGL context.");
	}
	// Allow SDL to do alpha blending on our config window
	SDL_SetRenderDrawBlendMode(m_settingsRender, SDL_BLENDMODE_BLEND);
}

void System::StartIMG()
{
	// Start the IMG plugin to read PNG and JPG
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		throw("Unable to start SDL Image.\nIs sdl2_image.dll in the folder?\nError: %s",IMG_GetError());
	}
}

void System::StartTTF()
{
	// Start the TTF plugin
	if (TTF_Init() == -1)
	{
		throw("Unable to start SDL TTF.\nIs sdl2_ttf.dll & zlib1.dll in the folder?\nError: %s", TTF_GetError());
	}
}

void System::StartTime()
{
	// Set all our delta times to 0 and start the ticks
	m_newTick = SDL_GetTicks();
	m_oldTick = m_newTick;
	m_deltaTime = 0.0f;
	m_running = true;
}

void System::StartGLEW()
{
	// Start GLEW and enable the depth tests and face culling
	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		throw("GLEW failed to initiliase.\nError: %s",glewGetErrorString(err));
	}
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// Removes faces not currently visible to the camera
	glEnable(GL_CULL_FACE);
}

void System::Update()
{
	// Update the delta time
	m_oldTick = m_newTick;
	m_newTick = SDL_GetTicks();
	m_deltaTime = (m_newTick - m_oldTick) / 1000.0f;
	m_cfgUpdate += m_deltaTime;
	// Update the events/input
	this->UpdateInput();
	// Update the scene
	m_scene->Update(m_deltaTime);
	// Update the config window's text and variables
	this->UpdateConfigDetails();
	// Update the config window itself with mouse coords
	m_config->Update(m_deltaTime,m_mouseX,m_mouseY,m_mouseState);
	
	// Update the input for the config window
	if (!m_usingKeyboard)
	{
		this->UpdateConfigInput();
	}
	
	// Prevent our windows constantly trying to gain focus from one another
	m_reFocusTimer += m_deltaTime;
	if (m_reFocus && m_reFocusTimer >= 0.1f)
	{
		m_reFocus = false;
	}

	// If we've moved the main window then we need to drag along our config window as it doesn't have a border
	int mx, my, cx, cy;
	SDL_GetWindowPosition(m_window, &mx, &my);
	SDL_GetWindowPosition(m_settingsWindow, &cx, &cy);
	if (cy != (my + m_screenHeight) || cx != mx)
	{
		SDL_SetWindowPosition(m_settingsWindow, mx, my + m_screenHeight);
	}

}

void System::UpdateInput()
{
	SDL_Event ev;
	while (SDL_PollEvent(&ev) != 0)
	{
		switch (ev.type)
		{
			// Mouse events
		case SDL_MOUSEMOTION:
			if (ev.window.windowID == 2)
			{
				SDL_GetMouseState(&m_mouseX, &m_mouseY);
			}
			else
			{
				m_mouseX = 0;
				m_mouseY = 0;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (ev.window.windowID == 2)
			{
				m_mouseState = 1;
			}
			else
			{
				m_mouseState = 0;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			m_mouseState = 0;
			break;

			// Window events, we want to make sure out config window always has mouse focus
			// Seeing how keyboard events work on either it doesn't matter so much for that
		case SDL_WINDOWEVENT:
			switch (ev.window.event)
			{
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				if (!m_reFocus)
				{
					m_reFocus = true;
					m_reFocusTimer = 0.0f;
					SDL_RaiseWindow(m_window);
					SDL_RaiseWindow(m_settingsWindow);					
				}
				break;
			}
			break;

			// The console is closed
		case SDL_QUIT:
			m_running = false;
			break;
			
			// Key downs used for changing OGL values
		case SDL_KEYDOWN:
			// Prevents the blank mouse GUI inputs from taking over
			m_usingKeyboard = true;
			switch (ev.key.keysym.sym)
			{
			case SDLK_w:
				UpdateSceneControls(1,2);
				break;
			case SDLK_s:
				UpdateSceneControls(1, -2);
				break;
			case SDLK_a:
				UpdateSceneControls(0, 2);
				break;
			case SDLK_d:
				UpdateSceneControls(0, -2);
				break;
			case SDLK_q:
				UpdateSceneControls(2, 1);
				break;
			case SDLK_e:
				UpdateSceneControls(2, -1);
				break;
			case SDLK_r:
				UpdateSceneControls(3, 10);
				break;
			case SDLK_f:
				UpdateSceneControls(3, -10);
				break;
			case SDLK_g:
				UpdateSceneControls(4, 0.2);
				break;
			case SDLK_b:
				UpdateSceneControls(4, -0.2);
				break;
			case SDLK_h:
				UpdateSceneControls(5, 0.2);
				break;
			case SDLK_n:
				UpdateSceneControls(5, -0.2);
				break;
			case SDLK_j:
				UpdateSceneControls(6, 0.2);
				break;
			case SDLK_m:
				UpdateSceneControls(6, -0.2);
				break;
			case SDLK_v:
				// Make sure not to constantly swap textures on and off
				if (!m_heldTexture)
				{
					m_heldTexture = true;
					UpdateSceneControls(7, 1);
				}
				break;
			}
			break;

			// Reset our values on key up so it stops moving/changing
		case SDL_KEYUP:
			m_usingKeyboard = false;
			switch (ev.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				// Quit the program!
				m_running = false;
				break;
			case SDLK_w:
			case SDLK_s:
				UpdateSceneControls(1, 0);
				break;
			case SDLK_a:
			case SDLK_d:
				UpdateSceneControls(0, 0);
				break;
			case SDLK_q:
			case SDLK_e:
				UpdateSceneControls(2, 0);
				break;
			case SDLK_r:
			case SDLK_f:
				UpdateSceneControls(3, 0);
				break;
			case SDLK_g:
			case SDLK_b:
				UpdateSceneControls(4, 0);
				break;
			case SDLK_h:
			case SDLK_n:
				UpdateSceneControls(5, 0);
				break;
			case SDLK_j:
			case SDLK_m:
				UpdateSceneControls(6, 0);
				break;
			case SDLK_v:
				m_heldTexture = false;
				break;
			}
			break;
		}
	}
}

void System::UpdateConfigDetails()
{
	// Send our text info for the key info items
	m_config->SetLightPower(m_scene->GetLightPower());
	m_config->SetTextureEnabled(m_scene->GetIsTexture());
	m_config->SetRedColour(m_scene->GetRedColour());
	m_config->SetGreenColour(m_scene->GetGreenColour());
	m_config->SetBlueColour(m_scene->GetBlueColour());
}

void System::UpdateConfigInput()
{
	// Check to see if any buttons are pressed and update their associated scene control
	std::vector<float> codes = m_config->GetReturnCodes();
	for (int i = 0; i < codes.size(); i++)
	{
		UpdateSceneControls(i, codes.at(i));
	}
}

void System::UpdateSceneControls(int _control, float _value)
{
	switch (_control)
	{
	case 0:
		// Camera X
		m_scene->SetCameraRotX(_value);
		break;
	case 1:
		// Camera Y
		m_scene->SetCameraRotY(_value);
		break;
	case 2:
		// Zoom
		m_scene->SetCameraZoom(_value);
		break;
	case 3:
		// Light Power
		m_scene->SetSceneLight(_value);
		break;
	case 4:
		// Red value
		m_scene->SetRColour(_value);
		break;
	case 5:
		// Green value
		m_scene->SetGColour(_value);
		break;
	case 6:
		// Blue value
		m_scene->SetBColour(_value);
		break;
	case 7:
		// Make sure to only swap when necessary
		// The GUI button for this only fires on mouse up
		if (_value != 0)
		{
			m_scene->SwapTexture();
		}
		break;
	}
}

void System::Draw()
{
	// Set the opengl clear colour to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// This writes the above colour to the colour part of the framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Draw the scene
	m_scene->Draw();
	// Draw the config window at 20fps
	// We don't want to impact the OGL context performance too much
	if (m_cfgUpdate > 0.05f)
	{
		m_config->Draw();
		m_cfgUpdate = 0.0f;
	}
	SDL_GL_SwapWindow(m_window);
}

void System::LoadObj()
{
	// Create the scene
	m_scene = new Scene(m_screenWidth,m_screenHeight);
	// Then the model
	m_scene->OBJModel = new Model();
	// Then load the shaders from the config file locations
	m_scene->LoadShaderFiles(m_vertexShaderLoc, m_fragmentShaderLoc);
	// Then actually create and compile them
	m_scene->CreateShader();
	// Then actually load in our obj file
	m_scene->OBJModel->LoadObj(m_fileLoc);
	// Load our texture if we have one
	if (m_textureLoc.length() > 0)
	{
		m_scene->OBJModel->LoadTexture(m_textureLoc);
		m_scene->SetTextureEnabled(true);
	}
	else
	{
		m_scene->SetTextureEnabled(false);
	}
	// And finally buffer and create our model in GPU memory
	m_scene->OBJModel->CreateModel();
}

void System::LoadFonts()
{
	// Load our fonts, although we only have one it's not difficult to have more
	m_smallFont = TTF_OpenFont(m_fontLoc.c_str(), 12);
	if (m_smallFont == NULL)
	{
		throw("Unable to load font file.\nAre you sure there is a font file in the folder?");
	}
	this->CreateConfigInfo();
}

void System::CreateConfigInfo()
{
	// Add all of the text for our config window
	m_config = new ConfigWindow(m_settingsRender, m_smallFont);
	std::vector<std::string> info;
	std::vector<int> keyinfo;
	// Our OBJ file name
	info.push_back("OBJ: " + m_fileLoc);
	// Version of GLEW
	info.push_back("GLEW Version: " + StringAnyInput(glewGetString(GLEW_VERSION)));
	// Vender of OpenGL (graphics card manufacturer)
	info.push_back("OpenGL Vendor: " + StringAnyInput(glGetString(GL_VENDOR)));
	// OGL Renderer (the graphics card itself)
	info.push_back("OpenGL Renderer: " + StringAnyInput(glGetString(GL_RENDERER)));
	// Version of opengl we're using
	info.push_back("OpenGL Version: " + StringAnyInput(glGetString(GL_VERSION)));
	// Shader language version
	info.push_back("OpenGL SL Version: " + StringAnyInput(glGetString(GL_SHADING_LANGUAGE_VERSION)));
	// FPS
	info.push_back("FPS:");
	keyinfo.push_back(info.size()-1);
	// Rest are self explanatory
	info.push_back("Resolution: " + std::to_string(m_screenWidth) + "x" + std::to_string(m_screenHeight));
	info.push_back("Vertices: " + std::to_string(m_scene->OBJModel->GetVerticesCount()));
	info.push_back("Triangles: " + std::to_string(m_scene->OBJModel->GetVerticesCount() / 3));
	info.push_back("UVs: " + std::to_string(m_scene->OBJModel->GetUVCount()));
	info.push_back("Normals: " + std::to_string(m_scene->OBJModel->GetNormalCount()));
	info.push_back("Light Power: " + std::to_string(m_scene->GetLightPower()));
	keyinfo.push_back(info.size()-1);
	std::string temp,r,g,b;
	// If we've got a texture then we enable and name as appropriately
	if (m_textureLoc.length() > 0)
	{
		temp = "Enabled";
		r = "TEXTURE";
		g = "TEXTURE";
		b = "TEXTURE";
		m_config->SetTextureAvailable(true);
		m_config->SetTextureEnabled(true);
	}
	else
	{
		temp = "None available";
		r = "0%";
		g = "50%";
		b = "100%";
		m_config->SetTextureAvailable(false);
		m_config->SetTextureEnabled(false);
	}
	info.push_back("Texture: " + temp);
	keyinfo.push_back(info.size() - 1);
	info.push_back("Red: " + r);
	keyinfo.push_back(info.size() - 1);
	info.push_back("Green: " + g);
	keyinfo.push_back(info.size() - 1);
	info.push_back("Blue: " + b);
	keyinfo.push_back(info.size() - 1);
	
	// Set out info in the config
	m_config->SetInfo(info, keyinfo);
}

void System::LoadSettings()
{
	// Load our settings from our settings file
	std::vector<std::string> settingsLines;
	std::string line;
	std::ifstream settingsFile(m_settingsName);
	if (settingsFile)
	{
		while (std::getline(settingsFile, line))
		{
			// Ignore comments and blank lines
			if (line.find("//") != 0 && line.length() != 0)
			{
				settingsLines.push_back(line);
			}
		}
		settingsFile.close();
	}
	else
	{
		// Create a new settings if we can't find the file
		CreateNewSettings();
		throw("Unable to find settings, default settings file has been created.\nPlease add a file name and relaunch.");
	}
	std::string throwMsg = "Unable to find setting ";
	std::string throwAfter = ", default settings file has been created.\nPlease add a file name and relaunch.";
	std::string throwRes;
	// We search for our required settings and then error and create a new file if we can't find them
	if (!this->GetSetting(m_fileLoc, settingsLines, "file"))
	{
		throwRes = throwMsg + "\"file\"" + throwAfter;
		throw(throwRes);
	}

	if (!this->GetSetting(m_textureLoc, settingsLines, "texture"))
	{
		throwRes = throwMsg + "\"texture\"" + throwAfter;
		throw(throwRes);
	}

	if (!this->GetSetting(m_glMajor, settingsLines, "glmajor"))
	{
		throwRes = throwMsg + "\"glmajor\"" + throwAfter;
		throw(throwRes);
	}

	if (!this->GetSetting(m_glMinor, settingsLines, "glminor"))
	{
		throwRes = throwMsg + "\"glminor\"" + throwAfter;
		throw(throwRes);
	}

	if (!this->GetSetting(m_screenWidth, settingsLines, "width"))
	{
		throwRes = throwMsg + "\"width\"" + throwAfter;
		throw(throwRes);
	}

	if (!this->GetSetting(m_screenHeight, settingsLines, "height"))
	{
		throwRes = throwMsg + "\"height\"" + throwAfter;
		throw(throwRes);
	}

	if (!this->GetSetting(m_fontLoc, settingsLines, "font"))
	{
		throwRes = throwMsg + "\"font\"" + throwAfter;
		throw(throwRes);
	}

	if (!this->GetSetting(m_vertexShaderLoc, settingsLines, "vertex"))
	{
		throwRes = throwMsg + "\"vertex\"" + throwAfter;
		throw(throwRes);
	}

	if (!this->GetSetting(m_fragmentShaderLoc, settingsLines, "fragment"))
	{
		throwRes = throwMsg + "\"fragment\"" + throwAfter;
		throw(throwRes);
	}

	// Remove all our data after (if there's any left)
	settingsLines.clear();
}

void System::CreateNewSettings()
{
	// Create the new settings file, overwriting any old one
	std::ofstream newSettings;
	newSettings.open(m_settingsName);
	newSettings << "// OBJ Loader by Rory Clark - https://rorywebdev.com\n//\n// Mouse input can be used however the keyboard is also viable.\n//\n// A + D will control X axis movement\n// W + S will control Y axis movement\n// Q + E will control zoom\n// R + F control light power\n// V will enable/disable textures when they are present\n// G + B will increase/decrease red values\n// H + N will increase/decrease green values\n// J + M will increase/decrease blue values\n// Esc will close the program at any point\n//\n// Please make sure your OBJ file only has one object inside of it, faces need to be triangulated.\n\n";
	newSettings << "file=INSERT YOUR FILE NAME\ntexture=INSERT TEXTURE FILE NAME\nglmajor=4\nglminor=3\nwidth=1280\nheight=720\nfont=INSERT FONT FILE NAME\nvertex=INSERT VERTEX SHADER FILE NAME\nfragment=INSERT FRAGMENT SHADER FILE NAME";
	newSettings.close();
}

// Search our settings lines for strings, return true for found, false for not
// Removes the line if found to reduce searching next time
bool System::GetSetting(std::string &_setting, std::vector<std::string> &_lines, std::string _query)
{
	int index = -1;
	for (int i = 0; i < _lines.size(); i++)
	{
		if (_lines.at(i).find(_query) != std::string::npos)
		{
			index = i;
			i = _lines.size();
		}
	}
	if (index != -1)
	{
		_setting = _lines.at(index).substr(_query.length() + 1);
		_lines.erase(_lines.begin() + index);
		return true;
	}
	else
	{
		this->CreateNewSettings();
		return false;
	}
}

// Same as above but for integer values
bool System::GetSetting(unsigned int &_setting, std::vector<std::string> &_lines, std::string _query)
{
	int index = -1;
	for (int i = 0; i < _lines.size(); i++)
	{
		if (_lines.at(i).find(_query) != std::string::npos)
		{
			index = i;
			i = _lines.size();
		}
	}
	if (index != -1)
	{
		_setting = atoi(_lines.at(index).substr(_query.length() + 1).c_str());
		_lines.erase(_lines.begin() + index);
		return true;
	}
	else
	{
		this->CreateNewSettings();
		return false;
	}
}

// Read in any input from anything and return a string that we can actually work with
template<typename T> std::string System::StringAnyInput(T _inp)
{
	std::stringstream input;
	input << _inp;
	return input.str();
}