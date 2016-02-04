#ifndef _TEXT_H
#define _TEXT_H

#include "GenericIncludes.h"

// Rory Clark
// https://rorywebdev.com
// 2016

class Text
{
public:
	Text(SDL_Renderer* _render);
	~Text();

	bool LoadText(std::string _textureText, SDL_Color _textColor, TTF_Font *_textureFont);

	void ChangeText(std::string _textureText);
	void ClipSizes(int _width, int _height);

	int GetWidth() { return m_width; };
	int GetHeight() { return m_height; };

	void ChangePos(int _x, int _y);

	void Draw();

private:

	void FreeText();

	SDL_Renderer* m_render;
	SDL_Texture* m_text;

	SDL_Rect m_rect;

	TTF_Font* m_textFont;
	SDL_Color m_textColor;
	std::string m_textString;

	int m_x,m_y, m_width, m_height;

};

#endif