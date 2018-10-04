#include "Campaign.h"
#include "FrameTimer.h"
#include "Utils.h"
#include <functional>

Campaign::Campaign( Graphics& gfx )
	:
	gfx( gfx ),
	curLevel( "Maps/_Tutorial.lvl" ),
	tiles( curLevel ),
	guy( { -50.0f,-50.0f },coll,bullets ),
	coll( tiles,gfx ),
	stairwell( { -50.0f,-50.0f } )
{
	ChangeLevel( curLevel );
}

void Campaign::UpdateAll( const Keyboard& kbd,
	const Mouse& mouse )
{
#if !NDEBUG
	if( kbd.KeyIsPressed( VK_SPACE ) )
	{
		if( canChangeLvl )
		{
			ChangeLevel( tiles.GetNextLvlName() );
			canChangeLvl = false;
		}
	}
	else canChangeLvl = true;
#endif

	auto dt = FrameTimer::Mark();
	if( dt > 0.5f ) dt = 0.0f;

	guy.Update( kbd,mouse,tiles,coll,dt );

	const auto& guyPos = guy.GetPos();
	const auto& guyRect = guy.GetRect();

	if( stairwell.IsActive() &&
		stairwell.GetRect().IsOverlappingWith( guy.GetRect() ) )
	{
		ChangeLevel( tiles.GetNextLvlName() );
	}
	
	for( auto& b : bullets ) b.Update( tiles,dt );

	for( auto& e : enemies )
	{
		e.Update( tiles,guyPos,dt );

		const auto& eRect = e.GetRect();

		if( guyRect.IsOverlappingWith( eRect ) )
		{
			guy.Attack( e.GetPos() );
		}

		for( auto& b : bullets )
		{
			if( eRect.IsOverlappingWith( b.GetRect() ) )
			{
				e.Attack();
				b.Kill();
			}
		}
	}

	for( auto& eb : enemyBullets )
	{
		eb.Update( tiles,dt );

		if( guyRect.IsOverlappingWith( eb.GetRect() ) )
		{
			guy.Attack( eb.GetPos() );
			eb.Kill();
		}
	}

	for( auto& sw : spikeWalls )
	{
		sw.Update( guyRect,dt );

		if( guyRect.IsOverlappingWith( sw.GetRect() ) )
		{
			guy.Attack( sw.GetPos() );
		}
	}

	for( auto& db : deathBalls )
	{
		db.Update( tiles,coll,dt );
	}

	chili::remove_erase_if( enemies,std::mem_fn( &Enemy::IsDead ) );
	chili::remove_erase_if( bullets,std::mem_fn( &Bullet::IsDead ) );
	chili::remove_erase_if( enemyBullets,std::mem_fn( &Bullet::IsDead ) );

	if( enemies.size() < 1 ) stairwell.Activate();

	if( guy.GetRect().IsOverlappingWith( theKey.GetRect() ) )
	{
		gotKey = true;
		theKey.Despawn();

		// keyWalls[0].Unlock();
		for( auto& keyWall : keyWalls )
		{
			keyWall.Unlock();
		}
	}
}

void Campaign::DrawAll()
{
	tiles.Draw( gfx );

	for( const auto& kWall : keyWalls ) kWall.Draw( gfx );
	theKey.Draw( gfx );

	stairwell.Draw( gfx );

	for( const auto& sw : spikeWalls ) sw.Draw( gfx );

	for( const auto& db : deathBalls ) db.Draw( gfx );

	for( const auto& e : enemies ) e.Draw( gfx );

	for( const auto& b : enemyBullets ) b.Draw( gfx );

	for( const auto& b : bullets ) b.Draw( gfx );

	guy.Draw( gfx );
}

void Campaign::ChangeLevel( const std::string& nextLevel )
{
	tiles.LoadFile( nextLevel );

	const auto guyPos = Vec2( tiles.FindFirstInstance( nextLevel,
		TileMap::Token::Player ) + tiles.GetTileSize() / 2 );
	guy.SetTopLeft( guyPos );

	// Enemies list should already be empty but clear just in case.
	bullets.clear();
	enemyBullets.clear();

	// Create all enemies.
	enemies.clear();
	const auto list = tiles.FindAllInstances( nextLevel,
		TileMap::Token::Enemy );
	for( const Vei2& thePos : list )
	{
		enemies.emplace_back( Enemy{ Vec2( thePos ),
			tiles,coll,enemyBullets } );
	}

	// Create all key walls.
	keyWalls.clear();
	const auto tempWalls = tiles.FindAllInstances( nextLevel,
		TileMap::Token::KeyWall );
	for( const Vei2& thePos : tempWalls )
	{
		keyWalls.emplace_back( KeyWall{ thePos,tiles } );
	}

	// Create all keys.
	theKey.Despawn();
	const auto keyList = tiles.FindAllInstances( nextLevel,TileMap::Token::Key );
	if( keyList.size() == 1 )
	{
		theKey.SpawnAt( keyList[0] );
	}

	stairwell.Spawn( tiles.FindFirstInstance( nextLevel,
		TileMap::Token::Stairs ) );
	stairwell.Deactivate();

	// Initialize spike walls of ALL directions.  We don't discriminate.
	spikeWalls.clear();
	const auto lSpikeWallList = tiles.FindAllInstances( nextLevel,TileMap::Token::SpikeWallLeft );
	const auto rSpikeWallList = tiles.FindAllInstances( nextLevel,TileMap::Token::SpikeWallRight );
	for( const Vei2& wallPos : lSpikeWallList )
	{
		spikeWalls.emplace_back( SpikeWall{ wallPos,SpikeWall::Dir::Left } );
	}
	for( const Vei2& wallPos : rSpikeWallList )
	{
		spikeWalls.emplace_back( SpikeWall{ wallPos,SpikeWall::Dir::Right } );
	}

	// Add all death balls to the vector.
	deathBalls.clear();
	const auto deathBallList = tiles.FindAllInstances( nextLevel,TileMap::Token::DeathBall );
	for( const Vei2& deathBallPos : deathBallList )
	{
		deathBalls.emplace_back( DeathBall{ deathBallPos } );
	}
}
