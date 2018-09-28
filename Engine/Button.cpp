#include "Button.h"

const Font Button::drawFont = "Fonts/LuckyPixel24x36.bmp";

// IMPORTANT NOTE ABOUT C++
// Members are constructed in the order they're defined in
//  in the header file, not the order you construct them
//  in in the initializer list.
Button::Button( const Vei2& center,const std::string& text )
	:
	size( GetSize( text ) ),
	pos( center - size / 2 ),
	text( text ),
	hitbox( pos,size.x,size.y )
{}

void Button::Update( const Mouse& ms )
{
	hovering = ( hitbox.ContainsPoint( ms.GetPos() ) );

	if( ms.LeftIsPressed() && hovering && canClick )
	{
		clicking = true;
	}
	else clicking = false;

	if( !ms.LeftIsPressed() ) canClick = true;
	else canClick = false;
}

void Button::Draw( Graphics& gfx ) const
{
	const auto curDrawCol = ( hovering ? col2 : col1 );
	gfx.DrawRect( pos.x,pos.y,size.x,size.y,curDrawCol );

	drawFont.DrawText( text,pos + padding / 2,textCol,gfx );
}

bool Button::IsDown() const
{
	return( clicking );
}

Vei2 Button::GetSize( const std::string& msg ) const
{
	const int msgWidth = int( msg.size() );
	const auto fontCharSize = drawFont.GetCharSize();
	return( Vei2{ fontCharSize.x * msgWidth,
		fontCharSize.y } + padding );
}
