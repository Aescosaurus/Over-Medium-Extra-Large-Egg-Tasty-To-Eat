#pragma once

#include "Vec2.h"
#include "TileMap.h"

class Collideable
{
public:
	Collideable( const TileMap& tileMapRef,Graphics& gfx );
	bool CheckCollision( Vec2& posToTest,
		const Vec2& moveDir ) const;
	bool CheckCollisionRect( Vec2& posRef,
		const Rect& rectToTest,const Vec2& moveDir ) const;
private:
	const TileMap& map;
	Graphics& gfx;
};