#pragma once

#include "Vec2.h"
#include "Rect.h"
#include "TileMap.h"

class Bullet
{
public:
	enum class Team
	{
		Player,
		Enemy
	};
public:
	Bullet() = delete;
	Bullet( const Vec2& pos,const Vec2& target );

	void Update( const TileMap& map,float dt );
	void Draw( Graphics& gfx ) const;

	bool IsDead() const;
	const Rect& GetRect() const;
private:
	static constexpr float speed = 70.0f;
	Vec2 pos;
	static constexpr Vei2 size = { 16,16 };
	Rect hitbox;
	Vec2 vel;
	// Team myTeam;
	bool dead = false;
};