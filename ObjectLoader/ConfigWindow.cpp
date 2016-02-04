#include "ConfigWindow.h"

// Rory Clark
// https://rorywebdev.com
// 2016

ConfigWindow::ConfigWindow(SDL_Renderer* _render,TTF_Font* _font)
{
	// Create our config window and pass through the key info (render, font) and get the size of the window etc
	m_render = _render;
	SDL_GetRendererOutputSize(m_render, &m_width, &m_height);
	m_font = _font;
	SDL_Color w = { 255, 255, 255 };
	m_textColor = w;
	m_textureAvailable = true;
	m_deltaFps = 0;
	m_frames = 0;
}

ConfigWindow::~ConfigWindow()
{
	// Clear our vectors of text objects and buttons
	m_infoText.clear();
	m_buttonText.clear();
	m_buttons.clear();
}

void ConfigWindow::SetInfo(std::vector<std::string> _info, std::vector<int> _keyinfo)
{
	// Load the text based info to our config window
	// Key info vector is the data that will change in the update function
	int yVal = 10;
	int xVal = 2;
	int maxX = 0;
	bool end = false;
	m_keyinfo = _keyinfo;
	for each (std::string str in _info)
	{
		end = false;
		m_infoText.push_back(new Text(m_render));
		m_infoText.back()->LoadText(str, m_textColor, m_font);
		m_infoText.back()->ChangePos(xVal, yVal);
		yVal += 20;
		if (maxX < m_infoText.back()->GetWidth())
		{
			maxX = m_infoText.back()->GetWidth();
		}
		if (yVal >= 115)
		{
			yVal = 10;
			xVal += maxX + 10;
			maxX = 0;
			end = true;
		}
	}
	if (!end)
	{
		xVal += maxX + 10;
	}
	// Create the buttons for our GUI
	this->CreateButtons(xVal);
}

void ConfigWindow::CreateButtons(int _xVal)
{
	// Make our visual array of buttons and position them accordingly
	int yVal = 10, xVal = _xVal;
	this->InitButton("X+", 2, xVal, yVal, true);
	xVal += 80;
	this->InitButton("X-", -2, xVal, yVal, true);
	yVal += 30;
	xVal -= 80;
	this->InitButton("Y+", 2, xVal, yVal, true);
	xVal += 80;
	this->InitButton("Y-", -2, xVal, yVal, true);
	yVal += 30;
	xVal -= 80;
	this->InitButton("Zoom+", 1, xVal, yVal, true);
	xVal += 80;
	this->InitButton("Zoom-", -1, xVal, yVal, true);
	yVal += 30;
	xVal -= 80;
	this->InitButton("Light+", 10, xVal, yVal, true);
	xVal += 80;
	this->InitButton("Light-", -10, xVal, yVal, true);
	yVal = 40;
	xVal += 80;
	this->InitButton("Red+", 0.2, xVal, yVal, true);
	xVal += 80;
	this->InitButton("Red-", -0.2, xVal, yVal, true);
	xVal -= 80;
	yVal += 30;
	this->InitButton("Green+", 0.2, xVal, yVal, true);
	xVal += 80;
	this->InitButton("Green-", -0.2, xVal, yVal, true);
	xVal -= 80;
	yVal += 30;
	this->InitButton("Blue+", 0.2, xVal, yVal, true);
	xVal += 80;
	this->InitButton("Blue-", -0.2, xVal, yVal, true);
	xVal -= 80;
	yVal = 10;
	// Our textures button is different to the rest so we just do the function process manually
	Text* tempText = new Text(m_render);
	tempText->LoadText("Textures On/Off", m_textColor, m_font);
	m_buttons.push_back(new Button(m_render, tempText, xVal, yVal, 155, 25, 1, false));

	// Create a vector for our button return codes
	// We half it so that we only ever have one "set" of buttons returning a value (e.g. the + of x or the - not both)
	// +1 accounts for our texture on/off button
	m_returnCodes = std::vector<float>((m_buttons.size()/2)+1, 0);
}

void ConfigWindow::InitButton(std::string _name, float _return, int _xVal, int _yVal, bool _hold)
{
	// Create the text and load it all in one function (1 line repeated vs 3 lines repeated)
	Text* tempText = new Text(m_render);
	tempText->LoadText(_name, m_textColor, m_font);
	m_buttons.push_back(new Button(m_render, tempText, _xVal, _yVal, 75, 25, _return, _hold));

}

void ConfigWindow::SetTextureEnabled(bool _value)
{
	// Read the new value of the texture enabled/disabled
	if (m_textureAvailable)
	{
		m_textureEnabled = _value;
	}
}

void ConfigWindow::Update(float _delta, int _mouseX, int _mouseY, int _mouseState)
{
	m_deltaFps += _delta;
	m_frames++;

	// We use a lot of boolean checks to prevent the constant changing of text surfaces
	// Text changes are far more expensive than a simple yes/no
	if (m_oldLightPower != m_lightPower)
	{
		m_oldLightPower = m_lightPower;
		m_infoText.at(m_keyinfo.at(1))->ChangeText("Light Power: " + std::to_string(m_lightPower));
	}

	// Check if we have the textures on or off
	if (m_textureEnabled)
	{
		// Check if we haven't changed the text yet
		if (m_oldTextureEnabled != m_textureEnabled)
		{
			m_oldTextureEnabled = m_textureEnabled;
			m_infoText.at(m_keyinfo.at(2))->ChangeText("Texture: Enabled");
			m_infoText.at(m_keyinfo.at(3))->ChangeText("Red: TEXTURE");
			m_infoText.at(m_keyinfo.at(4))->ChangeText("Green: TEXTURE");
			m_infoText.at(m_keyinfo.at(5))->ChangeText("Blue: TEXTURE");
		}
	}
	else
	{
		if (m_oldTextureEnabled != m_textureEnabled)
		{
			m_oldTextureEnabled = m_textureEnabled;
			// Make sure we have the option to swap textures, no point changing otherwise
			if (m_textureAvailable)
			{
				m_infoText.at(m_keyinfo.at(2))->ChangeText("Texture: Disabled");
			}
			m_oldRed = -1;
			m_oldGreen = -1;
			m_oldBlue = -1;
		}
		// Our colour values are "converted" to %'s
		if (m_oldRed != m_red)
		{
			m_oldRed = m_red;
			m_infoText.at(m_keyinfo.at(3))->ChangeText("Red: " + std::to_string((m_red * 100)) + "%");
		}
		if (m_oldGreen != m_green)
		{
			m_oldGreen = m_green;
			m_infoText.at(m_keyinfo.at(4))->ChangeText("Green: " + std::to_string((m_green * 100)) + "%");
		}
		if (m_oldBlue != m_blue)
		{
			m_oldBlue = m_blue;
			m_infoText.at(m_keyinfo.at(5))->ChangeText("Blue: " + std::to_string((m_blue * 100)) + "%");
		}
	}
	
	float count = 0;
	m_returnCodes = std::vector<float>(m_returnCodes.size(), 0);
	// Update all our buttons but only record the return value if we haven't set one with the previous group of buttons
	// int values less than the whole will always report the number below so adding +0.5 each time works perfectly for 2 numbers as an index
	for each (Button* btn in m_buttons)
	{
		if (m_returnCodes.at((int)count) != 0)
		{
			btn->Update(_delta, _mouseX, _mouseY, _mouseState);
		}
		else
		{
			m_returnCodes.at((int)count) = btn->Update(_delta, _mouseX, _mouseY, _mouseState);
		}
		count+= 0.5;
	}

	// Update the FPS text every second
	if (m_deltaFps >= 1.0f)
	{
		"FPS: " + std::to_string(m_frames);
		m_infoText.at(m_keyinfo.at(0))->ChangeText("FPS: " + std::to_string(m_frames));
		m_deltaFps = 0.0f;
		m_frames = 0;
	}
}

void ConfigWindow::Draw()
{
	// Black the render draw colour
	SDL_SetRenderDrawColor(m_render, 0, 0, 0, 255);
	// Clear our render with the above colour
	SDL_RenderClear(m_render);
	
	// Draw all our text and buttons
	for each (Text* t in m_infoText)
	{
		t->Draw();
	}
	for each (Button* btn in m_buttons)
	{
		btn->Draw();
	}
	// Present the render once everything is drawn (double buffering)
	SDL_RenderPresent(m_render);
}