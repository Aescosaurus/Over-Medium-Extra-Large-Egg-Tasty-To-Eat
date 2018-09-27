#include "Key.h"
#include "SpriteEffect.h"

const Surface Key::spr = Surface{ "Images/Key.bmp",size.x,size.y };

Key::Key( const Vei2& pos )
	:
	pos( pos ),
	hitbox( pos,size.x,size.y )
{}

void Key::Draw( Graphics& gfx ) const
{
	if( active )
	{
		// gfx.DrawRect( pos.x,pos.y,size.x,size.y,Colors::Blue );
		gfx.DrawSprite( pos.x,pos.y,spr,
			SpriteEffect::Chroma( Colors::Magenta ) );
	}
}

void Key::SpawnAt( const Vei2& pos )
{
	active = true;
	this->pos = pos;
	hitbox.MoveTo( pos );
}

void Key::Despawn()
{
	active = false;
	pos = { -50,-50 };
}

const RectI& Key::GetRect() const
{
	return( hitbox );
}
