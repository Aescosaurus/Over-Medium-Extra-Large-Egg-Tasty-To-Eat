#pragma once

#include "Vec2.h"
#include <string>
#include "Font.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Rect.h"
#include "Surface.h"

class Button
{
public:
	Button( const Vei2& center,const std::string& text );
	Button( int left,int top,const std::string& text );

	void Update( const Mouse& ms );
	virtual void Draw( Graphics& gfx ) const;

	bool IsDown() const;
protected:
	Button( const Vei2& pos,const Vei2& size );
private:
	Vei2 GetSize( const std::string& msg ) const;
protected:
	const Vei2 size;
	const Vei2 pos;
	bool hovering = false;
	const Color col1 = Colors::Gray;
	const Color col2 = Colors::LightGray;
	static constexpr Vei2 padding = { 16 };
private:
	std::string text;
	static const Font drawFont;
	RectI hitbox;
	bool canClick = false;
	bool clicking = false;
	const Color textCol = Colors::White;
};

class ImageButton
	:
	public Button
{
public:
	ImageButton( const Vei2& center,const Surface& image );

	void Draw( Graphics& gfx ) const override;
private:
	const Surface& mySpr;
};