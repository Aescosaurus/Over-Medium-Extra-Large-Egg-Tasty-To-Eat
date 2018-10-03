#pragma once

#include "Vec2.h"
#include "TileMap.h"
#include "Rect.h"

class DeathBall
{
public:
	DeathBall( const Vei2& pos );

	void Update( const TileMap& map,float dt );
	void Draw( Graphics& gfx ) const;
private:
	Vec2 pos;
	static constexpr Vei2 size = { 32,32 };
	Rect hitbox;
};