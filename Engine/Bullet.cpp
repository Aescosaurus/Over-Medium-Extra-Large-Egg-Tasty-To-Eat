#include "Bullet.h"

Bullet::Bullet( const Vec2& pos,const Vec2& target,float speed )
	:
	pos( pos ),
	vel( ( target - pos ).GetNormalized() * speed ),
	hitbox( pos,float( size.x ),float( size.y ) )
{
}

void Bullet::Update( const TileMap& map,float dt )
{
	pos += vel * dt;

	if( map.GetTileAt( pos ) == TileMap::TileType::Wall )
	{
		dead = true;
	}

	hitbox.MoveTo( pos - Vec2( size ) / 2.0f );
}

void Bullet::Draw( Graphics& gfx ) const
{
	if( hitbox.IsContainedBy( Graphics::GetScreenRect() ) )
	{
		const auto drawPos = Vei2( pos - Vec2( size ) / 2.0f );
		gfx.DrawRect( drawPos.x,drawPos.y,
			size.x,size.y,Colors::Red );
	}
}

void Bullet::Kill()
{
	dead = true;
}

bool Bullet::IsDead() const
{
	return( dead );
}

const Rect& Bullet::GetRect() const
{
	return( hitbox );
}
