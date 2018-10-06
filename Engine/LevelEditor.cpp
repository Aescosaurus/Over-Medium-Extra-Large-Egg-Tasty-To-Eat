#include "LevelEditor.h"
#include <cassert>
#include "SpriteEffect.h"
#include <fstream>
#include "FrameTimer.h"

int LevelEditor::nInstances = 0;

LevelEditor::LevelEditor()
	:
	spikeWallAnim( 0,0,10 * 4,10 * 4,5,spikeWallSpr,0.2f ),
	deathBallAnim( 0,0,8 * 4,8 * 4,4,deathBallSpr,0.2f )
{
	// Enforce singleton-ness.
	++nInstances;
	assert( nInstances == 1 );

	const auto nTotalTiles = nTiles.x * nTiles.y;
	tiles.reserve( nTotalTiles );
	// for( int i = 0; i < nTotalTiles; ++i )
	// {
	// 	tiles.emplace_back( char( Tile2Char::Empty ) );
	// }

	for( int y = 0; y < nTiles.y; ++y )
	{
		for( int x = 0; x < nTiles.x; ++x )
		{
			auto fill = Tile2Char::Empty;
			if( x == 0 || y == 0 ||
				x == nTiles.x - 1 || y == nTiles.y - 1 )
			{
				fill = Tile2Char::Wall;
			}
			tiles.emplace_back( fill );
		}
	}
}

void LevelEditor::Update( const Mouse& ms,const Keyboard& kbd )
{
	const float dt = FrameTimer::Mark();

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

	saveMap.Update( ms );

	floor.Update( ms );
	wall.Update( ms );
	player.Update( ms );
	enemy.Update( ms );
	stairs.Update( ms );
	keyWall.Update( ms );
	key.Update( ms );
	deathBall.Update( ms );
	lSpikeWall.Update( ms );
	rSpikeWall.Update( ms );
	eggsploder.Update( ms );

	if( saveMap.IsDown() ) PublishLevel();
	else if( floor.IsDown() ) brush = Tile2Char::Empty;
	else if( wall.IsDown() ) brush = Tile2Char::Wall;
	else if( player.IsDown() ) brush = Tile2Char::Player;
	else if( enemy.IsDown() ) brush = Tile2Char::Enemy;
	else if( stairs.IsDown() ) brush = Tile2Char::Stairs;
	else if( keyWall.IsDown() ) brush = Tile2Char::KeyWall;
	else if( key.IsDown() ) brush = Tile2Char::Key;
	else if( deathBall.IsDown() ) brush = Tile2Char::DeathBall;
	else if( lSpikeWall.IsDown() ) brush = Tile2Char::SpikeWallLeft;
	else if( rSpikeWall.IsDown() ) brush = Tile2Char::SpikeWallRight;
	else if( eggsploder.IsDown() ) brush = Tile2Char::Eggsploder;

	if( ms.LeftIsPressed() && wndRect.ContainsPoint( ms.GetPos() ) )
	{
		static constexpr auto tSize = TileMap::GetTileSize();
		const auto realPos = Vei2{ brushPos.x / tSize.x,brushPos.y / tSize.y };

		const auto tileOffset = GetTileOffset( brush );

		PutTile( realPos.x + tileOffset.x,
			realPos.y + tileOffset.y,
			brush );
	}

	if( fadeProgress > 0.0f )
	{
		fadeProgress -= fadeSpeed * dt * 60.0f;
		if( fadeProgress < 0.0f ) fadeProgress = 0.0f;
	}

	spikeWallAnim.Update( dt );
	deathBallAnim.Update( dt );
}

void LevelEditor::Draw( Graphics& gfx ) const
{
	struct AnimToDraw
	{
		const Anim& theAnim;
		const Vei2 pos;
		const bool flipped;
	};
	std::vector<AnimToDraw> animsToDraw;

	const auto chroma = SpriteEffect::Chroma{ Colors::Magenta };
	const auto copy = SpriteEffect::Copy{};

	// This is probably inefficient and slow but is easier
	//  than the alternative and won't matter in release.
	for( int y = 0; y < nTiles.y; ++y )
	{
		for( int x = 0; x < nTiles.x; ++x )
		{
			gfx.DrawSprite( x * tileSize.x,y * tileSize.y,
				floorSpr,copy,false );
		}
	}

	for( int y = 0; y < nTiles.y; ++y )
	{
		for( int x = 0; x < nTiles.x; ++x )
		{
			// bool drawingFloor = true;
			bool drawingWall = false;
			// gfx.DrawSprite( x * tileSize.x,y * tileSize.y,
			// 	floorSpr,
			// 	SpriteEffect::Copy{} );

			const auto curTile = Tile2Char( GetTile( x,y ) );
			const auto tileOffset = -GetTileOffset( curTile ) * 40;

			if( IsAnim( curTile ) )
			{
				const auto& theAnim = Tile2Anim( curTile );

				if( tileOffset.x != 0 || tileOffset.y != 0 )
				{
					animsToDraw.emplace_back( AnimToDraw{ theAnim,
						Vei2{ ( x * tileSize.x ) + tileOffset.x,
						( y * tileSize.y ) + tileOffset.y },
						IsFlipped( curTile ) } );

					// drawingFloor = false;
					drawingWall = true;
				}
				else
				{
					theAnim.Draw( Vei2{ ( x * tileSize.x ) + tileOffset.x,
						( y * tileSize.y ) + tileOffset.y },
						gfx,chroma,IsFlipped( curTile ) );
				}
			}
			else
			{
				const Surface* const drawSurf = Tile2Surf( curTile );

				if( drawSurf != &floorSpr )
				{
					// drawingFloor = false;
					drawingWall = false;

					gfx.DrawSprite( ( x * tileSize.x ) + tileOffset.x,
						( y * tileSize.y ) + tileOffset.y,
						*drawSurf,
						chroma,
						IsFlipped( curTile ) );
				}
			}
			
			if( drawingWall )
			{
				gfx.DrawSprite( x * tileSize.x,y * tileSize.y,
					wallSpr,
					SpriteEffect::Copy{} );
			}
		}
	}

	for( const auto& theAnimThing : animsToDraw )
	{
		theAnimThing.theAnim.Draw( theAnimThing.pos,
			gfx,chroma,
			theAnimThing.flipped );
	}

	backToMenu.Draw( gfx );
	saveMap.Draw( gfx );
	floor.Draw( gfx );
	wall.Draw( gfx );
	player.Draw( gfx );
	enemy.Draw( gfx );
	stairs.Draw( gfx );
	keyWall.Draw( gfx );
	key.Draw( gfx );
	deathBall.Draw( gfx,false );
	lSpikeWall.Draw( gfx,false );
	rSpikeWall.Draw( gfx,true );
	eggsploder.Draw( gfx );

	Rect brushRect;
	if( IsAnim( brush ) )
	{
		const auto& theAnim = Tile2Anim( brush );

		// const auto curTile = Tile2Char( brush );
		// const Vei2 addAmount = GetTileOffset( curTile );
		const Vei2 addAmount = { 0,0 };
		
		theAnim.Draw( brushPos + addAmount,gfx,
			SpriteEffect::Chroma{ Colors::Magenta },
			IsFlipped( brush ) );
		brushRect = Rect( theAnim.GetFrameRect() );
	}
	else
	{
		const Surface* const brushSpr = Tile2Surf( brush );
		gfx.DrawSprite( brushPos.x,brushPos.y,*brushSpr,
			SpriteEffect::Chroma{ Colors::Magenta },
			IsFlipped( brush ) );
		brushRect = brushSpr->GetRect();
	}

	// auto brushRect = brushSpr->GetRect().GetMovedBy( brushPos );
	brushRect.MoveBy( brushPos );
	static constexpr auto tSize = TileMap::GetTileSize();
	const auto realPos = Vei2{ brushPos.x / tSize.x,brushPos.y / tSize.y };
	if( realPos.x == 0 || realPos.x == nTiles.x ||
		realPos.y == 0 || realPos.y == nTiles.y ||
		brushRect.bottom >= 600 )
	{
		brushRect = brushRect.GetExpanded( -1 );
	}

	gfx.DrawHitbox( brushRect,Colors::Green );

	// gfx.DrawLine( menuCenter + Vei2{ 0,1 },menuCenter + Vei2{ 0,Graphics::ScreenHeight - 1 },Colors::Red );

	// This should go on top of pretty much everything.
	if( fadeProgress > 0.0f )
	{
		textFont->DrawText( "Saved Successfully!",
			{ 150,150 },Colors::White,
			SpriteEffect::SubstituteFade{ Colors::White,
			Colors::White,fadeProgress },gfx );
	}
}

bool LevelEditor::CheckReturning( const Mouse& ms )
{
	backToMenu.Update( ms );

	return( backToMenu.IsDown() );
}

void LevelEditor::PutTile( int x,int y,Tile2Char c )
{
	tiles[y * nTiles.x + x] = c;
}

void LevelEditor::PublishLevel()
{
	const auto name = GetNewestFileName();
	WriteToFile( name );
	fadeProgress = 1.0f;
}

void LevelEditor::WriteToFile( const std::string& fileName ) const
{
	std::ofstream out( fileName );
	assert( out.good() );

	for( int y = 0; y < nTiles.y; ++y )
	{
		for( int x = 0; x < nTiles.x; ++x )
		{
			out << char( GetTile( x,y ) );
			// Don't have commas at the end of each line.
			if( x < nTiles.x - 1 ) out << ',';
		}
		// Make sure you don't have a blank line at the end.
		if( y < nTiles.y - 1 ) out << '\n';
	}
}

std::string LevelEditor::GetNewestFileName() const
{
	const std::string baseName = "Maps/Map";
	for( int i = 0; true; ++i )
	{
		const std::string name = baseName + std::to_string( i ) + ".lvl";
		const std::ifstream test( name );

		if( !test.good() )
		{
			return( baseName + std::to_string( i ) + ".lvl" );
		}
	}
	// You should *really* never get this.
	assert( false );
	return( baseName );
}

LevelEditor::Tile2Char LevelEditor::GetTile( int x,int y ) const
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
	case Tile2Char::DeathBall:
		return( &deathBallSpr );
	case Tile2Char::SpikeWallLeft:
		return( &spikeWallSpr );
	case Tile2Char::SpikeWallRight:
		return( &spikeWallSpr );
	case Tile2Char::Eggsploder:
		return( &eggsploderSpr );
	default:
		// You will never get this!
		assert( false );
		return( &floorSpr );
	}
}

const Anim& LevelEditor::Tile2Anim( Tile2Char toTest ) const
{
	switch( toTest )
	{
	case Tile2Char::DeathBall:
		return( deathBallAnim );
	case Tile2Char::SpikeWallLeft:
	case Tile2Char::SpikeWallRight:
		return( spikeWallAnim );
	default:
		// This should never happen since anything that
		//  isn't an anim won't have an anim.
		assert( false );
		return( spikeWallAnim );
	}
}

bool LevelEditor::IsFlipped( Tile2Char test ) const
{
	switch( test )
	{
	case Tile2Char::SpikeWallRight:
		return( true );
	default:
		return( false );
	}

}

bool LevelEditor::IsAnim( Tile2Char toTest ) const
{
	switch( toTest )
	{
	case Tile2Char::DeathBall:
	case Tile2Char::SpikeWallLeft:
	case Tile2Char::SpikeWallRight:
		return( true );
	default:
		return( false );
	}
}

Vei2 LevelEditor::GetTileOffset( Tile2Char toTest ) const
{
	switch( toTest )
	{
	case Tile2Char::SpikeWallLeft:
		return( Vei2{ -1,0 } );
	case Tile2Char::SpikeWallRight:
		return( Vei2{ 1,0 } );
	default:
		return( Vei2{ 0,0 } );
	}
}
