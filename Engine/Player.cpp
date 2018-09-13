#include "Player.h"

Player::Player( const Vec2& pos,const Collideable& coll )
	:
	pos( pos ),
	coll( coll ),
	hitbox( pos,float( size.x ),float( size.y ) )
{
}

void Player::Update( const Keyboard& kbd,
	const TileMap& map,const Collideable& coll,float dt )
{
	Vec2 moveDir = { 0.0f,0.0f };
	if( kbd.KeyIsPressed( 'W' ) )
	{
		--moveDir.y;
	}
	if( kbd.KeyIsPressed( 'S' ) )
	{
		++moveDir.y;
	}
	if( kbd.KeyIsPressed( 'A' ) )
	{
		--moveDir.x;
	}
	if( kbd.KeyIsPressed( 'D' ) )
	{
		++moveDir.x;
	}
	moveDir = moveDir.GetNormalized() * speed * dt;

	// coll.CheckCollision( pos,moveDir );
	coll.CheckCollisionRect( pos,hitbox,moveDir );

	hitbox.MoveTo( GetCenter() );
}

void Player::Draw( Graphics& gfx ) const
{
	const auto drawPos = Vei2( GetCenter() );
	gfx.DrawRect( drawPos.x,drawPos.y,
		size.x,size.y,Colors::Red );
}

void Player::Attack()
{
}

const Vec2& Player::GetPos() const
{
	return( pos );
}

const Rect& Player::GetRect() const
{
	return( hitbox );
}

const Vec2 Player::GetCenter() const
{
	return( Vei2( pos - Vec2( size ) / 2.0f ) );
}
