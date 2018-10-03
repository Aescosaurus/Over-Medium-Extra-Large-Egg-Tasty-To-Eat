#include "SpikeWall.h"
#include "SpriteEffect.h"

SpikeWall::SpikeWall( const Vei2& pos,Dir facingDir )
	:
	pos( pos ),
	myDir( facingDir ),
	shootSpikes( 0,0,size.x,size.y,6,*sprSheet,0.2f ),
	hitbox( pos,float( size.x ),float( size.y ) )
{
	if( myDir == Dir::Left )
	{
		hitbox.MoveTo( pos - Vec2( float( size.x ) / 2.0f,0.0f ) );
	}
	else
	{
		hitbox.MoveTo( pos );
	}
}

void SpikeWall::Update( const Rect& guyRect,float dt )
{
	if( hitbox.IsOverlappingWith( guyRect ) )
	{
		spiking = true;
	}

	if( spiking && !doneSpiking )
	{
		shootSpikes.Update( dt );
	
		if( shootSpikes.IsFinished() )
		{
			doneSpiking = true;
			shootSpikes.SetFrame( 5 );
		}
	}
}

void SpikeWall::Draw( Graphics& gfx ) const
{
	if( myDir == Dir::Left )
	{
		shootSpikes.Draw( pos - Vec2( float( size.x ) / 2.0f,0.0f ),gfx,
			SpriteEffect::Chroma{ Colors::Magenta },true );
	}
	else
	{
		shootSpikes.Draw( pos,gfx,
			SpriteEffect::Chroma{ Colors::Magenta },false );
	}

	// gfx.DrawHitbox( hitbox );
}

Vec2 SpikeWall::GetPos() const
{
	return( Vec2( pos ) );
}

const Rect& SpikeWall::GetRect() const
{
	return( hitbox );
}
