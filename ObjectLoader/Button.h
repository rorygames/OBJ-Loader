#ifndef _BUTTON_H
#define _BUTTON_H

#include "Text.h"

// Rory Clark
// https://rorywebdev.com
// 2016

class Button
{
public:
	Button(SDL_Renderer* _render, Text* _text, int _topX, int _topY, int _width, int _height, float _returnCode, bool _hold);
	~Button();

	void ChangeText(Text* _text);

	float Update(float _delta, int _mouseX, int _mouseY, int _mouseState);
	void Draw();


private:
	int m_topX, m_topY, m_width, m_height;
	int m_textX, m_textY;
	int m_state;
	int m_oldMouseState;
	int m_mouseCode;
	float m_returnCode;

	bool m_hold;

	float m_hoverState, m_clickState;

	SDL_Rect m_buttonRect;
	SDL_Renderer* m_render;

	Text* m_text;
};

#endif