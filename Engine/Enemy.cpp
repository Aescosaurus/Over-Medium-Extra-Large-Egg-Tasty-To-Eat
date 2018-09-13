#include "Enemy.h"
#include "Random.h"
#include "Collideable.h"
#include "Random.h"

Enemy::Enemy( const Vec2& pos,const TileMap& map,
	const Collideable& coll,std::vector<Bullet>& bullets )
	:
	pos( pos ),
	coll( coll ),
	hitbox( pos,float( size.x ),float( size.y ) ),
	myBullets( bullets ),
	shotTimer( 1.1f ),
	tilemap( map )
{
	UpdateTarget( tilemap );
}

Enemy::Enemy( const Enemy& other )
	:
	Enemy( other.pos,other.tilemap,other.coll,other.myBullets )
{
	*this = other;
}

Enemy& Enemy::operator=( const Enemy& other )
{
	pos = other.pos;
	target = other.target;
	vel = other.vel;
	hitbox = other.hitbox;
	didCollide = other.didCollide;
	shotTimer = other.shotTimer;
	hp = other.hp;

	return( *this );
}

void Enemy::Update( const TileMap& map,
	const Vec2& playerPos,float dt )
{
	// When you reach the tile you're going for, retarget
	if( map.GetTilePos( pos ) == target || didCollide )
	{
		UpdateTarget( tilemap );
	}

	// didCollide = coll.CheckCollision( pos,vel * dt );
	didCollide = coll.CheckCollisionRect( pos,hitbox,vel * dt );
	hitbox.MoveTo( pos );

	shotTimer.Update( dt );
	if( shotTimer.IsDone() )
	{
		shotTimer.Reset( Random::RangeF( minShotTime,maxShotTime ) );

		myBullets.emplace_back( Bullet{ GetCenter(),playerPos,bulletSpeed } );
	}
}

void Enemy::Draw( Graphics& gfx ) const
{
	gfx.DrawRect( int( pos.x ),int( pos.y ),
		size.x,size.y,Colors::Red );
}

void Enemy::Attack()
{
	--hp;
}

const Rect& Enemy::GetRect() const
{
	return( hitbox );
}

bool Enemy::IsDead() const
{
	return( hp < 1 );
}

void Enemy::UpdateTarget( const TileMap& map )
{
	didCollide = false;

	int xSpot;
	int ySpot;
	do
	{
		xSpot = Random::RangeI( 0,map.GetWidth() - 1 );
		ySpot = Random::RangeI( 0,map.GetHeight() - 1 );
	}
	while( map.GetTile( xSpot,ySpot ) == TileMap::TileType::Wall );

	target = { xSpot,ySpot };

	vel = Vec2( map.GetWorldPos( target ) - pos )
		.GetNormalized() * speed;
}

Vec2 Enemy::GetCenter() const
{
	return( pos + Vec2( size ) / 2.0f );
}
