#include "Button.h"
#include "SpriteEffect.h"

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

Button::Button( int left,int top,const std::string& text )
	:
	Button( Vei2{ left,top } + GetSize( text ) / 2,text )
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

	drawFont->DrawText( text,pos + padding / 2,textCol,gfx );
}

bool Button::IsDown() const
{
	return( clicking );
}

Button::Button( const Vei2& pos,const Vei2& size )
	:
	size( size ),
	pos( pos ),
	text( "" ),
	hitbox( pos,size.x,size.y )
{
}

Vei2 Button::GetSize( const std::string& msg ) const
{
	const int msgWidth = int( msg.size() );
	const auto fontCharSize = drawFont->GetCharSize();
	return( Vei2{ fontCharSize.x * msgWidth,
		fontCharSize.y } + padding );
}

ImageButton::ImageButton( const Vei2& center,const Surface& image )
	:
	Button( center - image.GetSize() / 2 - padding / 4,image.GetSize() + padding / 2 ),
	mySpr( image )
{}

ImageButton::ImageButton( const Vei2& pos,const Vei2& size,
	const Surface& image )
	:
	Button( pos,size ),
	mySpr( image )
{}

void ImageButton::Draw( Graphics& gfx ) const
{
	Draw( gfx,false );
}

void ImageButton::Draw( Graphics& gfx,bool flipped ) const
{
	const auto drawCol = hovering ? col2 : col1;
	gfx.DrawRect( pos.x,pos.y,size.x,size.y,drawCol );

	gfx.DrawSprite( pos.x + padding.x / 4,
		pos.y + padding.y / 4,
		mySpr,SpriteEffect::Chroma{ Colors::Magenta },
		flipped );
}

AnimButton::AnimButton( const Vei2& center,Anim& myAnim )
	:
	ImageButton( center,
		Surface{ myAnim.GetFrameRect().GetWidth(),
		myAnim.GetFrameRect().GetHeight() } ),
	myAnim( myAnim )
{}

void AnimButton::Draw( Graphics& gfx,bool flipped ) const
{
	const auto drawCol = hovering ? col2 : col1;
	gfx.DrawRect( pos.x,pos.y,size.x,size.y,drawCol );
	
	myAnim.Draw( pos + padding / 4,gfx,
		SpriteEffect::Chroma{ Colors::Magenta },
		flipped );
}
