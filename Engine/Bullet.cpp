#include "Bullet.h"
#include "SpriteEffect.h"

Bullet::Bullet( const Vec2& pos,const Vec2& target,
	float speed,Team myTeam )
	:
	pos( pos ),
	vel( ( target - pos ).GetNormalized() * speed ),
	hitbox( pos,float( size.x ),float( size.y ) ),
	// rotate( 0,( myTeam == Team::Player ) ? 0 : size.y,
	// 	size.x,size.y,4,*sprSheet,0.2f ),
	rotate( 0,size.x * int( myTeam ),size.x,size.y,
		4,*sprSheet,0.2f ),
	myTeam( myTeam )
{}

Bullet::Bullet( const Bullet& other )
	:
	Bullet( other.pos,Vec2{ -50.0f,-50.0f },9999.0f,myTeam )
{
	*this = other;
}

Bullet& Bullet::operator=( const Bullet& other )
{
	pos = other.pos;
	hitbox = other.hitbox;
	vel = other.vel;
	dead = other.dead;
	rotate = other.rotate;
	myTeam = other.myTeam;
	
	return( *this );
}

void Bullet::Update( const TileMap& map,float dt )
{
	pos += vel * dt;

	if( map.GetTileAt( pos ) == TileMap::TileType::Wall )
	{
		dead = true;
	}

	hitbox.MoveTo( GetCenter() );

	rotate.Update( dt );
}

void Bullet::Draw( Graphics& gfx ) const
{
	// if( hitbox.IsContainedBy( Graphics::GetScreenRect() ) )
	{
		const auto drawPos = Vei2( GetCenter() );
		// gfx.DrawRect( drawPos.x,drawPos.y,
		// 	size.x,size.y,Colors::Red );

		rotate.Draw( drawPos,gfx,
			SpriteEffect::Chroma{ Colors::Magenta } );

		// gfx.DrawHitbox( hitbox );
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

const Vec2& Bullet::GetPos() const
{
	return( pos );
}

const Rect& Bullet::GetRect() const
{
	return( hitbox );
}

void Bullet::MoveTo( const Vec2& movedTo )
{
	pos = movedTo;
}

Vec2 Bullet::GetCenter() const
{
	return( pos - Vec2( size ) / 2.0f );
}

SpellBullet::SpellBullet( const Vec2& pos )
	:
	Bullet( pos,pos,0.0f,Team::Spell ),
	t( cycleTime ),
	startPos( pos )
{}

void SpellBullet::Update( const TileMap& map,float dt )
{
	Bullet::Update( map,dt );

	Vec2 updatedPos = { amplitude.x * cos( t.GetPercent() *
		freq * deg2rad ) + startPos.x,
		amplitude.y * sin( t.GetPercent() *
		freq * 2.0f * deg2rad ) + startPos.y };

	t.Update( dt );
	if( t.IsDone() ) t.Reset();
}
