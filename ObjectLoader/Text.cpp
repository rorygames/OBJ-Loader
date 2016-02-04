#include "Text.h"

// Rory Clark
// https://rorywebdev.com
// 2016

Text::Text(SDL_Renderer* render)
{
	// Assign out values
	this->m_text = NULL;
	this->m_width = NULL;
	this->m_height = NULL;
	this->m_render = render;
}

Text::~Text()
{
	// Delete the surfaces on destruct
	this->FreeText();
	this->m_render = NULL;
}

bool Text::LoadText(std::string _textureText, SDL_Color _textColor, TTF_Font* _textureFont)
{
	// Make sure to delete any previous surfaces (unlikely but just in case)
	this->FreeText();

	// Create the new surface by using the SDL_TTF plugin, blended so we have the lovely alpha values of the text
	SDL_Surface* textSurface = TTF_RenderText_Blended(_textureFont, _textureText.c_str(), _textColor);
	if (textSurface == NULL)
	{
		// If we can't make it then error
		printf("Unable to render text surface. SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		// Otherwise create and store our values for later use
		this->m_textString = _textureText;
		this->m_textColor = _textColor;
		this->m_textFont = _textureFont;
		// Create the texture we're going to draw from the surface we just created
		this->m_text = SDL_CreateTextureFromSurface(this->m_render, textSurface);
		if (m_text == NULL)
		{
			printf("Unable to create texture from rendered text. SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			// Assign the sizes from the surface and store them for drawing
			m_width = textSurface->w;
			m_height = textSurface->h;
			SDL_Rect rect = { 0, 0, m_width, m_height };
			m_rect = rect;
		}
		SDL_FreeSurface(textSurface);
	}

	return this->m_text != NULL;
}

void Text::ChangeText(std::string _textureText)
{
	// Very similar to load text however we re-use our values we set like size, position, font, colour and render
	this->FreeText();

	SDL_Texture* oldTex = m_text;

	SDL_Surface* textSurface = TTF_RenderText_Blended(this->m_textFont, _textureText.c_str(), this->m_textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface. SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		this->m_textString = _textureText;
		m_text = SDL_CreateTextureFromSurface(this->m_render, textSurface);
		if (m_text == NULL)
		{
			printf("Unable to create texture from rendered text. SDL Error: %s\n", SDL_GetError());
			m_text = oldTex;
			oldTex = NULL;
		}
		else
		{
			m_width = textSurface->w;
			m_height = textSurface->h;
			SDL_Rect tRect = { m_x, m_y, m_width, m_height };
			m_rect = tRect;
			oldTex = NULL;
		}

		SDL_FreeSurface(textSurface);
	}

}

void Text::ClipSizes(int width, int height)
{
	// Cut the sizes of the texture so that we could transition the text in and out
	if (width != -1)
	{
		this->m_width = width;
	}
	if (height != -1)
	{
		this->m_height = height;
	}
	SDL_Rect tRect = { m_x, m_y, m_width, m_height };
	m_rect = tRect;
}

void Text::FreeText()
{
	if (this->m_text != NULL)
	{
		// Destroy our texture
		SDL_DestroyTexture(this->m_text);
		this->m_text = NULL;
		this->m_width = 0;
		this->m_height = 0;
	}
}

void Text::ChangePos(int _x, int _y)
{
	// Change the position which actually changes the drawing rectangle
	SDL_Rect tRect = { _x, _y, m_width, m_height };
	m_x = _x;
	m_y = _y;
	m_rect = tRect;
}

void Text::Draw()
{
	// Copy the texture to the render with the rectangle used above being our bounds
	SDL_RenderCopy(this->m_render, this->m_text, NULL, &m_rect);
}