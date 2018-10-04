#pragma once

#include "Vec2.h"
#include "TileMap.h"
#include "Rect.h"
#include "Codex.h"
#include "Anim.h"
#include "Collideable.h"

class DeathBall
{
public:
	DeathBall( const Vei2& pos );

	void Update( const TileMap& map,const Collideable& coll,float dt );
	void Draw( Graphics& gfx ) const;
private:
	void RandomizeVel();
private:
	Vec2 pos;
	static constexpr Vei2 size = { 32,32 };
	Rect hitbox;
	Vec2 vel;
	static constexpr float speed = 140.0f;
	const Surface* const sprSheet = Codex<Surface>::RetrieveSurf( "Images/DeathBallAnim.bmp",{ size.x * 4,size.y } );
	Anim rotate;
};