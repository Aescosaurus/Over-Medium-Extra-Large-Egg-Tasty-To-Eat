#include "Stairs.h"
#include "SpriteEffect.h"

Stairs::Stairs( const Vec2& pos )
	:
	pos( pos ),
	hitbox( pos,float( size.x ),float( size.y ) )
{
	Despawn();
}

void Stairs::Draw( Graphics& gfx ) const
{
	if( activated )
	{
		// gfx.DrawRect( int( pos.x ),int( pos.y ),
		// 	size.x,size.y,Colors::Cyan );

		gfx.DrawSprite( int( pos.x ),int( pos.y ),
			mySpr,SpriteEffect::Copy{} );

		// gfx.DrawHitbox( hitbox );
	}
}

void Stairs::Spawn( const Vec2& whereTo )
{
	pos = whereTo;
	hitbox.MoveTo( whereTo );
	activated = true;
}

void Stairs::Despawn()
{
	activated = false;
	pos = { -50.0f,-50.0f };
}

void Stairs::Activate()
{
	activated = true;
}

void Stairs::Deactivate()
{
	activated = false;
}

const Rect& Stairs::GetRect() const
{
	return( hitbox );
}

bool Stairs::IsActive() const
{
	return( activated );
}
