#include "LevelEditor.h"
#include <cassert>
#include "SpriteEffect.h"

LevelEditor::LevelEditor()
{
	const auto nTotalTiles = nTiles.x * nTiles.y;
	tiles.reserve( nTotalTiles );
	for( int i = 0; i < nTotalTiles; ++i )
	{
		tiles.emplace_back( char( Tile2Char::Empty ) );
	}
}

void LevelEditor::Update( const Mouse& ms )
{
	auto tempPos = ms.GetPos();

	while( tempPos.x % tileSize.x != 0 ) --tempPos.x;
	while( tempPos.y % tileSize.y != 0 ) --tempPos.y;

	brushPos = tempPos;
}

void LevelEditor::Draw( Graphics& gfx ) const
{
	for( int y = 0; y < nTiles.y; ++y )
	{
		for( int x = 0; x < nTiles.x; ++x )
		{
			const auto curTile = Tile2Char( GetTile( x,y ) );

			const Surface* const drawSurf = Tile2Surf( curTile );

			gfx.DrawSprite( x * tileSize.x,y * tileSize.y,
				*drawSurf,
				SpriteEffect::Chroma{ Colors::Magenta } );
		}
	}

	const Surface* const brushSpr = Tile2Surf( brush );
	gfx.DrawSprite( brushPos.x,brushPos.y,*brushSpr,
		SpriteEffect::Chroma{ Colors::Magenta } );
}

void LevelEditor::PutTile( int x,int y,char c )
{
	tiles[y * nTiles.x + x] = c;
}

char LevelEditor::GetTile( int x,int y ) const
{
	return( tiles[y * nTiles.x + x] );
}

const Surface* const LevelEditor::Tile2Surf( Tile2Char tileType ) const
{
	const Surface* drawSurf;

	switch( tileType )
	{
	case Tile2Char::Empty:
		drawSurf = &floorSpr;
		break;
	case Tile2Char::Wall:
		drawSurf = &wallSpr;
		break;
	case Tile2Char::Player:
		drawSurf = &playerSpr;
		break;
	case Tile2Char::Enemy:
		drawSurf = &enemySpr;
		break;
	case Tile2Char::Stairs:
		drawSurf = &stairsSpr;
		break;
	case Tile2Char::KeyWall:
		drawSurf = &keyWallSpr;
		break;
	case Tile2Char::Key:
		drawSurf = &keySpr;
		break;
	default:
		// You will never get this!
		assert( false );
		break;
	}

	return( drawSurf );
}
