#pragma once

#include "Vec2.h"
#include "TileMap.h"
#include "Graphics.h"
#include "Surface.h"
#include "Codex.h"

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
	const Surface* const wallSpr = Codex<Surface>::RetrieveSurf( "Images/KeyWall.bmp",{ 4,4 } );
};