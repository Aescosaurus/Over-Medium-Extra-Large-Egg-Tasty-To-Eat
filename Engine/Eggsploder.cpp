#include "Eggsploder.h"
#include "SpriteEffect.h"

Eggsploder::Eggsploder( const Vec2& pos,const TileMap& map,
	const Collideable& coll )
	:
	EnemyBase( pos,size,map,coll,myHP ),
	legsRunning( 0,size.y,size.x,size.y,4,*sprSheet,0.12f ),
	bodyBreaking( 0,0,size.x,size.y,4,*sprSheet,1.0f ),
	retarget( retargetTime ),
	vel( 0.0f,0.0f ) // very important lol
{}

Eggsploder::Eggsploder( const Eggsploder& other )
	:
	Eggsploder( other.pos,other.map,other.coll )
{
	*this = other;
}

Eggsploder& Eggsploder::operator=( const Eggsploder& other )
{
	legsRunning = other.legsRunning;
	bodyBreaking = other.bodyBreaking;
	bodyCrackIndex = other.bodyCrackIndex;
	retarget = other.retarget;
	vel = other.vel;
	
	return( *this );
}

void Eggsploder::Update( const Vec2& playerPos,float dt )
{
	retarget.Update( dt );
	if( retarget.IsDone() )
	{
		retarget.Reset();
		const auto diff = playerPos - pos;
		vel = diff.GetNormalized() * speed;
	}
	
	const auto moveTest = vel * dt;
	// pos += moveTest;

	coll.CheckCollisionRect( pos,hitbox,moveTest );

	hitbox.MoveTo( pos );

	legsRunning.Update( dt );
}

void Eggsploder::Draw( Graphics& gfx ) const
{
	// gfx.DrawRect( int( pos.x ),int( pos.y ),
	// 	size.x,size.y,Colors::Red );
	
	const auto chroma = SpriteEffect::Chroma{ Colors::Magenta };
	const auto isFlipped = ( vel.x < 0.0f );
	
	bodyBreaking.Draw( Vei2( pos ),gfx,
		chroma,isFlipped );
	
	legsRunning.Draw( pos,gfx,
		chroma,isFlipped );

	// gfx.DrawHitbox( hitbox );
}

void Eggsploder::Attack( const Vec2& whereFrom )
{
	EnemyBase::Attack( whereFrom );
	bodyBreaking.Update( 1.0f );

	const auto diff = whereFrom - pos;
	pos -= diff.GetNormalized() * knockbackPower;
}