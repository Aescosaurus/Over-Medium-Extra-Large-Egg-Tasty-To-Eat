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
	tiles( "Maps/Map1.lvl" ),
	guy( { 150.0f,150.0f },coll ),
	coll( tiles,gfx )
{
	const auto list = tiles.GetEnemies( "Maps/Map1.lvl" );
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

	guy.Update( wnd.kbd,tiles,coll,dt );

	for( auto& e : enemies )
	{
		e.Update( tiles,guy.GetPos(),dt );
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

	for( const auto& e : enemies )
	{
		e.Draw( gfx );
	}

	for( const auto& b : enemyBullets )
	{
		b.Draw( gfx );
	}

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
