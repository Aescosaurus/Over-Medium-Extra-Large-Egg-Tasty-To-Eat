#pragma once

#include "Mouse.h"
#include "Graphics.h"
#include <vector>
#include "TileMap.h"
#include "Surface.h"
#include "Button.h"
#include "Codex.h"
#include "Font.h"
#include <map>

class LevelEditor
{
private:
	enum class TileType
	{
		Empty = int( '0' ),
		Wall = int( '1' ),
		Player = int( 'P' ),
		Stairs = int( 'S' ),

		EggSoldier = int( 'E' ),
		Eggsploder = int( 'G' ),
		// other stuff
		// other stuff

		KeyWall = int( 'L' ),
		Key = int( 'K' ),
		SpikeWallLeft = int( 'Y' ),
		SpikeWallRight = int( 'T' )
	};
public:
	LevelEditor();

	void Update( const Mouse& ms );
	void Draw( Graphics& gfx ) const;

	bool CheckReturning( const Mouse& ms );
private:
	void PutTile( int x,int y,TileType t );

	TileType GetTile( int x,int y ) const;
private:
	std::vector<TileType> tiles;
	static constexpr Vei2 nTiles = TileMap::nTiles;
	static constexpr Vei2 tileSize = TileMap::GetTileSize();
	TileType brush = TileType::Wall;
	Vei2 brushPos = { 0,0 };

	std::map<TileType,const Surface> surfs;

	static constexpr Vei2 start = { Graphics::GameScreenWidth,64 };
	// std::vector<ImageButton> buttons;
	std::map<TileType,ImageButton> buttons;

	// TODO: Put this in graphics as Graphics::GetWndRect()
	static constexpr RectI wndRect = { { 0,0 },Graphics::GameScreenWidth,Graphics::GameScreenHeight };

	float fadeProgress = 0.0f;
	static constexpr float fadeSpeed = 0.0076f;
	const Font* const textFont = Codex<Font>::Retrieve( "Fonts/LuckyPixel24x36.bmp" );
};