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
#include <cassert>
#include "SpriteEffect.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	theMainGame( gfx )
{
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
	switch( curState )
	{
	case GameState::Menu:
		playCampaign.Update( wnd.mouse );
		playUnlimited.Update( wnd.mouse );
		startLevelEditor.Update( wnd.mouse );

		quitGame.Update( wnd.mouse );

		if( playCampaign.IsDown() ) curState = GameState::Campaign;
		if( playUnlimited.IsDown() ) curState = GameState::Unlimited;
		if( startLevelEditor.IsDown() ) curState = GameState::LevelEditor;
		if( quitGame.IsDown() ) wnd.Kill();
		break;
	case GameState::Campaign:
		theMainGame.UpdateAll( wnd.kbd,wnd.mouse );
		break;
	case GameState::Unlimited:
		break;
	case GameState::LevelEditor:
		theLevelEditor.Update( wnd.mouse );
		if( theLevelEditor.CheckReturning( wnd.mouse ) )
		{
			curState = GameState::Menu;
		}
		break;
	default:
		assert( false );
		break;
	}
}

void Game::ComposeFrame()
{
	switch( curState )
	{
	case GameState::Menu:
		playCampaign.Draw( gfx );
		playUnlimited.Draw( gfx );
		startLevelEditor.Draw( gfx );
		quitGame.Draw( gfx );
		break;
	case GameState::Campaign:
		theMainGame.DrawAll();
		break;
	case GameState::Unlimited:
		break;
	case GameState::LevelEditor:
		theLevelEditor.Draw( gfx );
		break;
	default:
		assert( false );
		break;
	}
}