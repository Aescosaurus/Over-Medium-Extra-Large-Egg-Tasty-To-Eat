#include "EggSpellcaster.h"
#include "SpriteEffect.h"

EggSpellcaster::EggSpellcaster( const Vec2& pos,const TileMap& map,
	const Collideable& coll,std::vector<std::unique_ptr<Bullet>>& bullets )
	:
	EnemyBase( pos,size,map,coll,myHP ),
	spellBullets( bullets ),
	bodyCrack( 0,0,size.x,size.y,4,*sprSheet,1.0f ),
	legs( 0,size.y,size.x,size.y,2,*sprSheet,0.2f ),
	shotTimer( refireTime )
{}

void EggSpellcaster::Update( const Vec2& playerPos,float dt )
{
	shotTimer.Update( dt );

	if( shotTimer.IsDone() )
	{
		shotTimer.Reset();

		spellBullets.emplace_back( new SpellBullet{ GetCenter() } );
	}

	// hitbox.MoveTo( pos );
}

void EggSpellcaster::Draw( Graphics& gfx ) const
{
	const auto chroma = SpriteEffect::Chroma{ Colors::Magenta };
	bodyCrack.Draw( pos,gfx,chroma );

	legs.Draw( pos,gfx,chroma );

	gfx.DrawHitbox( hitbox );
}

Vec2 EggSpellcaster::GetCenter() const
{
	return( pos + Vec2( size ) / 2.0f );
}
