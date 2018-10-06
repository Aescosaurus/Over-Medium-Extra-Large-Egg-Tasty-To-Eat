#pragma once

#include <vector>
#include <string>
#include "Graphics.h"
#include <string>
#include "Surface.h"
#include "Codex.h"

class TileMap
{
public:
	enum class TileType
	{
		Empty,
		Wall
	};
	// If you modify this make sure to update in LoadFile.
	enum class Token
	{
		Player = int( 'P' ),
		Enemy = int( 'E' ),
		Stairs = int( 'S' ),
		KeyWall = int( 'L' ),
		Key = int( 'K' ),
		SpikeWallLeft = int( 'T' ),
		SpikeWallRight = int( 'Y' ),
		DeathBall = int( 'B' )
	};
public:
	TileMap( const std::string& fileName );

	void Draw( Graphics& gfx ) const;
	void DrawTileAt( int x,int y,Color c,Graphics& gfx ) const;

	void SetTile( int x,int y,TileType type );
	void LoadFile( const std::string& fileName );
	std::string GetNextLvlName();

	std::vector<Vei2> FindAllInstances( const std::string& fileName,Token search ) const;
	Vei2 FindFirstInstance( const std::string& fileName,Token search ) const;
	int CountNLevels( const std::string& name ) const;

	TileType GetTile( int x,int y ) const;
	Vei2 GetTilePos( const Vec2& screenPos ) const;
	Vec2 GetWorldPos( const Vei2& mapPos ) const;
	TileType GetTileAt( const Vec2& screenPos ) const;
	const std::vector<TileType>& GetTiles() const;
	int GetWidth() const;
	int GetHeight() const;
	int GetTileNum() const;
	static constexpr Vei2 GetTileSize() { return( tileDim ); }
	std::string GetRandLvlName() const;
public:
	static constexpr Vei2 nTiles = { 20,15 };
private:
	int width; // Number of tiles left-right.
	int height; // Number of tiles up-down.
	// Vei2 tileDim; // Size of each tile on screen.
	static constexpr Vei2 tileDim = { 40,40 };
	std::vector<TileType> tiles;
	int curLevel = 0;
	const Surface* const wallSpr = Codex<Surface>::RetrieveSurf( "Images/Wall1.bmp",{ 4,4 } );
	const Surface* const groundSpr = Codex<Surface>::RetrieveSurf( "Images/Floor.bmp",{ 4,4 } );
};