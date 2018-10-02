#include "Campaign.h"
#include "FrameTimer.h"

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
	auto dt = FrameTimer::Mark();
	if( dt > 0.5f ) dt = 0.0f;

	guy.Update( kbd,mouse,tiles,coll,dt );

	if( stairwell.IsActive() &&
		stairwell.GetRect().IsOverlappingWith( guy.GetRect() ) )
	{
		ChangeLevel( tiles.GetNextLvlName() );
	}

	for( auto it = enemies.begin(); it < enemies.end(); ++it )
	{
		auto& e = *it;
		e.Update( tiles,guy.GetPos(),dt );

		const auto& eRect = e.GetRect();
		for( auto& b : bullets )
		{
			if( eRect.IsOverlappingWith( b.GetRect() ) )
			{
				b.Kill();
				e.Attack();
			}
		}

		if( e.IsDead() )
		{
			if( enemies.size() == 1 )
			{
				// stairwell.Spawn( it->GetPos() );
				stairwell.Activate();
			}
			it = enemies.erase( it );
			if( it == enemies.end() ) break;
		}
	}

	for( auto it = bullets.begin(); it < bullets.end(); ++it )
	{
		auto& b = *it;
		b.Update( tiles,dt );

		if( b.IsDead() )
		{
			it = bullets.erase( it );
			if( it == bullets.end() ) break;
		}
	}

	const Rect& playerBox = guy.GetRect();
	for( auto& it = enemyBullets.begin();
		it < enemyBullets.end();
		++it )
	{
		auto& b = *it;
		b.Update( tiles,dt );

		if( b.GetRect().IsOverlappingWith( playerBox ) )
		{
			guy.Attack();
		}

		if( b.IsDead() )
		{
			it = enemyBullets.erase( it );
			if( it == enemyBullets.end() ) break;
		}
	}

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

	for( const auto& e : enemies ) e.Draw( gfx );

	for( const auto& b : enemyBullets ) b.Draw( gfx );

	for( const auto& b : bullets ) b.Draw( gfx );

	guy.Draw( gfx );
}

void Campaign::ChangeLevel( const std::string& nextLevel )
{
	tiles.LoadFile( nextLevel );

	guy.SetTopLeft( Vec2( tiles.FindFirstInstance( nextLevel,
		TileMap::Token::Player ) + tiles.GetTileSize() / 2 ) );

	// Create all enemies.
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

	const auto keyList = tiles.FindAllInstances( nextLevel,TileMap::Token::Key );
	if( keyList.size() == 1 )
	{
		theKey.SpawnAt( keyList[0] );
	}

	stairwell.Spawn( tiles.FindFirstInstance( nextLevel,
		TileMap::Token::Stairs ) );
	stairwell.Deactivate();

	// Enemies list should already be empty.
	bullets.clear();
	enemyBullets.clear();
}
