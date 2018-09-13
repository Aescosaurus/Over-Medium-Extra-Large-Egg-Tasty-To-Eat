#pragma once

#include <vector>
#include <string>
#include "Graphics.h"

class TileMap
{
public:
	enum class TileType
	{
		Empty,
		Wall
	};
public:
	TileMap( const std::string& fileName );

	void Draw( Graphics& gfx ) const;
	void DrawTileAt( int x,int y,Color c,Graphics& gfx ) const;

	void SetTile( int x,int y,TileType type );
	void LoadFile( const std::string& fileName );
	std::vector<Vei2> GetEnemies( const std::string& fileName ) const;

	TileType GetTile( int x,int y ) const;
	Vei2 GetTilePos( const Vec2& screenPos ) const;
	Vec2 GetWorldPos( const Vei2& mapPos ) const;
	TileType GetTileAt( const Vec2& screenPos ) const;
	const std::vector<TileType>& GetTiles() const;
	int GetWidth() const;
	int GetHeight() const;
	int GetTileNum() const;
private:
	int width; // Number of tiles left-right.
	int height; // Number of tiles up-down.
	Vei2 tileDim; // Size of each tile on screen.
	std::vector<TileType> tiles;
};