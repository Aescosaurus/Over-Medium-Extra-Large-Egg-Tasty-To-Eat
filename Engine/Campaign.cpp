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

	// Typed this out just so we're clear what's going on:
	//  This is a vector of unique pointers to EnemyBases,
	//  We don't have to use a ref since it's just a tiny
	//  little pointer, but it has to be a ref since it
	//  can be any child of EnemyBase.
	for( std::unique_ptr<EnemyBase>& e : eggs )
	{
		e->Update( guyPos,dt );

		const auto& eRect = e->GetRect();

		if( guyRect.IsOverlappingWith( eRect ) )
		{
			guy.Attack( e->GetPos() );
		}

		for( auto& b : bullets )
		{
			if( eRect.IsOverlappingWith( b.GetRect() ) )
			{
				e->Attack( guy.GetPos() );
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

	// for( auto& db : deathBalls ) db.Update( tiles,coll,dt );

	chili::remove_erase_if( eggs,std::mem_fn( &EnemyBase::IsDead ) );
	chili::remove_erase_if( bullets,std::mem_fn( &Bullet::IsDead ) );
	chili::remove_erase_if( enemyBullets,std::mem_fn( &Bullet::IsDead ) );

	if( eggs.size() < 1 ) stairwell.Activate();

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

	// for( const auto& db : deathBalls ) db.Draw( gfx );

	for( const auto& e : eggs ) e->Draw( gfx );

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

	// All egg enemy stuff.
	eggs.clear(); // Should be empty anyway, but if you
	//  skip levels it won't be so do this just in case.
	
	// Create all egg soldier enemies.
	const auto enemyList = tiles.FindAllInstances( nextLevel,
		TileMap::Token::Enemy );
	for( const Vei2& thePos : enemyList )
	{
		// enemies.emplace_back( Enemy{ Vec2( thePos ),
		// 	tiles,coll,enemyBullets } );
		eggs.emplace_back( new EggSoldier{ thePos,tiles,coll,enemyBullets } );
	}

	// Eggsploder enemy type.
	const auto eggsploderList = tiles.FindAllInstances( nextLevel,
		TileMap::Token::Eggsploder );
	for( const Vei2& thePos : eggsploderList )
	{
		eggs.emplace_back( new Eggsploder{ thePos,tiles,coll } );
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

	// No more death balls, for now.
	// Add all death balls to the vector.
	// deathBalls.clear();
	// const auto deathBallList = tiles.FindAllInstances( nextLevel,TileMap::Token::DeathBall );
	// for( const Vei2& deathBallPos : deathBallList )
	// {
	// 	deathBalls.emplace_back( DeathBall{ deathBallPos } );
	// }
}
