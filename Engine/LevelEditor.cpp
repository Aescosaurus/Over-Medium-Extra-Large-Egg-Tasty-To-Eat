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

void LevelEditor::Update( const Mouse& ms,const Keyboard& kbd )
{
	auto tempPos = ms.GetPos();

	while( tempPos.x >= Graphics::GameScreenWidth ) --tempPos.x;
	while( tempPos.x % tileSize.x != 0 ) --tempPos.x;
	while( tempPos.y % tileSize.y != 0 ) --tempPos.y;

	brushPos = tempPos;

	// if( kbd.KeyIsPressed( 'Q' ) ) brush = Tile2Char::Empty;
	// else if( kbd.KeyIsPressed( 'W' ) ) brush = Tile2Char::Wall;
	// else if( kbd.KeyIsPressed( 'E' ) ) brush = Tile2Char::Player;
	// else if( kbd.KeyIsPressed( 'A' ) ) brush = Tile2Char::Enemy;
	// else if( kbd.KeyIsPressed( 'S' ) ) brush = Tile2Char::Enemy;
	// else if( kbd.KeyIsPressed( 'D' ) ) brush = Tile2Char::Stairs;

	floor.Update( ms );
	wall.Update( ms );
	player.Update( ms );
	enemy.Update( ms );
	stairs.Update( ms );
	keyWall.Update( ms );
	key.Update( ms );

	if( floor.IsDown() ) brush = Tile2Char::Empty;
	else if( wall.IsDown() ) brush = Tile2Char::Wall;
	else if( player.IsDown() ) brush = Tile2Char::Player;
	else if( enemy.IsDown() ) brush = Tile2Char::Enemy;
	else if( stairs.IsDown() ) brush = Tile2Char::Stairs;
	else if( keyWall.IsDown() ) brush = Tile2Char::KeyWall;
	else if( key.IsDown() ) brush = Tile2Char::Key;

	if( ms.LeftIsPressed() && wndRect.ContainsPoint( ms.GetPos() ) )
	{
		const auto tSize = TileMap::GetTileSize();
		const auto realPos = Vei2{ brushPos.x / tSize.x,brushPos.y / tSize.y };
		PutTile( realPos.x,realPos.y,char( brush ) );
	}
}

void LevelEditor::Draw( Graphics& gfx ) const
{
	for( int y = 0; y < nTiles.y; ++y )
	{
		for( int x = 0; x < nTiles.x; ++x )
		{
			gfx.DrawSprite( x * tileSize.x,y * tileSize.y,
				floorSpr,
				SpriteEffect::Copy{} );

			const auto curTile = Tile2Char( GetTile( x,y ) );
			const Surface* const drawSurf = Tile2Surf( curTile );

			if( drawSurf != &floorSpr )
			{
				gfx.DrawSprite( x * tileSize.x,y * tileSize.y,
					*drawSurf,
					SpriteEffect::Chroma{ Colors::Magenta } );
			}
		}
	}

	backToMenu.Draw( gfx );
	floor.Draw( gfx );
	wall.Draw( gfx );
	player.Draw( gfx );
	enemy.Draw( gfx );
	stairs.Draw( gfx );
	keyWall.Draw( gfx );
	key.Draw( gfx );

	const Surface* const brushSpr = Tile2Surf( brush );
	gfx.DrawSprite( brushPos.x,brushPos.y,*brushSpr,
		SpriteEffect::Chroma{ Colors::Magenta } );
	gfx.DrawHitbox( brushSpr->GetRect().GetMovedBy( brushPos ),Colors::Green );

	// gfx.DrawLine( menuCenter + Vei2{ 0,1 },menuCenter + Vei2{ 0,Graphics::ScreenHeight - 1 },Colors::Red );
}

bool LevelEditor::CheckReturning( const Mouse& ms )
{
	backToMenu.Update( ms );

	return( backToMenu.IsDown() );
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
	const Surface* drawSurf = nullptr;

	switch( tileType )
	{
	case Tile2Char::Empty:
		return( &floorSpr );
	case Tile2Char::Wall:
		return( &wallSpr );
	case Tile2Char::Player:
		return( &playerSpr );
	case Tile2Char::Enemy:
		return( &enemySpr );
	case Tile2Char::Stairs:
		return( &stairsSpr );
	case Tile2Char::KeyWall:
		return( &keyWallSpr );
	case Tile2Char::Key:
		return( &keySpr );
	default:
		// You will never get this!
		assert( false );
		return( &floorSpr );
	}
}
