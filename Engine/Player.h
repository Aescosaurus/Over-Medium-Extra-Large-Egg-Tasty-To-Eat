#pragma once

#include "Vec2.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "TileMap.h"
#include "Collideable.h"
#include "Rect.h"
#include "Timer.h"

// Works like top down now, but can be modified
//  to use platformer controls.
class Player
{
public:
	Player( const Vec2& pos,const Collideable& coll );

	void Update( const Keyboard& kbd,
		const TileMap& map,const Collideable& coll,float dt );
	void Draw( Graphics& gfx ) const;

	void Attack();

	const Vec2& GetPos() const;
	const Rect& GetRect() const;
private:
	const Vec2 GetCenter() const;
private:
	Vec2 pos;
	static constexpr Vei2 size = { 24,24 }; // from 32,32
	static constexpr float speed = 140.0f;
	const Collideable& coll;
	Rect hitbox;
};