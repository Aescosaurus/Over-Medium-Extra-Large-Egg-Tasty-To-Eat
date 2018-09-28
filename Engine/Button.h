#pragma once

#include "Vec2.h"
#include <string>
#include "Font.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Rect.h"

class Button
{
public:
	Button( const Vei2& center,const std::string& text );

	void Update( const Mouse& ms );
	void Draw( Graphics& gfx ) const;

	bool IsDown() const;
private:
	Vei2 GetSize( const std::string& msg ) const;
private:
	const Vei2 size;
	const Vei2 pos;
	std::string text;
	static const Font drawFont;
	static constexpr Vei2 padding = { 16 };
	RectI hitbox;
	bool hovering = false;
	bool canClick = false;
	bool clicking = false;
	const Color col1 = Colors::Gray;
	const Color col2 = Colors::LightGray;
	const Color textCol = Colors::White;
};