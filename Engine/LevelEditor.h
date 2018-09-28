#pragma once

#include <vector>
#include "Graphics.h"
#include "TileMap.h"
#include "Surface.h"
#include "Mouse.h"

class LevelEditor
{
private:
	enum class Tile2Char
	{
		Empty = int( '0' ),
		Wall = int( '1' ),
		Player = int( 'P' ),
		Enemy = int( 'E' ),
		Stairs = int( 'S' ),
		KeyWall = int( 'L' ),
		Key = int( 'K' )
	};
public:
	LevelEditor();

	void Update( const Mouse& ms );
	void Draw( Graphics& gfx ) const;
private:
	void PutTile( int x,int y,char c );

	char GetTile( int x,int y ) const;
	const Surface* const Tile2Surf( Tile2Char tileType ) const;
private:
	std::vector<char> tiles;
	static constexpr Vei2 nTiles = TileMap::nTiles;
	static constexpr Vei2 tileSize = TileMap::GetTileSize();
	Tile2Char brush = Tile2Char::Wall;
	Vei2 brushPos = { 0,0 };
	const Surface floorSpr = { "Images/Floor.bmp",40,40 };
	const Surface wallSpr = { "Images/Wall1.bmp",40,40 };
	const Surface playerSpr = { "Images/Player.bmp",32,32 };
	const Surface enemySpr = { "Images/EggEnemyAnim.bmp",RectI{ 0,0,16,16 } };
	const Surface stairsSpr = { "Images/Stairs.bmp",32,32 };
	const Surface keyWallSpr = { "Images/KeyWall.bmp",40,40 };
	const Surface keySpr = { "Images/Key.bmp",32,32 };
};