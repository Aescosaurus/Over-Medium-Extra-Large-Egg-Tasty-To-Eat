#include "Player.h"
#include "SpriteEffect.h"

Player::Player( const Vec2& pos,const Collideable& coll,
	std::vector<Bullet>& bullets )
	:
	pos( pos ),
	coll( coll ),
	hitbox( pos,float( size.x ),float( size.y ) ),
	myBullets( bullets ),
	shotTimer( refireTime )
{
}

void Player::Update( const Keyboard& kbd,const Mouse& ms,
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

	shotTimer.Update( dt );
	if( ms.LeftIsPressed() && shotTimer.IsDone() )
	{
		shotTimer.Reset();

		myBullets.emplace_back( Bullet{ pos,
			Vec2( ms.GetPos() ),bulletSpeed } );
	}
}

void Player::Draw( Graphics& gfx ) const
{
	const auto drawPos = Vei2( GetCenter() );
	// gfx.DrawRect( drawPos.x,drawPos.y,
	// 	size.x,size.y,Colors::Green );
	gfx.DrawSprite( drawPos.x,drawPos.y,*mySpr,
		SpriteEffect::Chroma{ Colors::Magenta } );
}

void Player::Attack()
{
}

void Player::SetTopLeft( const Vec2& newPos )
{
	pos = newPos;

	hitbox.MoveTo( pos );
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
