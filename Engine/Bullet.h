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
	Bullet( const Vec2& pos,const Vec2& target,float speed );

	void Update( const TileMap& map,float dt );
	void Draw( Graphics& gfx ) const;

	void Kill();

	bool IsDead() const;
	const Vec2& GetPos() const;
	const Rect& GetRect() const;
private:
	Vec2 pos;
	static constexpr Vei2 size = { 16,16 };
	Rect hitbox;
	Vec2 vel;
	// Team myTeam;
	bool dead = false;
};