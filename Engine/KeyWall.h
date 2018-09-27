#pragma once

#include "Vec2.h"
#include "TileMap.h"
#include "Graphics.h"
#include "Surface.h"

class KeyWall
{
public:
	KeyWall( const Vei2& pos,TileMap& map );

	void Draw( Graphics& gfx ) const;

	void Unlock();
private:
	const Vei2 pos;
	static constexpr Vei2 size = TileMap::GetTileSize();
	bool unlocked = false;
	TileMap& myTileMap;
	static const Surface wallSpr;
};