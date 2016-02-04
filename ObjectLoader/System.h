#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "Scene.h"
#include "ConfigWindow.h"
#include <iomanip>

// Rory Clark
// https://rorywebdev.com
// 2016

class System
{
public:
	System();
	~System();

	void CloseProgram();

	void StartSDL();

	void StartOpenGL();

	void StartWindow();
	void StartRenderer();

	void StartIMG();
	void StartTTF();

	void StartGLEW();

	void StartTime();

	void Update();

	void Draw();

	void LoadSettings();
	void LoadObj();
	void LoadFonts();

	bool GetRunning() { return m_running; };

	std::string GetObjFileName() { return m_fileLoc; };
	std::string GetTextureFileName() { return m_textureLoc; };

	int GetGLMajor() { return m_glMajor; };
	int GetGLMinor() { return m_glMinor; };
	
	int GetScreenSize() { return m_screenSize; };
	int GetScreenLeft() { return m_screenLeft; };
	int GetScreenTop() { return m_screenTop; };
	
	SDL_Window *GetWindow() { return m_window; };
	SDL_Renderer *GetRenderer() { return m_render; };

	float GetDelta() { return m_deltaTime; };

private:

	bool m_running;

	ConfigWindow *m_config;

	SDL_Window *m_window, *m_settingsWindow;
	SDL_Renderer *m_render, *m_settingsRender;

	SDL_GLContext m_glContext;

	Scene* m_scene;

	Uint32 m_newTick, m_oldTick;
	double m_deltaTime;

	std::string m_settingsName;
	std::string m_fileLoc,m_textureLoc,m_vertexShaderLoc,m_fragmentShaderLoc;
	unsigned int m_screenSize, m_screenWidth, m_screenHeight, m_screenLeft, m_screenTop;
	unsigned int m_glMajor, m_glMinor;

	std::string m_fontLoc;
	TTF_Font* m_smallFont;

	int m_mouseX, m_mouseY, m_mouseState;

	float m_cfgUpdate,m_reFocusTimer;
	bool m_reFocus,m_heldTexture,m_usingKeyboard;

	bool GetSetting(std::string &setting, std::vector<std::string> &_lines, std::string _query);
	bool GetSetting(unsigned int &setting, std::vector<std::string> &_lines, std::string _query);
	void CreateNewSettings();
	void CreateConfigInfo();

	template<typename T> std::string StringAnyInput(T _inp);

	void UpdateInput();

	void UpdateConfigDetails();
	void UpdateConfigInput();
	void UpdateSceneControls(int _control, float _value);


};


#endif