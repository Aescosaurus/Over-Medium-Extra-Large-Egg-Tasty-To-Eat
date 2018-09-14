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
	curLevel( "Maps/Map0.lvl" ),
	tiles( curLevel ),
	guy( { -50.0f,-50.0f },coll,bullets ),
	coll( tiles,gfx )
{
	guy.SetTopLeft( Vec2( tiles.FindFirstInstance( curLevel,
		TileMap::Token::Player ) + tiles.GetTileSize() / 2 ) );

	const auto list = tiles.FindAllInstances( curLevel,
		TileMap::Token::Enemy );
	for( const Vei2& thePos : list )
	{
		enemies.emplace_back( Enemy{ Vec2( thePos ),
			tiles,coll,enemyBullets } );
	}
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
}

void Game::ComposeFrame()
{
	tiles.Draw( gfx );

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
