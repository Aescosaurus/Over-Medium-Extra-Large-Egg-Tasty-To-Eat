#pragma once

#include "Vec2.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "TileMap.h"
#include "Collideable.h"
#include "Rect.h"
#include "Timer.h"
#include <vector>
#include "Bullet.h"
#include "Mouse.h"
#include "Surface.h"
#include "Codex.h"

// Works like top down now, but can be modified
//  to use platformer controls.
class Player
{
public:
	Player( const Vec2& pos,const Collideable& coll,
		std::vector<Bullet>& bullets );

	void Update( const Keyboard& kbd,const Mouse& ms,
		const TileMap& map,const Collideable& coll,float dt );
	void Draw( Graphics& gfx ) const;

	void Attack();
	void SetTopLeft( const Vec2& newPos );

	const Vec2& GetPos() const;
	const Rect& GetRect() const;
private:
	const Vec2 GetCenter() const;
private:
	Vec2 pos;
	static constexpr Vei2 size = { 32,32 }; // from 24, 24
	static constexpr float speed = 140.0f;
	const Collideable& coll;
	Rect hitbox;
	static constexpr float bulletSpeed = 390.1f;
	std::vector<Bullet>& myBullets;
	Timer shotTimer;
	static constexpr float refireTime = 0.21f;
	// const Surface mySpr = { "Images/Player.bmp",size.x,size.y };
	const Surface* const mySpr = Codex<Surface>::RetrieveSurf( "Images/Player.bmp",size );
};