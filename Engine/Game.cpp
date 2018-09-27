/******************************************************************************************
 *	Chili DirectX Framework Version 16.07.20											  *
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "FrameTimer.h"
#include "Collideable.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	curLevel( "Maps/_Tutorial.lvl" ),
	tiles( curLevel ),
	guy( { -50.0f,-50.0f },coll,bullets ),
	coll( tiles,gfx ),
	stairwell( { -50.0f,-50.0f } )
{
	ChangeLevel( curLevel );
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	auto dt = FrameTimer::Mark();
	if( dt > 0.5f ) dt = 0.0f;

	guy.Update( wnd.kbd,wnd.mouse,tiles,coll,dt );

	if( stairwell.GetRect().IsOverlappingWith( guy.GetRect() ) )
	{
		ChangeLevel( tiles.GetRandLvlName() );
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

		keyWalls[0].Unlock();
	}
}

void Game::ComposeFrame()
{
	tiles.Draw( gfx );

	for( const auto& kWall : keyWalls ) kWall.Draw( gfx );
	theKey.Draw( gfx );

	stairwell.Draw( gfx );

	for( const auto& e : enemies ) e.Draw( gfx );

	for( const auto& b : enemyBullets ) b.Draw( gfx );

	for( const auto& b : bullets ) b.Draw( gfx );

	// Draw outline
	// const auto pos = tiles.GetTilePos( guy.GetPos() );
	// tiles.DrawTileAt( pos.x,pos.y,Colors::Cyan,gfx );
	guy.Draw( gfx );

	// if( wnd.mouse.IsInWindow() )
	// {
	// 	const auto msPos = tiles.GetTilePos( wnd.mouse.GetPos() );
	// 	tiles.DrawTileAt( msPos.x,msPos.y,Colors::Green,gfx );
	// }
}

void Game::ChangeLevel( const std::string& nextLevel )
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
	const auto tempWalls = tiles.FindAllInstances( nextLevel,
		TileMap::Token::KeyWall );
	for( const Vei2& thePos : tempWalls )
	{
		keyWalls.emplace_back( KeyWall{ thePos,tiles } );
	}

	theKey.SpawnAt( tiles.FindFirstInstance( nextLevel,TileMap::Token::Key ) );

	stairwell.Spawn( tiles.FindFirstInstance( nextLevel,
		TileMap::Token::Stairs ) );
	stairwell.Deactivate();

	// Enemies list should already be empty.
	bullets.clear();
	enemyBullets.clear();
}