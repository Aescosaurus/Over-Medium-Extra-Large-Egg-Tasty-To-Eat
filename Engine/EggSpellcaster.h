#pragma once

#include "EnemyBase.h"
#include "Bullet.h"
#include <memory>
#include <vector>
#include "Codex.h"
#include "Surface.h"
#include "Graphics.h"
#include "Timer.h"

class EggSpellcaster
	:
	public EnemyBase
{
public:
	EggSpellcaster() = delete;
	EggSpellcaster( const Vec2& pos,const TileMap& map,
		const Collideable& coll,std::vector<std::unique_ptr<Bullet>>& bullets );

	void Update( const Vec2& playerPos,float dt ) override;
	void Draw( Graphics& gfx ) const override;
private:
	Vec2 GetCenter() const;
private:
	static constexpr Vei2 size = { 32,32 };
	static constexpr int myHP = 4;
	std::vector<std::unique_ptr<Bullet>>& spellBullets;
	const Surface* const sprSheet = Codex<Surface>::RetrieveSurf( "Images/EggSpellcaster.bmp",{ 4,4 } );
	Anim bodyCrack;
	Anim legs;
	static constexpr float refireTime = 2.4f;
	Timer shotTimer;
};