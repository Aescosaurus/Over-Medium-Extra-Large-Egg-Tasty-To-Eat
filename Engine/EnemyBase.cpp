#include "EnemyBase.h"

void EnemyBase::Attack( const Vec2& whereFrom )
{
	// TODO: Make enemy get pushed back when attacked.
	--hp;
}

const Vec2& EnemyBase::GetPos() const
{
	return( pos );
}

const Rect& EnemyBase::GetRect() const
{
	return( hitbox );
}

bool EnemyBase::IsDead() const
{
	return( hp < 1 );
}

EnemyBase::EnemyBase( const Vec2& pos,const Vei2& size,
	const TileMap& map,const Collideable& coll,
	int hp )
	:
	pos( pos ),
	hitbox( pos,float( size.x ),float( size.y ) ),
	coll( coll ),
	map( map ),
	hp( hp )
{}

EnemyBase::EnemyBase( const EnemyBase& other )
	:
	coll( other.coll ),
	map( other.map )
{}

EnemyBase& EnemyBase::operator=( const EnemyBase& other )
{
	pos = other.pos;
	hitbox = other.hitbox;
	hp = other.hp;

	return( *this );
}
