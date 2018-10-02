#include "Enemy.h"
#include "Random.h"
#include "Collideable.h"
#include "Random.h"
#include "SpriteEffect.h"

// const Surface Enemy::spriteSheet( "Images/EggEnemyAnim.bmp",128 * 4,16 * 4 );
const Surface Enemy::spriteSheet( "Images/EggEnemyAnim.bmp",160 * 4,32 * 4 );

Enemy::Enemy( const Vec2& pos,const TileMap& map,
	const Collideable& coll,std::vector<Bullet>& bullets )
	:
	pos( pos ),
	coll( coll ),
	hitbox( pos,float( size.x ),float( size.y ) ),
	myBullets( bullets ),
	shotTimer( 1.1f ),
	tilemap( map ),
	// running( 0,0,size.x,size.y,8,spriteSheet,0.2f,Colors::Magenta ),
	legsRunning( 0,16 * 4,size.x,size.y,8,spriteSheet,0.2f,Colors::Magenta )
{
	for( int i = 0; i < 5; ++i )
	{
		// bodyCracking( 0,0,size.x,size.y,10,spriteSheet,1.0f,Colors::Magenta )
		bodyCracking.emplace_back( Anim{ 32 * 4 * i,0,
			size.x,size.y,2,spriteSheet,0.2f } );
	}
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
	// running = other.running;
	legsRunning = other.legsRunning;
	bodyCracking = other.bodyCracking;

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

	// running.Update( dt * Random::RangeF( 0.9,1.1 ) );
	const auto rngOffset = Random::RangeF( 0.9f,1.1f );
	legsRunning.Update( dt * rngOffset );
	bodyCracking[bodyBreakIndex].Update( dt * rngOffset );
}

void Enemy::Draw( Graphics& gfx ) const
{
	// running.Draw( Vei2( pos ),gfx,
	// 	SpriteEffect::Chroma{ Colors::Magenta },
	// 	vel.x < 0.0f );

	const auto chromaEffect = SpriteEffect::Chroma{ Colors::Magenta };
	legsRunning.Draw( Vei2( pos ),gfx,
		chromaEffect,vel.x < 0.0f );
	bodyCracking[bodyBreakIndex].Draw( Vei2( pos ),gfx,
		chromaEffect,vel.x < 0.0f );

	// gfx.DrawHitbox( hitbox );
}

void Enemy::Attack()
{
	--hp;
	++bodyBreakIndex;
}

const Vec2& Enemy::GetPos() const
{
	return( pos );
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
