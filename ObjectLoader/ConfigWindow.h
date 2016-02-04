#ifndef _CONFIGWINDOW_H
#define _CONFIGWINDOW_H

#include "Button.h"

// Rory Clark
// https://rorywebdev.com
// 2016

class ConfigWindow
{
public:
	ConfigWindow(SDL_Renderer* _render, TTF_Font* _font);
	~ConfigWindow();

	void SetInfo(std::vector<std::string> _info, std::vector<int> _keyinfo);
	void SetLightPower(float _value) { m_lightPower = _value; };
	void SetTextureAvailable(bool _value) { m_textureAvailable = _value; };
	void SetTextureEnabled(bool _value);
	void SetRedColour(float _value) { m_red = _value; };
	void SetGreenColour(float _value) { m_green = _value; };
	void SetBlueColour(float _value) { m_blue = _value; };

	std::vector<float> GetReturnCodes() { return m_returnCodes; };

	void Update(float _delta, int _mouseX, int _mouseY, int _mouseState);

	void Draw();
private:

	SDL_Renderer* m_render;

	float m_deltaFps,m_lightPower,m_oldLightPower,m_red,m_oldRed,m_blue,m_oldBlue,m_green,m_oldGreen;
	bool m_textureEnabled,m_textureAvailable,m_oldTextureEnabled;
	int m_frames, m_oldFps;
	int m_width, m_height;

	SDL_Color m_textColor;
	TTF_Font* m_font;

	std::vector<Text*> m_infoText;
	std::vector<int> m_keyinfo;

	std::vector<Text*> m_buttonText;
	std::vector<Button*> m_buttons;
	std::vector<float> m_returnCodes;

	void CreateButtons(int xVal);
	void InitButton(std::string _name, float _return, int _xVal, int _yVal, bool _hold);

};

#endif