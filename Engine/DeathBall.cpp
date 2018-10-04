#include "DeathBall.h"
#include "Random.h"
#include "SpriteEffect.h"

DeathBall::DeathBall( const Vei2& pos )
	:
	pos( pos ),
	hitbox( pos,float( size.x ),float( size.y ) ),
	vel( 0.0f,0.0f ),
	rotate( 0,0,size.x,size.y,4,*sprSheet,0.2f )
{
}

void DeathBall::Update( const TileMap& map,float dt )
{
	pos += vel * speed * dt;

	const auto biggerHitbox = hitbox.GetExpanded( 2.0f );
	static constexpr auto wall = TileMap::TileType::Wall;
	if( map.GetTileAt( biggerHitbox.TopLeft() ) == wall ||
		map.GetTileAt( biggerHitbox.TopRight() ) == wall ||
		map.GetTileAt( biggerHitbox.BotLeft() ) == wall ||
		map.GetTileAt( biggerHitbox.BotRight() ) == wall )
	{
		RandomizeVel();
	}

	rotate.Update( dt );
}

void DeathBall::Draw( Graphics & gfx ) const
{
	rotate.Draw( Vei2( pos ),gfx,
		SpriteEffect::Chroma{ Colors::Magenta } );
}

void DeathBall::RandomizeVel()
{
	const auto dir = Random::RangeI( 0,10 ) > 5;
	if( dir )
	{
		vel.x = Random::RangeI( 0,10 ) > 5
			? -1.0f : 1.0f;
		vel.y = 0.0f;
	}
	else
	{
		vel.x = 0.0f;
		vel.y = Random::RangeI( 0,10 ) > 5
			? -1.0f : 1.0f;
	}
}
