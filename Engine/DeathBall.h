#pragma once

#include "Vec2.h"
#include "TileMap.h"
#include "Rect.h"
#include "Codex.h"
#include "Anim.h"

class DeathBall
{
public:
	DeathBall( const Vei2& pos );

	void Update( const TileMap& map,float dt );
	void Draw( Graphics& gfx ) const;
private:
	void RandomizeVel();
private:
	Vec2 pos;
	static constexpr Vei2 size = { 32,32 };
	Rect hitbox;
	Vec2 vel;
	static constexpr float speed = 50.0f;
	const Surface* const sprSheet = Codex<Surface>::RetrieveSurf( "Images/DeathBallAnim.bmp",size );
	Anim rotate;
};