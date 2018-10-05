#include "EggSoldier.h"
#include "Random.h"
#include "SpriteEffect.h"

EggSoldier::EggSoldier( const Vec2& pos,const TileMap& map,
	const Collideable& coll,std::vector<Bullet>& bullets )
	:
	EnemyBase( pos,size,map,coll,5 ),
	myBullets( bullets ),
	shotTimer( 1.1f ),
	legsRunning( 0,16 * 4,size.x,size.y,8,*spriteSheet,0.2f )
{
	for( int i = 0; i < 5; ++i )
	{
		bodyCracking.emplace_back( Anim{ 32 * 4 * i,0,
			size.x,size.y,2,*spriteSheet,0.2f } );
	}
	UpdateTarget();
}

void EggSoldier::Update( const Vec2& playerPos,float dt )
{
	// When you reach the tile you're going for, retarget
	if( map.GetTilePos( pos ) == target || didCollide )
	{
		UpdateTarget();
	}

	didCollide = coll.CheckCollisionRect( pos,hitbox,vel * dt );
	hitbox.MoveTo( pos );

	shotTimer.Update( dt );
	if( shotTimer.IsDone() )
	{
		shotTimer.Reset( Random::RangeF( minShotTime,maxShotTime ) );

		myBullets.emplace_back( Bullet{ GetCenter(),playerPos,bulletSpeed } );
	}

	const auto rngOffset = Random::RangeF( 0.9f,1.1f );
	legsRunning.Update( dt * rngOffset );
	bodyCracking[bodyBreakIndex].Update( dt * rngOffset );
}

void EggSoldier::Draw( Graphics& gfx ) const
{
	const auto chromaEffect = SpriteEffect::Chroma{ Colors::Magenta };

	legsRunning.Draw( Vei2( pos ),gfx,
		chromaEffect,vel.x < 0.0f );
	bodyCracking[bodyBreakIndex].Draw( Vei2( pos ),gfx,
		chromaEffect,vel.x < 0.0f );
}

EggSoldier::EggSoldier( const EggSoldier& other )
	:
	EggSoldier( other.pos,other.map,other.coll,other.myBullets )
	// Sprite sheet should be the same pointer for everybody,
	//  so we won't have to set it here I think.
{
	*this = other;
}

EggSoldier& EggSoldier::operator=( const EggSoldier& other )
{
	target = other.target;
	vel = other.vel;
	didCollide = other.didCollide;
	shotTimer = other.shotTimer;
	legsRunning = other.legsRunning;
	bodyCracking = other.bodyCracking;
	bodyBreakIndex = other.bodyBreakIndex;
	
	return( *this );
}

void EggSoldier::Attack( const Vec2& whereFrom )
{
	EnemyBase::Attack( whereFrom );

	const int maxIndex = int( bodyCracking.size() - 1 );
	++bodyBreakIndex;
	if( bodyBreakIndex > maxIndex ) bodyBreakIndex = maxIndex;

	// UpdateTarget();
}

void EggSoldier::UpdateTarget()
{
	didCollide = false;

	int xSpot;
	int ySpot;
	do
	{
		xSpot = Random::RangeI( 0,map.GetWidth() - 1 );
		ySpot = Random::RangeI( 0,map.GetHeight() - 1 );
	} while( map.GetTile( xSpot,ySpot ) == TileMap::TileType::Wall );

	target = { xSpot,ySpot };

	vel = Vec2( map.GetWorldPos( target ) - pos )
		.GetNormalized() * speed;
}

Vec2 EggSoldier::GetCenter() const
{
	return( pos + Vec2( size ) / 2.0f );
}
