#include "Player.h"
#include "SpriteEffect.h"

Player::Player( const Vec2& pos,const Collideable& coll,
	std::vector<Bullet>& bullets )
	:
	pos( pos ),
	coll( coll ),
	hitbox( pos,float( size.x ),float( size.y ) ),
	myBullets( bullets ),
	shotTimer( refireTime ),
	walk( 0,0,size.x,size.y,4,*sprSheet,0.2f )
{}

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
		lookingLeft = true;
	}
	if( kbd.KeyIsPressed( 'D' ) )
	{
		++moveDir.x;
		lookingLeft = false;
	}
	moveDir = moveDir.GetNormalized() * speed * dt;

	if( moveDir.GetLengthSq() != 0.0f ) walk.Update( dt );
	else walk.SetFrame( 0 );

	// coll.CheckCollision( pos,moveDir );
	coll.CheckCollisionRect( pos,hitbox,moveDir );

	hitbox.MoveTo( GetCenter() );

	shotTimer.Update( dt );
	if( ( ms.LeftIsPressed() || kbd.KeyIsPressed( 'F' ) ) &&
		shotTimer.IsDone() )
	{
		shotTimer.Reset();

		myBullets.emplace_back( Bullet{ pos,
			Vec2( ms.GetPos() ),bulletSpeed,
			Bullet::Team::Player } );
	}
}

void Player::Draw( Graphics& gfx ) const
{
	const auto drawPos = Vei2( GetCenter() );
	// gfx.DrawRect( drawPos.x,drawPos.y,
	// 	size.x,size.y,Colors::Green );

	// gfx.DrawSprite( drawPos.x,drawPos.y,*mySpr,
	// 	SpriteEffect::Chroma{ Colors::Magenta } );

	walk.Draw( drawPos,gfx,
		SpriteEffect::Chroma{ Colors::Magenta },
		lookingLeft );
}

void Player::Attack( const Vec2& AttackSource )
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
