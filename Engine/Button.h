#pragma once

#include "Vec2.h"
#include <string>
#include "Font.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Rect.h"
#include "Surface.h"
#include "Codex.h"
#include "Anim.h"

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
private:
	// Sorry this guy has to be initialized before all those protected members.
	const Font* const drawFont = Codex<Font>::Retrieve( "Fonts/LuckyPixel24x36.bmp" );
protected:
	const Vei2 size;
	const Vei2 pos;
	bool hovering = false;
	const Color col1 = Colors::Gray;
	const Color col2 = Colors::LightGray;
	static constexpr Vei2 padding = { 16 };
private:
	std::string text;
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
	ImageButton( const Vei2& pos,const Vei2& size,
		const Surface& image );

	void Draw( Graphics& gfx ) const override;
	virtual void Draw( Graphics& gfx,bool flipped ) const;
private:
	const Surface& mySpr;
};

class AnimButton
	:
	public ImageButton
{
public:
	AnimButton( const Vei2& center,Anim& myAnim );

	void Draw( Graphics& gfx,bool flipped ) const override;
private:
	Anim& myAnim;
};