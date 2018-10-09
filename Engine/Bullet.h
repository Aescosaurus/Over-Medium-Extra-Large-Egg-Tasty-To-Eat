#pragma once

#include "Vec2.h"
#include "Rect.h"
#include "TileMap.h"
#include "Codex.h"
#include "Anim.h"
#include "Timer.h"

class Bullet
{
public:
	enum class Team
	{
		Player = 0,
		Enemy = 1,
		Spell = 2
	};
public:
	Bullet() = delete;
	Bullet( const Vec2& pos,const Vec2& target,
		float speed,Team myTeam );

	Bullet( const Bullet& other );
	Bullet& operator=( const Bullet& other );

	virtual void Update( const TileMap& map,float dt );
	void Draw( Graphics& gfx ) const;

	void Kill();

	bool IsDead() const;
	const Vec2& GetPos() const;
	const Rect& GetRect() const;
protected:
	void MoveTo( const Vec2& movedTo );
private:
	Vec2 GetCenter() const;
private:
	Vec2 pos;
	static constexpr Vei2 size = { 16,16 };
	Rect hitbox;
	Vec2 vel;
	Team myTeam;
	bool dead = false;
	const Surface* const sprSheet = Codex<Surface>::RetrieveSurf( "Images/BulletAnim.bmp",{ 4,4 } );
	Anim rotate;
	// IF YOU ADD NEW MEMBERS UPDATE COPY CTOR AND COPY EQUALS!
};

// Bullet that moves in a figure eight pattern and ignores walls.
class SpellBullet
	:
	public Bullet
{
public:
	SpellBullet( const Vec2& pos );

	// Don't destroy if colliding with walls, at least for now.
	void Update( const TileMap& map,float dt ) override;
private:
	Vec2 startPos;
	static constexpr Vei2 amplitude = { 50,25 };
	static constexpr int freq = 5;
	Timer t;
	static constexpr float cycleTime = 1.0f; // from 5
	static constexpr float deg2rad = ( 2.0f * 3.14159265f ) / 360.0f;
};