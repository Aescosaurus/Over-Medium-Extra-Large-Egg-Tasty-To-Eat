#include "LevelEditor2.h"
#include "FrameTimer.h"
#include "SpriteEffect.h"

LevelEditor2::LevelEditor2()
{
	const auto nTotalTiles = nTiles.x * nTiles.y;
	tiles.reserve( nTotalTiles );

	for( int y = 0; y < nTiles.y; ++y )
	{
		for( int x = 0; x < nTiles.x; ++x )
		{
			auto fill = TileType::Empty;
			if( x == 0 || y == 0 ||
				x == nTiles.x - 1 || y == nTiles.y - 1 )
			{
				fill = TileType::Wall;
			}
			tiles.emplace_back( fill );
		}
	}

	surfs.insert( { TileType::Empty,{ "Images/Floor.bmp",4,4 } } );
	surfs.insert( { TileType::Wall,{ "Images/Wall1.bmp",4,4 } } );
	surfs.insert( { TileType::Player,{ { "Images/PlayerAnim.bmp",RectI{ 0,8,0,8 } },4,4 } } );
	surfs.insert( { TileType::Stairs,{ "Images/Stairs.bmp",4,4 } } );
	surfs.insert( { TileType::EggSoldier,{ { "Images/EggSoldierAnim.bmp",RectI{ 0,8,0,8 } },4,4 } } );
	surfs.insert( { TileType::Eggsploder,{ { "Images/EggsploderAnim.bmp",RectI{ 0,8,0,8 } },4,4 } } );
	// other stuff
	// other stuff
	surfs.insert( { TileType::KeyWall,{ "Images/KeyWall.bmp",4,4 } } );
	surfs.insert( { TileType::Key,{ "Images/Key.bmp",4,4 } } );
	surfs.insert( { TileType::SpikeWallLeft,{ { "Images/SpikeWallAnim.bmp",RectI{ 40,50,0,10 } },4,4 } } );
	surfs.insert( { TileType::SpikeWallRight,{ { "Images/SpikeWallAnim.bmp",RectI{ 40,50,0,10 } },4,4 } } );

	// TODO: Find a way to place these really really easily.
	// pos = { start.x,start.x }
	// buttons.emplace_back( ImageButton{ pos += { x,y } } )
	// buttons.emplace_back( ImageButton{ pos += { x,y } } )
	// buttons.emplace_back( ImageButton{ pos += { x,y } } )
	// pos.x = start
	// pos.y += lineHeight
	// buttons.emplace_back( ImageButton{ pos += { x,y } } )
	// 	...
	static constexpr Vei2 xAdd = { 50,0 };
	static constexpr Vei2 yAdd = { 0,50 };
	static constexpr Vei2 buttonSize = { 48,48 };
	Vei2 pos = start - xAdd + Vei2{ 1,0 };

	buttons.emplace_back( ImageButton{ pos += xAdd,buttonSize,surfs.at( TileType::Empty ) } );
	buttons.emplace_back( ImageButton{ pos += xAdd,buttonSize,surfs.at( TileType::Wall ) } );
	buttons.emplace_back( ImageButton{ pos += xAdd,buttonSize,surfs.at( TileType::Player ) } );
	buttons.emplace_back( ImageButton{ pos += xAdd,buttonSize,surfs.at( TileType::Stairs ) } );

	pos.x = start.x - xAdd.x + 1;
	pos += yAdd;

	buttons.emplace_back( ImageButton{ pos += xAdd,buttonSize,surfs.at( TileType::EggSoldier ) } );
	buttons.emplace_back( ImageButton{ pos += xAdd,buttonSize,surfs.at( TileType::Eggsploder ) } );
	pos += xAdd;
	pos += xAdd;

	pos.x = start.x - xAdd.x + 1;
	pos += yAdd;

	buttons.emplace_back( ImageButton{ pos += xAdd,buttonSize,surfs.at( TileType::KeyWall ) } );
	buttons.emplace_back( ImageButton{ pos += xAdd,buttonSize,surfs.at( TileType::Key ) } );
	buttons.emplace_back( ImageButton{ pos += xAdd,buttonSize,surfs.at( TileType::SpikeWallLeft ) } );
	buttons.emplace_back( ImageButton{ pos += xAdd,buttonSize,surfs.at( TileType::SpikeWallRight ) } );
}

void LevelEditor2::Update( const Mouse& ms )
{
	const float dt = FrameTimer::Mark();

	auto tempPos = ms.GetPos();

	while( tempPos.x >= Graphics::GameScreenWidth ) --tempPos.x;
	while( tempPos.x % tileSize.x != 0 ) --tempPos.x;
	while( tempPos.y % tileSize.y != 0 ) --tempPos.y;

	brushPos = tempPos;

	// (update buttons)

	// (check button presses)

	if( ms.LeftIsPressed() && wndRect.ContainsPoint( ms.GetPos() ) )
	{
		static constexpr auto tSize = TileMap::GetTileSize();
		const auto realPos = Vei2{ brushPos.x / tSize.x,brushPos.y / tSize.y };

		PutTile( realPos.x,realPos.y,brush );
	}

	if( fadeProgress > 0.0f )
	{
		fadeProgress -= fadeSpeed * dt * 60.0f;
		if( fadeProgress < 0.0f ) fadeProgress = 0.0f;
	}
}

void LevelEditor2::Draw( Graphics& gfx ) const
{
	// Set up sprite effect functors.
	const auto copy = SpriteEffect::Copy{};
	const auto chroma = SpriteEffect::Chroma{ Colors::Magenta };

	// Draw all the tiles.
	for( int y = 0; y < nTiles.y; ++y )
	{
		for( int x = 0; x < nTiles.x; ++x )
		{
			const auto curTile = GetTile( x,y );
			const auto& curSurf = surfs.at( curTile );

			gfx.DrawSprite( x * tileSize.x,y * tileSize.y,
				curSurf,copy,false );
		}
	}

	// Draw Buttons.
	for( const auto& b : buttons )
	{
		b.Draw( gfx );
	}
}

bool LevelEditor2::CheckReturning( const Mouse& ms )
{
	return( false );
}

void LevelEditor2::PutTile( int x,int y,TileType t )
{
	tiles[y * nTiles.x + x] = t;
}

LevelEditor2::TileType LevelEditor2::GetTile( int x,int y ) const
{
	return( tiles[y * nTiles.x + x] );
}